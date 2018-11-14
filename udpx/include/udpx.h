/*************************************************************************
	> File Name: udpx.h
	> Author: 
	> Mail: 
	> Created Time: Fri 27 Jul 2018 02:14:45 PM CST
 ************************************************************************/

#ifndef _UDPX_H
#define _UDPX_H

#include <arpa/inet.h>
#include <pthread.h>

#include "list.h"
#include "udpx_buffer.h"
#include "udpx_int_ring_buffer.h"

#define UDPX_MAGIC (0x5957)
#define UDPX_VERSION (1)

#define UDPX_MSG_CATEGORY_REGISTER (0x80000001)
#define UDPX_MSG_CATEGORY_ACK (0x80000000)
#define UDPX_MSG_CATEGORY_DATA (0)

/**
 * 通信协议头.
 */
typedef struct _udpx_header {
    unsigned short magic;
    unsigned char version:4;
    unsigned char flag:4;
    char reserved;

    /**
     * 0长数组. 用于判断消息类别.
     * 0x80000001: 注册报文
     * 0x80000000: ACK报文
     * 0x0xxxxxxx: 数据报文
     */
    unsigned int type[0];

    union {
        struct {
            int msg_category; // 控制类别
            int msg_type; // 控制类别子类型
            int session_id; // 通信会话ID
        } control_header; // 控制类头部

        struct {
            int seq_no; // 数据包序号
            int msg_no; // 消息编号
            int session_id; // 通信会话ID
        } data_header; // 数据类头部
    };

    /**
     * 用于索引后续内容
     */
    unsigned char content[0];
} udpx_header;

/**
 * 通信消息响应.
 */
typedef struct _udpx_ack {
    int seq_no; // 确认序号
    int from_ip; // 来源IP. 可以为0
    short from_port; // 来源端口. 可以为0
} udpx_ack;

/**
 * 通信消息头.
 */
typedef struct _udpx_msg_header {
    unsigned char reserved[8];
    int src_server_id; // 源服务器ID
    int src_obj_id; // 源对象ID
    int dst_server_id; // 目标服务器ID
    int dst_obj_id; // 目标对象ID

    int msg_type; // 消息类型
    int msg_id; // 消息序列号
    int session_id; // 会话ID
    int payload_length; // 载荷长度
    unsigned char content[0]; // 载荷
} udpx_msg_header;

/**
 * udpx运行状态.
 */
#define UDPX_STOPED (0)
#define UDPX_RUNNING (1)

/**
 * udpx socket消息接收超时时间. 单位: ms.
 */
#define UDPX_POLL_TIMEOUT (5 * 1000) // 5000 ms.

/**
 * 协议定义载荷最大长度. 
 * 超过的数据包须进行拆包处理.
 */
#define UDPX_PAYLOAD_MAX_LENGTH (1300)

/**
 * 一个协议消息的最小/最大长度.
 */
#define UDPX_MSG_MIN_LENGTH (sizeof(udpx_header) + sizeof(udpx_ack))
#define UDPX_MSG_MAX_LENGTH (sizeof(udpx_header) + UDPX_PAYLOAD_MAX_LENGTH)

/**
 * 协议相关Buffer的最大长度.
 */
#define UDPX_BUFFER_MAX_LENGTH (1400)

/**
 * 服务质量.
 */
typedef struct _udpx_qos {
    /**
     * 互斥锁.
     */
    pthread_mutex_t mutex;

    /**
     * 时间戳.
     * 单位: ms.
     */
    unsigned long timestamp;

    /**
     * 统计间隔.
     * 间隔: ms.
     */
    unsigned long statistic_interval;

    /**
     * 实际发送数据包总数.
     * socket发送出去的数据包总数.
     */
    unsigned int real_send_packets;

    /**
     * 成功发送数据包总数.
     * 收到ACK的数据包总数.
     */
    unsigned int success_send_packets;

    /**
     * 发送包丢包率.
     * 百分比. 
     * 计算公式: 100 - 100*{@var success_send_packets}/{@var real_send_packets}.
     */
    unsigned int send_packets_loss_rate;

    /**
     * 网络往返时延总和.
     * 用于统计{@var round_trip_time}
     */
    unsigned long round_trip_time_sum;

    /**
     * 网络往返时延.
     * 接收到ACK时间戳与其相应发送数据包发送时间戳差值.
     * 计算公式: {@var round_trip_time_sum}/{@var success_send_packets}
     */
    unsigned int round_trip_time;

    /**
     * 业务需求发送总字节数.
     * 业务需求发送出去的数据包总字节数. 
     * 用于业务需求发送流量统计.
     */
    unsigned long send_bytes;

    /**
     * 业务需求发送带宽.
     * 计算公式: {@var send_bytes}/{@var timestamp}.
     */
    unsigned int send_bandwidth;

    /**
     * socket实际发送总字节数.
     * socket实际发送出去的数据包总字节数. 
     * 用于socket实际发送流量统计.
     */
    unsigned long real_send_bytes;

    /**
     * socket实际发送带宽.
     * 计算公式: {@var real_send_bytes}/{@var timestamp}.
     */
    unsigned int real_send_bandwidth;

    /**
     * 成功发送总字节数.
     * socket成功发送出去的数据包总字节数. 
     * 用于成功发送流量统计.
     */
    unsigned long success_send_bytes;

    /**
     * 成功发送带宽.
     * 计算公式: {@var success_send_bytes}/{@var timestamp}.
     */
    unsigned int success_send_bandwidth;

    /**
     * 接收数据包总数.
     * 接收到的非ACK数据包总数.
     */
    unsigned int recv_packets;

    /**
     * 接收总字节数.
     * 接收到的非ACK数据包总字节数.
     * 用于接收流量统计.
     */
    unsigned long recv_bytes;

    /**
     * 接收带宽.
     * 计算公式: {@var recv_bytes}/{@var timestamp}.
     */
    unsigned int recv_bandwidth;
} udpx_qos;

/**
 * 通信模块实例信息.
 */
typedef struct _udpx_info {
    /**
     * 运行状态.
     * 0: 未运行
     * 1: 运行中
     */
    volatile int state;

    /**
     * 当前socket文件描述符.
     */
    int sock_fd;

    /**
     * socket发送时间戳.
     * 单位: us.
     */
    unsigned long send_timestamp;

    /**
     * 当前会话ID.
     */
    int session_id;

    /**
     * 发送包序列号.
     */
    unsigned int send_seq_no;

    /**
     * 发送消息序列号.
     */
    unsigned int send_msg_no;

    /**
     * 接收包序列号.
     */
    unsigned int recv_seq_no;

    /**
     * 接收消息序列号.
     */
    unsigned int recv_msg_no;

    /**
     * 接收线程PID.
     */
    pthread_t recv_thread_pid;

    /**
     * 发送策略线程PID.
     */
    pthread_t send_policy_thread_pid;

    /**
     * QoS统计线程PID.
     */
    pthread_t qos_statistic_thread_pid;

    /**
     * 当前服务器地址.
     */
    struct sockaddr_in sock_addr;

    /**
     * 发送/接收buffer池.
     */
    udpx_buffer_pool *send_buffer_pool;
    udpx_buffer_pool *recv_buffer_pool;

    /**
     * ack序号缓存.
     */
    udpx_int_ring_buffer *ack_buffer;

    /**
     * 已发送报文缓存链表.
     */
    unsigned int send_packet_list_length;
    struct list_head send_packet_list;
    pthread_mutex_t send_list_mutex;

    /**
     * 收到消息缓存链表.
     */
    unsigned int recv_message_list_length;
    struct list_head recv_message_list;
    pthread_mutex_t recv_list_mutex;

    /**
     * 条件变量. 超时阻塞等待ack消息.
     */
    pthread_cond_t ack_cond;
    pthread_mutex_t ack_cond_mutex;

    /**
     * 服务质量.
     */
    udpx_qos qos;

    /**
     * 服务质量. 用于快速统计.
     */
    udpx_qos qos_fast;

    /**
     * 服务质量. 用于慢速统计.
     */
    udpx_qos qos_slow;
} udpx_info;

// //////////////////// 

/**
 * 全局变量.
 */
extern udpx_info udpx_instance;

#ifdef __cplusplus
extern "C" {
#endif
    int udpx_init(const char *server_ip, short port);
    int udpx_fini();
    int udpx_start();
    void udpx_stop();

    void udpx_set_session_id(int session_id);
    udpx_buffer *udpx_get_buffer(int length);
    void udpx_free_buffer(udpx_buffer *buffer);
    int udpx_send_register(udpx_buffer *buffer);
    int udpx_send_data(udpx_buffer *buffer);

    udpx_buffer *udpx_get_message();
    udpx_buffer *udpx_get_message_timeout(int timeout);
    void udpx_free_message(udpx_buffer *buffer);
#ifdef __cplusplus    
}
#endif

#endif
