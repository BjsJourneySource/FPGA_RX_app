/*************************************************************************
	> File Name: udpx.c
	> Author: 
	> Mail: 
	> Created Time: Fri 27 Jul 2018 03:01:34 PM CST
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <poll.h>
#include <time.h>

// #define UDPX_DEBUG
#define LOG_TAG "udpx"

#include "udpx_log.h"
#include "udpx_errno.h"
#include "udpx_misc.h"
#include "udpx.h"

/**
 * 发送Buffer参数.
 */
#ifdef __ANDROID__
#define SEND_SMALL_BUFFER_LENGTH (1400)
#define SEND_SMALL_BUFFER_COUNT (1000)
#define SEND_BIG_BUFFER_LENGTH (1<<18) // 256KB
#define SEND_BIG_BUFFER_COUNT (50)
#else
#define SEND_SMALL_BUFFER_LENGTH (1400)
#define SEND_SMALL_BUFFER_COUNT (1000)
#define SEND_BIG_BUFFER_LENGTH (1<<18) // 256KB
#define SEND_BIG_BUFFER_COUNT (8)
#endif

/**
 * 接收Buffer参数.
 */
#define RECV_SMALL_BUFFER_LENGTH (1400)
#define RECV_SMALL_BUFFER_COUNT (400)
#define RECV_BIG_BUFFER_LENGTH (0) // 暂时接收部分不会收到大数据包.
#define RECV_BIG_BUFFER_COUNT (0)

/**
 * 协议常量.
 */
#define UDPX_REGISTER_SESSION_ID (0xFFFFFFFF)
#define UDPX_SEQ_NO_MASK (0xFFFFFFF)
#define UDPX_MSG_NO_MASK (0x3FFFFFFF)

/**
 * ACK缓存Buffer参数.
 */
#define UDPX_ACK_BUFFER_LENGTH (500)

/**
 * socket发送/接收缓冲区大小.
 */
#define UDPX_SOCKET_SNDBUF_LENGTH (64 * 1024)
#define UDPX_SOCKET_RCVBUF_LENGTH (256 * 1024)

/**
 * socket发送优先级.
 * 取值范围: 0-6.
 */
#define UDPX_SOCKET_PRIORITY (6)

/**
 * socket发送最小时间间隔.
 * 可根据丢包率对此值作修正. 例: 修正因子设为(1 + send_packets_loss_rate/100.0).
 * 单位: us.
 */
#define UDPX_SOCKET_SEND_INTERVAL_MIN (10)

/**
 * 发送数据包有效时长.
 * TODO 有效时长需根据不同类型报文设置.
 * 单位: ms.
 */
#define UDPX_PACKET_LIFE_TIME (1000)

/**
 * 丢包率大于该值时, 启动随机伪发送.
 */
#define UDPX_FAKE_SEND_LOSS_RATE_MIN (5)

/**
 * 伪发送的伪发送率最大百分比.
 */
#define UDPX_FAKE_SEND_FAKE_RATE_MAX (95)

/**
 * 由丢包率计算伪发送率.
 */
#define UDPX_FAKE_SEND_FAKE_RATE(loss_rate) \
            ((UDPX_FAKE_SEND_FAKE_RATE_MAX) * ((loss_rate) - (UDPX_FAKE_SEND_LOSS_RATE_MIN)) / (100 - (UDPX_FAKE_SEND_LOSS_RATE_MIN)))

/**
 * 同一数据包重发最小时间间隔因子.
 * 最小时间间隔为(round_trip_time * UDPX_PACKET_RETRY_SEND_INTERVAL_FACTOR).
 * 可根据丢包率对最小时间间隔作修正. 例: 修正为(round_trip_time * (UDPX_PACKET_RETRY_SEND_INTERVAL_FACTOR + send_packets_loss_rate/5/2.0)).
 * TODO 最小时间间隔需根据不同类型报文设置.
 * 单位: ms.
 */
#define UDPX_PACKET_RETRY_SEND_INTERVAL_FACTOR (2)

/**
 * 同一数据包重发次数最小值.
 * 重发时间间隔是: (UDPX_PACKET_LIFE_TIME / UDPX_PACKET_RETRY_SEND_TIME_MIN).
 */
#define UDPX_PACKET_RETRY_SEND_TIME_MIN (3)
#define UDPX_PACKET_RETRY_SEND_INTERVAL_MAX (UDPX_PACKET_LIFE_TIME / UDPX_PACKET_RETRY_SEND_TIME_MIN)

/**
 * QoS统计时间间隔.
 * 单位: ms.
 */
#define UDPX_QOS_FAST_STATISTIC_INTERVAL (1000)
#define UDPX_QOS_SLOW_STATISTIC_INTERVAL (60 * 1000)

/**
 * QoS统计, 周期内发送包数量小于该值时, 统计值不可靠.
 */
#define UDPX_QOS_SEND_PACKETS_DIFF_MIN (5)

/**
 * 网络往返时延缺省值.
 * 单位: ms.
 */
#define UDPX_ROUND_TRIP_TIME_DEFAULT (50)

/**
 * 全局变量.
 * udpx全局实例.
 */
udpx_info udpx_instance;

/**
 * 条件变量. 用于实现超时阻塞型{@func udpx_get_message_timeout()}.
 */
pthread_cond_t udpx_get_message_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t udpx_get_message_cond_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * 互斥锁. 用于保证socket串行发送.
 */
pthread_mutex_t udpx_socket_send_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * 互斥锁. 用于发送接口串行调用.
 */
pthread_mutex_t udpx_send_api_mutex = PTHREAD_MUTEX_INITIALIZER;

/**************************************************
 * UDPX头.
 **************************************************/

/**
 * @brief 通信协议头信息打印
 *
 * @param header    待打印的通信协议头
 */
void udpx_header_dump(udpx_header *header) {
    if (header == NULL) {
        return;
    }

    char *pBuffer = (char *) header;
    char buffer[1024];
    int position = 0;
    int i = 0;

    for (i = 0; i < sizeof(udpx_header); i++) {
        char c = pBuffer[i];
        position += sprintf(buffer + position, "%02X ", c);
    }
    LOGD("%s: %s\n", __func__, buffer);
}

/**
 * @brief 通信协议头通用初始化
 *
 * @param header    待初始化的通信协议头指针
 */
void udpx_header_init_common(udpx_header *header) {
    if (header == NULL) {
        LOGE("%s: header is null.\n", __func__);
        return;
    }

    header->magic = htons(UDPX_MAGIC);
    header->version = UDPX_VERSION;
    header->flag = 0;
    header->reserved = 0;
}

/**
 * @brief 注册报文通信协议头初始化
 *
 * @param header    待初始化的通信协议头指针
 */
void udpx_header_init_register(udpx_header *header) {
    if (header == NULL) {
        LOGE("%s: header is null.\n", __func__);
        return;
    }

    udpx_header_init_common(header);

    header->control_header.msg_category = htonl(UDPX_MSG_CATEGORY_REGISTER);
    header->control_header.msg_type = 0;
    header->control_header.session_id = htonl(UDPX_REGISTER_SESSION_ID);
}

/**
 * @brief ACK报文通信协议头初始化
 *
 * @param header    待初始化的通信协议头指针
 * @param session_id    通信会话ID
 */
void udpx_header_init_ack(udpx_header *header, int session_id) {
    if (header == NULL) {
        LOGE("%s: header is null.\n", __func__);
        return;
    }

    udpx_header_init_common(header);

    header->control_header.msg_category = htonl(UDPX_MSG_CATEGORY_ACK);
    header->control_header.msg_type = 0;
    header->control_header.session_id = htonl(session_id);
}

/**
 * @brief 数据报文通信协议头初始化
 *
 * @param header    待初始化的通信协议头指针
 * @param seq_no    发送报文序号
 * @param msg_no    发送消息序号
 * @param session_id    通信会话ID
 */
void udpx_header_init_data(udpx_header *header, int seq_no, int msg_no, int session_id) {
    if (header == NULL) {
        LOGE("%s: header is null.\n", __func__);
        return;
    }

    udpx_header_init_common(header);

    header->data_header.seq_no = htonl(seq_no & UDPX_SEQ_NO_MASK); // 报文序号: 低28bits
    header->data_header.msg_no = htonl(msg_no & UDPX_MSG_NO_MASK); // 消息序号: 低30bits
    header->data_header.session_id = htonl(session_id);
}

/**
 * @brief 设置数据报文分组开始/结束标识
 *
 * @param header    待设置的通信协议头
 * @param start_bit 数据报文分组开始标识
 * @param stop_bit  数据报文分组结束标识
 */
void udpx_header_data_set_control_bit(udpx_header *header, int start_bit, int stop_bit) {
    if (header == NULL) {
        LOGE("%s: header is null.\n", __func__);
        return;
    }

    int msg_no = ntohl(header->data_header.msg_no);

    if ((start_bit & 0x1) == 1) {
        msg_no |= (1 << 31);
    } else {
        msg_no &= ~(1 << 31);
    }

    if ((stop_bit & 0x1) == 1) {
        msg_no |= (1 << 30);
    } else {
        msg_no &= ~(1 << 30);
    }

    header->data_header.msg_no = htonl(msg_no);
}

/**************************************************
 * SOCKET封装.
 **************************************************/

/**
 * @brief socket发送
 *
 * @param buf   发送内容buffer指针
 * @param len   发送内容长度
 * @param flags {@func sendto()}的flags
 */
int udpx_socket_sendto(const void *buf, int len, int flags) {
    int retval = 0;
    unsigned long long timestamp = 0;
    int timestamp_diff = 0;
    struct sockaddr *sock_addr = (struct sockaddr *) &udpx_instance.sock_addr;
    socklen_t addrlen = sizeof(udpx_instance.sock_addr);

    udpx_qos *qos = &udpx_instance.qos_fast;
    int loss_rate = qos->send_packets_loss_rate;

    int socket_send_interval = UDPX_SOCKET_SEND_INTERVAL_MIN;
    if (loss_rate > 0 && loss_rate <= 100) {
        socket_send_interval = UDPX_SOCKET_SEND_INTERVAL_MIN * (100 + loss_rate) / 100;
    }

    // 加锁
    pthread_mutex_lock(&udpx_socket_send_mutex);
    timestamp = udpx_get_timestamp_us();
    timestamp_diff = (int) (timestamp - udpx_instance.send_timestamp);
    if (timestamp_diff < socket_send_interval) {
        usleep(socket_send_interval - timestamp_diff);
    }
    retval = sendto(udpx_instance.sock_fd, buf, len, flags, sock_addr, addrlen);
    if (retval == -1) {
        LOGW("%s: failed: %s.\n", __func__, strerror(errno));
    }
    udpx_instance.send_timestamp = udpx_get_timestamp_us();
    // 解锁
    pthread_mutex_unlock(&udpx_socket_send_mutex);

    return retval;
}

/**
 * @brief socket属性初始化
 */
void udpx_socket_options_init(int sock_fd) {
    int err = 0;
    int optval = 0;
    struct linger optlinger;
    socklen_t optlen = 0;

    // 是否处于监听模式.
    optlen = sizeof(optval);
    err = getsockopt(sock_fd, SOL_SOCKET, SO_ACCEPTCONN, &optval, &optlen);
    if (err == -1) {
        LOGW("%s: getsockopt(SO_ACCEPTCONN) failed: %s.\n", __func__, strerror(errno));
    }
    LOGI("%s: SO_ACCEPTCONN: %d.\n", __func__, optval);

    // 是否支持广播.
    optlen = sizeof(optval);
    err = getsockopt(sock_fd, SOL_SOCKET, SO_BROADCAST, &optval, &optlen);
    if (err == -1) {
        LOGW("%s: getsockopt(SO_BROADCAST) failed: %s.\n", __func__, strerror(errno));
    }
    LOGI("%s: SO_BROADCAST: %d.\n", __func__, optval);

    // 是否允许调试.
    optlen = sizeof(optval);
    err = getsockopt(sock_fd, SOL_SOCKET, SO_DEBUG, &optval, &optlen);
    if (err == -1) {
        LOGW("%s: getsockopt(SO_DEBUG) failed: %s.\n", __func__, strerror(errno));
    }
    LOGI("%s: SO_DEBUG: %d.\n", __func__, optval);

    // 获取错误状态.
    optlen = sizeof(optval);
    err = getsockopt(sock_fd, SOL_SOCKET, SO_ERROR, &optval, &optlen);
    if (err == -1) {
        LOGW("%s: getsockopt(SO_ERROR) failed: %s.\n", __func__, strerror(errno));
    }
    LOGI("%s: SO_ERROR: %d.\n", __func__, optval);

    // 是否禁用路由表.
    optlen = sizeof(optval);
    err = getsockopt(sock_fd, SOL_SOCKET, SO_DONTROUTE, &optval, &optlen);
    if (err == -1) {
        LOGW("%s: getsockopt(SO_DONTROUTE) failed: %s.\n", __func__, strerror(errno));
    }
    LOGI("%s: SO_DONTROUTE: %d.\n", __func__, optval);

    // 是否发送保活.
    optlen = sizeof(optval);
    err = getsockopt(sock_fd, SOL_SOCKET, SO_KEEPALIVE, &optval, &optlen);
    if (err == -1) {
        LOGW("%s: getsockopt(SO_KEEPALIVE) failed: %s.\n", __func__, strerror(errno));
    }
    LOGI("%s: SO_KEEPALIVE: %d.\n", __func__, optval);

    // 获取当前linger.
    optlen = sizeof(optlinger);
    err = getsockopt(sock_fd, SOL_SOCKET, SO_LINGER, &optlinger, &optlen);
    if (err == -1) {
        LOGW("%s: getsockopt(SO_LINGER) failed: %s.\n", __func__, strerror(errno));
    }
    LOGI("%s: SO_LINGER: l_onoff(%d), l_linger(%d).\n", __func__, optlinger.l_onoff, optlinger.l_linger);

    // 带外数据是否在常规数据中传输.
    optlen = sizeof(optval);
    err = getsockopt(sock_fd, SOL_SOCKET, SO_OOBINLINE, &optval, &optlen);
    if (err == -1) {
        LOGW("%s: getsockopt(SO_OOBINLINE) failed: %s.\n", __func__, strerror(errno));
    }
    LOGI("%s: SO_OOBINLINE: %d.\n", __func__, optval);

    // 设置发送优先级.
    optval = UDPX_SOCKET_PRIORITY;
    err = setsockopt(sock_fd, SOL_SOCKET, SO_PRIORITY, (void *) &optval, optlen);
    if (err == -1) {
        LOGW("%s: setsockopt(SO_PRIORITY) failed: %s.\n", __func__, strerror(errno));
    }

    // 发送优先级.
    optlen = sizeof(optval);
    err = getsockopt(sock_fd, SOL_SOCKET, SO_PRIORITY, &optval, &optlen);
    if (err == -1) {
        LOGW("%s: getsockopt(SO_PRIORITY) failed: %s.\n", __func__, strerror(errno));
    }
    LOGI("%s: SO_PRIORITY: %d.\n", __func__, optval);

#ifdef __ANDROID__
    // 协议类型.
    optlen = sizeof(optval);
    err = getsockopt(sock_fd, SOL_SOCKET, SO_PROTOCOL, &optval, &optlen);
    if (err == -1) {
        LOGW("%s: getsockopt(SO_PROTOCOL) failed: %s.\n", __func__, strerror(errno));
    }
    LOGI("%s: SO_PROTOCOL: %d.\n", __func__, optval);
#endif

    // 设置接收缓冲区大小.
    optval = UDPX_SOCKET_RCVBUF_LENGTH;
    err = setsockopt(sock_fd, SOL_SOCKET, SO_RCVBUF, (void *) &optval, optlen);
    if (err == -1) {
        LOGW("%s: setsockopt(SO_RCVBUF) failed: %s.\n", __func__, strerror(errno));
    }

    // 接收缓冲区大小.
    optlen = sizeof(optval);
    err = getsockopt(sock_fd, SOL_SOCKET, SO_RCVBUF, &optval, &optlen);
    if (err == -1) {
        LOGW("%s: getsockopt(SO_RCVBUF) failed: %s.\n", __func__, strerror(errno));
    }
    LOGI("%s: SO_RCVBUF: %d.\n", __func__, optval);

    // 用户可接收最小字节数.
    optlen = sizeof(optval);
    err = getsockopt(sock_fd, SOL_SOCKET, SO_RCVLOWAT, &optval, &optlen);
    if (err == -1) {
        LOGW("%s: getsockopt(SO_RCVLOWAT) failed: %s.\n", __func__, strerror(errno));
    }
    LOGI("%s: SO_RCVLOWAT: %d.\n", __func__, optval);

    // 实际发送最小字节数.
    optlen = sizeof(optval);
    err = getsockopt(sock_fd, SOL_SOCKET, SO_SNDLOWAT, &optval, &optlen);
    if (err == -1) {
        LOGW("%s: getsockopt(SO_SNDLOWAT) failed: %s.\n", __func__, strerror(errno));
    }
    LOGI("%s: SO_SNDLOWAT: %d.\n", __func__, optval);

    // 接收超时时间. 单位: ms.
    optlen = sizeof(optval);
    err = getsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, &optval, &optlen);
    if (err == -1) {
        LOGW("%s: getsockopt(SO_RCVTIMEO) failed: %s.\n", __func__, strerror(errno));
    }
    LOGI("%s: SO_RCVTIMEO: %d.\n", __func__, optval);

    // 发送超时时间. 单位: ms.
    optlen = sizeof(optval);
    err = getsockopt(sock_fd, SOL_SOCKET, SO_SNDTIMEO, &optval, &optlen);
    if (err == -1) {
        LOGW("%s: getsockopt(SO_SNDTIMEO) failed: %s.\n", __func__, strerror(errno));
    }
    LOGI("%s: SO_SNDTIMEO: %d.\n", __func__, optval);

    // 地址是否复用.
    optlen = sizeof(optval);
    err = getsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval, &optlen);
    if (err == -1) {
        LOGW("%s: getsockopt(SO_REUSEADDR) failed: %s.\n", __func__, strerror(errno));
    }
    LOGI("%s: SO_REUSEADDR: %d.\n", __func__, optval);

    // 设置发送缓冲区大小.
    optval = UDPX_SOCKET_SNDBUF_LENGTH;
    err = setsockopt(sock_fd, SOL_SOCKET, SO_SNDBUF, (void *) &optval, optlen);
    if (err == -1) {
        LOGW("%s: setsockopt(SO_SNDBUF) failed: %s.\n", __func__, strerror(errno));
    }

    // 发送缓冲区大小.
    optlen = sizeof(optval);
    err = getsockopt(sock_fd, SOL_SOCKET, SO_SNDBUF, &optval, &optlen);
    if (err == -1) {
        LOGW("%s: getsockopt(SO_SNDBUF) failed: %s.\n", __func__, strerror(errno));
    }
    LOGI("%s: SO_SNDBUF: %d.\n", __func__, optval);

    // socket类型.
    optlen = sizeof(optval);
    err = getsockopt(sock_fd, SOL_SOCKET, SO_TYPE, &optval, &optlen);
    if (err == -1) {
        LOGW("%s: getsockopt(SO_TYPE) failed: %s.\n", __func__, strerror(errno));
    }
    LOGI("%s: SO_TYPE: %d.\n", __func__, optval);
}

/**************************************************
 * QoS.
 **************************************************/

/**
 * @brief QoS初始化
 *
 * @param qos   待初始化的QoS指针
 */
void udpx_qos_init(udpx_qos *qos) {
    if (qos == NULL) {
        LOGE("%s: qos is null.\n", __func__);
        return;
    }

    //
    pthread_mutex_init(&qos->mutex, NULL);
    qos->timestamp = udpx_get_timestamp();
    qos->statistic_interval = UDPX_QOS_FAST_STATISTIC_INTERVAL;

    // 
    qos->real_send_packets = 0;
    qos->success_send_packets = 0;
    qos->send_packets_loss_rate = 0;

    // 
    qos->round_trip_time_sum = 0;
    qos->round_trip_time = UDPX_ROUND_TRIP_TIME_DEFAULT;

    // 
    qos->send_bytes = 0;
    qos->send_bandwidth = 0;

    // 
    qos->real_send_bytes = 0;
    qos->real_send_bandwidth = 0;

    //
    qos->success_send_bytes = 0;
    qos->success_send_bandwidth = 0;

    // 
    qos->recv_packets = 0;

    // 
    qos->recv_bytes = 0;
    qos->recv_bandwidth = 0;
}

/**
 * @brief QoS拷贝.
 * @param dest  拷贝目标QoS
 * @param src   拷贝源QoS
 */
void udpx_qos_copy(udpx_qos *dest, udpx_qos *src) {
    if (dest == NULL || src == NULL) {
        LOGW("%s: qos is null.\n", __func__);
        return;
    }

    dest->timestamp = src->timestamp;

    dest->real_send_packets = src->real_send_packets;
    dest->success_send_packets = src->success_send_packets;
    dest->send_packets_loss_rate = src->send_packets_loss_rate;

    dest->round_trip_time_sum = src->round_trip_time_sum;
    dest->round_trip_time = src->round_trip_time;

    dest->send_bytes = src->send_bytes;
    dest->send_bandwidth = src->send_bandwidth;

    dest->real_send_bytes = src->real_send_bytes;
    dest->real_send_bandwidth = src->real_send_bandwidth;

    dest->success_send_bytes = src->success_send_bytes;
    dest->success_send_bandwidth = src->success_send_bandwidth;

    dest->recv_packets = src->recv_packets;

    dest->recv_bytes = src->recv_bytes;
    dest->recv_bandwidth = src->recv_bandwidth;
}

/**
 * @brief QoS资源释放
 *
 * @param qos   待释放的QoS指针
 */
void udpx_qos_fini(udpx_qos *qos) {
    if (qos == NULL) {
        LOGW("%s: qos is null.\n", __func__);
        return;
    }

    pthread_mutex_destroy(&qos->mutex);
}

/**************************************************
 * UDPX初始化/释放.
 **************************************************/

/**
 * @brief 初始化通信模块
 *
 * @param server_ip 字符串型服务器IP
 * @param port  服务器端口
 *
 * @return 
 * @retval {@enum SUCCESS}  成功
 * @retval {@enum ENULL_POINTER}    服务器IP为空
 * @retval {@enum EINVALID_PARAMETER}   服务器IP不合法
 * @retval {@enum ESOCKET_FAILED}   socket打开失败
 * @retval {@enum EUNKNOWN} 未知错误. 收发buffer池初始化失败
 */
int udpx_init(const char *server_ip, short port) {
    if (server_ip == NULL) {
        LOGE("%s: server ip is null.\n", __func__);
        return ENULL_POINTER;
    }

    struct in_addr addr;
    if (inet_aton(server_ip, &addr) == 0) {
        LOGE("%s: server ip is invalid.\n", __func__);
        return EINVALID_PARAMETER;
    }

    // 清空UDPX实例.
    memset(&udpx_instance, 0, sizeof(udpx_instance));

    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd == -1) {
        LOGE("%s: socket() failed: %s.\n", __func__, strerror(errno));
        return ESOCKET_FAILED;
    }
    // TODO 设置socket属性.
    udpx_socket_options_init(sock_fd);

    struct sockaddr_in sock_addr;
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(port);
    sock_addr.sin_addr.s_addr = inet_addr(server_ip);

    // 填充UDPX实例.
    udpx_instance.state = UDPX_STOPED;
    udpx_instance.sock_fd = sock_fd;
    udpx_instance.send_timestamp = udpx_get_timestamp_us();
    udpx_instance.session_id = UDPX_REGISTER_SESSION_ID;
    udpx_instance.send_seq_no = udpx_rand();
    udpx_instance.send_msg_no = udpx_rand();
    udpx_instance.recv_seq_no = 0;
    udpx_instance.recv_msg_no = 0;
    udpx_instance.recv_thread_pid = 0;
    udpx_instance.send_policy_thread_pid = 0;
    udpx_instance.qos_statistic_thread_pid = 0;
    udpx_instance.sock_addr = sock_addr;

    udpx_instance.send_buffer_pool = udpx_buffer_init(SEND_SMALL_BUFFER_LENGTH, SEND_SMALL_BUFFER_COUNT, SEND_BIG_BUFFER_LENGTH, SEND_BIG_BUFFER_COUNT);
    if (udpx_instance.send_buffer_pool == NULL) {
        LOGE("%s: send buffer pool init failed.\n", __func__);
        goto ERR;
    }
    udpx_instance.send_packet_list_length = 0;
    INIT_LIST_HEAD(&udpx_instance.send_packet_list);
    pthread_mutex_init(&udpx_instance.send_list_mutex, NULL);

    udpx_instance.recv_buffer_pool = udpx_buffer_init(RECV_SMALL_BUFFER_LENGTH, RECV_SMALL_BUFFER_COUNT, RECV_BIG_BUFFER_LENGTH, RECV_BIG_BUFFER_COUNT);
    if (udpx_instance.recv_buffer_pool == NULL) {
        LOGE("%s: recv buffer pool init failed.\n", __func__);
        goto ERR;
    }
    udpx_instance.recv_message_list_length = 0;
    INIT_LIST_HEAD(&udpx_instance.recv_message_list);
    pthread_mutex_init(&udpx_instance.recv_list_mutex, NULL);

    udpx_instance.ack_buffer = udpx_int_ring_buffer_init(UDPX_ACK_BUFFER_LENGTH);
    if (udpx_instance.ack_buffer == NULL) {
        LOGW("%s: ack buffer init failed.\n", __func__);
    }

    pthread_cond_init(&udpx_instance.ack_cond, NULL);
    pthread_mutex_init(&udpx_instance.ack_cond_mutex, NULL);

    udpx_qos_init(&udpx_instance.qos);
    udpx_qos_init(&udpx_instance.qos_fast);
    udpx_instance.qos_fast.statistic_interval = UDPX_QOS_FAST_STATISTIC_INTERVAL;
    udpx_qos_init(&udpx_instance.qos_slow);
    udpx_instance.qos_slow.statistic_interval = UDPX_QOS_SLOW_STATISTIC_INTERVAL;

    return SUCCESS;

ERR:
    udpx_fini();
    return EUNKNOWN;
}

/**
 * @brief 释放通信模块
 *
 * @return
 * @retval {@enum SUCCESS}  成功
 */
int udpx_fini() {
    if (udpx_instance.state == UDPX_RUNNING) {
        udpx_stop();
    }

    int sock_fd = udpx_instance.sock_fd;
    if (sock_fd != -1) {
        LOGI("%s: close socket(%d).\n", __func__, sock_fd);
        close(sock_fd);
        udpx_instance.sock_fd = -1;
    } 

    if (udpx_instance.send_buffer_pool != NULL) {
        udpx_buffer_fini(udpx_instance.send_buffer_pool);
        udpx_instance.send_buffer_pool = NULL;
    }
    if (udpx_instance.recv_buffer_pool != NULL) {
        udpx_buffer_fini(udpx_instance.recv_buffer_pool);
        udpx_instance.recv_buffer_pool = NULL;
    }

    if (udpx_instance.ack_buffer != NULL) {
        udpx_int_ring_buffer_fini(udpx_instance.ack_buffer);
        udpx_instance.ack_buffer = NULL;
    }

    pthread_mutex_destroy(&udpx_instance.send_list_mutex);
    pthread_mutex_destroy(&udpx_instance.recv_list_mutex);

    pthread_cond_destroy(&udpx_instance.ack_cond);
    pthread_mutex_destroy(&udpx_instance.ack_cond_mutex);

    udpx_qos_fini(&udpx_instance.qos);
    udpx_qos_fini(&udpx_instance.qos_fast);
    udpx_qos_fini(&udpx_instance.qos_slow);

    memset(&udpx_instance, 0, sizeof(udpx_instance));

    return SUCCESS;
}

/**************************************************
 * UDPX发送/接收缓存链表.
 **************************************************/

/**
 * @brief 将收到的报文添加到接收消息链表尾部
 *
 * @param buffer    报文buffer
 */
void udpx_recv_message_list_add(udpx_buffer *buffer) {
    if (buffer == NULL) {
        return;
    }
    // 加锁
    pthread_mutex_lock(&udpx_instance.recv_list_mutex);
    list_add_tail(&buffer->available_node, &udpx_instance.recv_message_list);
    udpx_instance.recv_message_list_length++;
    // 解锁
    pthread_mutex_unlock(&udpx_instance.recv_list_mutex);

    // QoS
    // 解锁
    pthread_mutex_lock(&udpx_instance.qos.mutex);
    udpx_instance.qos.recv_packets++;
    udpx_instance.qos.recv_bytes += buffer->payload_length + sizeof(udpx_header);
    // 解锁
    pthread_mutex_unlock(&udpx_instance.qos.mutex);

    // 唤醒阻塞线程{@func udpx_get_message_timeout()}.
    // 加锁
    pthread_mutex_lock(&udpx_get_message_cond_mutex);
    pthread_cond_broadcast(&udpx_get_message_cond);
    // 解锁
    pthread_mutex_unlock(&udpx_get_message_cond_mutex);
}

/**
 * @brief 从接收消息链表中取出头部报文
 *
 * @return 报文buffer
 */
udpx_buffer *udpx_recv_message_list_del() {
    udpx_buffer *pBuffer = NULL;
    // 加锁
    pthread_mutex_lock(&udpx_instance.recv_list_mutex);
    if (!list_empty(&udpx_instance.recv_message_list)) {
        pBuffer = list_first_entry(&udpx_instance.recv_message_list, udpx_buffer, available_node);
        if (pBuffer != NULL) {
            list_del_init(&pBuffer->available_node);
            udpx_instance.recv_message_list_length--;
        }
    }
    // 解锁
    pthread_mutex_unlock(&udpx_instance.recv_list_mutex);
    return pBuffer;
}

/**
 * @brief 清空接收消息链表
 */
void udpx_recv_message_list_clear() {
    udpx_buffer *pBuffer = NULL;
    // 加锁
    pthread_mutex_lock(&udpx_instance.recv_list_mutex);
    while (!list_empty(&udpx_instance.recv_message_list)) {
        pBuffer = list_first_entry(&udpx_instance.recv_message_list, udpx_buffer, available_node);
        if (pBuffer != NULL) {
            list_del_init(&pBuffer->available_node);
            udpx_instance.recv_message_list_length--;
            udpx_free_message(pBuffer);
        }
    }
    // 解锁
    pthread_mutex_unlock(&udpx_instance.recv_list_mutex);
}

/**
 * @brief 将发送的报文添加到发送报文链表尾部
 *
 * @param buffer    报文buffer
 */
void udpx_send_packet_list_add(udpx_buffer *buffer) {
    if (buffer == NULL) {
        return;
    }
    // 加锁
    pthread_mutex_lock(&udpx_instance.send_list_mutex);
    list_add_tail(&buffer->available_node, &udpx_instance.send_packet_list);
    udpx_instance.send_packet_list_length++;
    // 解锁
    pthread_mutex_unlock(&udpx_instance.send_list_mutex);
}

/**
 * @brief 从发送报文链表中取出头部报文
 *
 * @return 报文buffer
 */
udpx_buffer *udpx_send_packet_list_del() {
    udpx_buffer *pBuffer = NULL;
    // 加锁
    pthread_mutex_lock(&udpx_instance.send_list_mutex);
    if (!list_empty(&udpx_instance.send_packet_list)) {
        pBuffer = list_first_entry(&udpx_instance.send_packet_list, udpx_buffer, available_node);
        if (pBuffer != NULL) {
            list_del_init(&pBuffer->available_node);
            udpx_instance.send_packet_list_length--;
        }
    }
    // 解锁
    pthread_mutex_unlock(&udpx_instance.send_list_mutex);
    return pBuffer;
}

/**
 * @brief 清空发送报文链表
 */
void udpx_send_packet_list_clear() {
    udpx_buffer *pBuffer = NULL;
    // 加锁
    pthread_mutex_lock(&udpx_instance.send_list_mutex);
    while (!list_empty(&udpx_instance.send_packet_list)) {
        pBuffer = list_first_entry(&udpx_instance.send_packet_list, udpx_buffer, available_node);
        if (pBuffer != NULL) {
            list_del_init(&pBuffer->available_node);
            udpx_instance.send_packet_list_length--;
            udpx_free_buffer(pBuffer);
        }
    }
    // 解锁
    pthread_mutex_unlock(&udpx_instance.send_list_mutex);
}

/**************************************************
 * UDPX SOCKET接收消息处理.
 **************************************************/

/**
 * @brief 注册报文处理
 *
 * @param buffer    报文buffer
 * @param length    报文长度
 */
void udpx_msg_process_register(unsigned char *buffer, int length) {
    int header_length = sizeof(udpx_header);
    udpx_header *header = (udpx_header *) buffer;
    // 保存session_id.
    udpx_msg_header *data = (udpx_msg_header *) header->content;
    if (udpx_instance.session_id == UDPX_REGISTER_SESSION_ID) {
        // 首次收到注册报文
        udpx_instance.session_id = data->session_id;

        // 注意: 每次接收到新会话, 需重置接收序号.
        udpx_instance.recv_seq_no = 0;
        udpx_instance.recv_msg_no = 0;

        LOGI("%s: new session id is 0x%x, message length is %d\n", __func__, udpx_instance.session_id, length);
    } else {
        // 会话ID是否有效, 需要业务层解析.
        LOGI("%s: temp session id is 0x%x, message length is %d\n", __func__, data->session_id, length);
    }

    // 放入接收消息链表.
    udpx_buffer *pBuffer = udpx_buffer_get(udpx_instance.recv_buffer_pool, length);
    if (pBuffer != NULL) {
        pBuffer->offset = header_length;
        memcpy(pBuffer->content + pBuffer->offset, header->content, length - header_length);
        pBuffer->payload_length = length - header_length;
        udpx_recv_message_list_add(pBuffer);
    } else {
        LOGW("%s: get recv buffer failed.\n", __func__);
        return;
    }
}

/**
 * @brief ACK报文处理
 *
 * @param buffer    报文buffer
 * @param length    报文长度
 */
void udpx_msg_process_ack(unsigned char *buffer, int length) {
    udpx_header *header = (udpx_header *) buffer;
    udpx_ack *ack = (udpx_ack *) header->content;
    int seq_no = ntohl(ack->seq_no) & UDPX_SEQ_NO_MASK;
    LOGD("%s: seq_no is 0x%x.\n", __func__, seq_no);

    // 放入ACK缓存.
    udpx_int_ring_buffer_put(udpx_instance.ack_buffer, seq_no);

    // 唤醒阻塞线程{@func udpx_packet_send_policy_thread()}.
    // 加锁
    pthread_mutex_lock(&udpx_instance.ack_cond_mutex);
    pthread_cond_broadcast(&udpx_instance.ack_cond);
    // 解锁
    pthread_mutex_unlock(&udpx_instance.ack_cond_mutex);
}

/**
 * @brief 数据报文处理
 *
 * @param buffer    报文buffer
 * @param length    报文长度
 */
void udpx_msg_process_data(unsigned char *buffer, int length) {
    int header_length = sizeof(udpx_header);
    unsigned char content[128];
    udpx_header *header = (udpx_header *) buffer;
    int seq_no = ntohl(header->data_header.seq_no) & UDPX_SEQ_NO_MASK;
    int msg_no = ntohl(header->data_header.msg_no) & UDPX_MSG_NO_MASK;

    // 重复消息判断.
    // TODO
    if (seq_no <= udpx_instance.recv_seq_no) {
        LOGV("%s: duplicate message(0x%x).\n", __func__, seq_no);
        return;
    }
    udpx_instance.recv_seq_no = seq_no;
    udpx_instance.recv_msg_no = msg_no;

    memset(content, 0, sizeof(content));

    // 构造ACK.
    header = (udpx_header *) content;
    udpx_header_init_ack(header, udpx_instance.session_id);

    udpx_ack *ack = (udpx_ack *) header->content;
    ack->seq_no = htonl(seq_no);
    ack->from_ip = 0;
    ack->from_port = 0;

    // 发送ACK.
    int send_length = sizeof(udpx_header) + sizeof(udpx_ack);
    udpx_socket_sendto(header, send_length, 0);

    LOGD("%s: message(0x%x) length is %d.\n", __func__, seq_no, length);
    // 放入接收消息链表.
    header = (udpx_header *) buffer;
    udpx_buffer *pBuffer = udpx_buffer_get(udpx_instance.recv_buffer_pool, length);
    if (pBuffer != NULL) {
        pBuffer->offset = header_length;
        memcpy(pBuffer->content + pBuffer->offset, header->content, length - header_length);
        pBuffer->payload_length = length - header_length;
        udpx_recv_message_list_add(pBuffer);
        // TODO 组包. 仅用于设备端, 不会收到大包数据包.
    } else {
        LOGW("%s: get recv buffer failed.\n", __func__);
        return;
    }
}

/**
 * @brief 协议消息合法性检查
 *
 * @param buffer    报文buffer
 * @param length    报文长度
 */
void udpx_msg_check_func(unsigned char *buffer, int length) {
    if (length < UDPX_MSG_MIN_LENGTH) {
        LOGI("%s: message is too short.\n", __func__);
        return;
    }
    if (length > UDPX_MSG_MAX_LENGTH) {
        LOGI("%s: message is too long.\n", __func__);
        return;
    }

    udpx_header *header = (udpx_header *) buffer;
    unsigned short magic = ntohs(header->magic);
    if (magic != UDPX_MAGIC) {
        LOGI("%s: magic(0x%x) is wrong.\n", __func__, magic);
        return;
    }
    unsigned char version = header->version;
    if (version != UDPX_VERSION) {
        LOGI("%s: version(%d) is incompatible.\n", __func__, version);
        return;
    }

    unsigned int type = ntohl(*(header->type));
    if (type == UDPX_MSG_CATEGORY_REGISTER) {
        // 注册类报文
        udpx_msg_process_register(buffer, length);
    } else  if (type == UDPX_MSG_CATEGORY_ACK) {
        // ACK类报文
        int session_id = ntohl(header->control_header.session_id);
        if (session_id != udpx_instance.session_id) {
            LOGI("%s: ack: session id(0x%x) is not equal 0x%x.\n", __func__, session_id, udpx_instance.session_id);
            return;
        }
        udpx_msg_process_ack(buffer, length);
    } else if ((type >> 31) == 0) {
        // 数据类报文
        int session_id = ntohl(header->data_header.session_id);
        if (session_id != udpx_instance.session_id) {
            LOGI("%s: data: session id(0x%x) is not equal 0x%x.\n", __func__, session_id, udpx_instance.session_id);
            return;
        }
        udpx_msg_process_data(buffer, length);
    } else {
        LOGI("%s: unknown message(0x%08X).\n", __func__, type);
    }
}

/**
 * @brief socket消息接收
 *
 * @param buffer    接收buffer
 * @param length    接收buffer长度
 */
void udpx_msg_recv_func(unsigned char *buffer, int length) {
    socklen_t addrlen = sizeof(struct sockaddr_in);
    ssize_t recv_length = recvfrom(udpx_instance.sock_fd, (void *) buffer, length, 0, (struct sockaddr *) &udpx_instance.sock_addr, &addrlen);
    if (recv_length < 0) {
        LOGW("%s: recvfrom() failed: %s.\n", __func__, strerror(errno));
    } else if (recv_length == 0) {
        // orderly shutdown
        LOGW("%s: the peer has performed an orderly shutdown.\n", __func__);
    } else {
        // LOGV("%s: recv length is %d.\n", __func__, (int) recv_length);
        udpx_msg_check_func(buffer, recv_length);
    }
}

/**
 * @brief socket接收处理线程
 *
 * @param param 未使用
 * 
 * @return 空
 */
void *udpx_msg_recv_thread(void *param) {
    int ready;
    struct pollfd fds[1];
    unsigned char buffer[UDPX_BUFFER_MAX_LENGTH];

    LOGI("%s: pid is %d.\n", __func__, gettid());

    fds[0].fd = udpx_instance.sock_fd;
    fds[0].events = POLLIN;
    
    while (udpx_instance.state == UDPX_RUNNING) {
        ready = poll(fds, sizeof(fds) / sizeof(struct pollfd), UDPX_POLL_TIMEOUT);
        if (ready < 0) {
            LOGW("%s: poll() failed: %s.\n", __func__, strerror(errno));
            continue;
        } else if (ready == 0) {
            // timeout.
            continue;
        } else {
            if (fds[0].revents & POLLIN) {
                // 
                udpx_msg_recv_func(buffer, sizeof(buffer));
            }
        }
    }
    return NULL;
}

/**************************************************
 * UDPX丢包重传策略.
 **************************************************/

/**
 * @brief 数据包重传策略.
 *
 * @param seq_no    ack消息对应的数据包发送序号
 */
void udpx_packet_send_policy_retry_send(int seq_no) {
    /**
     * 每收到一个ack, 最多重发一包数据.
     */
    int retry_send_flag = 1;

    udpx_header *header = NULL;
    udpx_buffer *pBuffer = NULL;
    struct list_head *pNode = NULL;
    struct list_head *pTempNode = NULL;

    int payload_length = 0;
    unsigned long current_timestamp = udpx_get_timestamp();

    udpx_qos *qos = &udpx_instance.qos_fast;
    int loss_rate = qos->send_packets_loss_rate;
    int round_trip_time = qos->round_trip_time;

    int packet_retry_send_interval = round_trip_time * UDPX_PACKET_RETRY_SEND_INTERVAL_FACTOR;
    if (loss_rate > 0 && loss_rate <= 100) {
        packet_retry_send_interval = round_trip_time * (UDPX_PACKET_RETRY_SEND_INTERVAL_FACTOR * 2 + loss_rate / 5) / 2;
    }
    if (packet_retry_send_interval > UDPX_PACKET_RETRY_SEND_INTERVAL_MAX) {
        packet_retry_send_interval = UDPX_PACKET_RETRY_SEND_INTERVAL_MAX;
    }

    /**
     * {@var send_packet_list}链表中的发送包的{@var seq_no}是自增的.
     * 在找到{@param seq_no}对应的数据包之前的包均可认为是发送失败, 需要重传.
     */
    // 加锁
    pthread_mutex_lock(&udpx_instance.send_list_mutex);
    list_for_each_safe(pNode, pTempNode, &udpx_instance.send_packet_list) {
        pBuffer = list_entry(pNode, udpx_buffer, available_node);
        if (pBuffer != NULL) {
            header = (udpx_header *) (pBuffer->content + pBuffer->offset);
            int no = ntohl(header->data_header.seq_no) & UDPX_SEQ_NO_MASK;
            if (no == seq_no) {
                payload_length = pBuffer->payload_length;
                // 找到ack对应的数据包
                list_del_init(&pBuffer->available_node);
                udpx_instance.send_packet_list_length--;
                udpx_free_buffer(pBuffer);

                // QoS
                // 加锁
                pthread_mutex_lock(&udpx_instance.qos.mutex);
                udpx_instance.qos.success_send_packets++;
                udpx_instance.qos.round_trip_time_sum += current_timestamp - pBuffer->timestamp;
                udpx_instance.qos.success_send_bytes += payload_length;
                // 解锁
                pthread_mutex_unlock(&udpx_instance.qos.mutex);
                break;
            }

            if (pBuffer->deadline < current_timestamp) {
                // 数据包已无效.
                list_del_init(&pBuffer->available_node);
                udpx_instance.send_packet_list_length--;
                udpx_free_buffer(pBuffer);
                LOGI("%s: timeout, seq_no is 0x%x.\n", __func__, no);
                continue;
            }

            if (retry_send_flag == 1 && current_timestamp - pBuffer->timestamp > packet_retry_send_interval) {
                // 数据包重发
                int ret = udpx_socket_sendto(header, pBuffer->payload_length, 0);
                if (ret == -1) {
                    LOGW("%s: retry send failed, seq_no is 0x%x.\n", __func__, no);
                } else {
                    LOGI("%s: retry send, interval is %dms, seq_no is 0x%x, lag: %lums.\n", __func__, packet_retry_send_interval, no, (current_timestamp + UDPX_PACKET_LIFE_TIME - pBuffer->deadline));

                    // 更新数据包发送时间戳.
                    pBuffer->timestamp = udpx_get_timestamp();

                    // QoS
                    // 加锁
                    pthread_mutex_lock(&udpx_instance.qos.mutex);
                    udpx_instance.qos.real_send_packets++;
                    udpx_instance.qos.real_send_bytes += ret;
                    // 解锁
                    pthread_mutex_unlock(&udpx_instance.qos.mutex);

                    /**
                     * 每收到一个ack消息, 最多重发一个数据包.
                     */
                    retry_send_flag = 0;
                }
            }
        }
    }
    // 解锁
    pthread_mutex_unlock(&udpx_instance.send_list_mutex);
}

/**
 * @brief 清除发送报文链表中的超时无效数据包.
 */
void udpx_packet_send_policy_list_clear() {
    udpx_header *header = NULL;
    udpx_buffer *pBuffer = NULL;
    struct list_head *pNode = NULL;
    struct list_head *pTempNode = NULL;

    unsigned long current_timestamp = udpx_get_timestamp();

    /**
     * {@var send_packet_list}链表中的发送包的{@var seq_no}是自增的.
     * 在找到{@param seq_no}对应的数据包之前的包均可认为是发送失败, 需要重传.
     */
    // 加锁
    pthread_mutex_lock(&udpx_instance.send_list_mutex);
    list_for_each_safe(pNode, pTempNode, &udpx_instance.send_packet_list) {
        pBuffer = list_entry(pNode, udpx_buffer, available_node);
        if (pBuffer != NULL) {
            if (pBuffer->deadline < current_timestamp) {
                header = (udpx_header *) (pBuffer->content + pBuffer->offset);
                int no = ntohl(header->data_header.seq_no) & UDPX_SEQ_NO_MASK;
                // 数据包已无效.
                list_del_init(&pBuffer->available_node);
                udpx_instance.send_packet_list_length--;
                udpx_free_buffer(pBuffer);
                LOGI("%s: timeout, seq_no is 0x%x.\n", __func__, no);
            }
        }
    }
    // 解锁
    pthread_mutex_unlock(&udpx_instance.send_list_mutex);
}

/**
 * @brief socket发送策略线程
 *
 * @param param 未使用
 *
 * @return 空
 */
void *udpx_packet_send_policy_thread(void *param) {
    int seq_no = 0;
    int timeout = 1000; // ms.
    struct timespec out_time;

    LOGI("%s: pid is %d.\n", __func__, gettid());

    while (udpx_instance.state == UDPX_RUNNING) {
        if (!udpx_int_ring_buffer_is_empty(udpx_instance.ack_buffer)) {
            seq_no = udpx_int_ring_buffer_get(udpx_instance.ack_buffer);
            udpx_packet_send_policy_retry_send(seq_no);
        } else {
            // 阻塞线程.
            // 加锁
            pthread_mutex_lock(&udpx_instance.ack_cond_mutex);
            if (udpx_set_timeout(&out_time, timeout) == -1) {
                LOGW("%s: udpx_set_timeout() failed.\n", __func__);
                usleep(timeout * 1000 / 2);
                goto UNLOCK;
            }

            int ret = pthread_cond_timedwait(&udpx_instance.ack_cond, &udpx_instance.ack_cond_mutex, &out_time);
UNLOCK:
            // 解锁
            pthread_mutex_unlock(&udpx_instance.ack_cond_mutex);

            if (ret == ETIMEDOUT) {
                // 清理无效消息.
                udpx_packet_send_policy_list_clear();
                LOGI("%s: clear invalid send packet.\n", __func__);
            }
        }
    }
    return NULL;
}

/**************************************************
 * QoS统计.
 **************************************************/

#define UDPX_QOS_STATISTIC_TYPE_FAST (1)
#define UDPX_QOS_STATISTIC_TYPE_SLOW (2)

/**
 * @brief QoS统计
 *
 * @param type  统计类型.
 *      {@val UDPX_QOS_STATISTIC_TYPE_FAST}
 *      {@val UDPX_QOS_STATISTIC_TYPE_SLOW}
 */
void udpx_qos_statistic(int type) {
    unsigned long timestamp = udpx_get_timestamp();
    udpx_qos *qos = &udpx_instance.qos;
    // 默认为快速统计.
    udpx_qos *qos_backup = &udpx_instance.qos_fast;
    char *tag = "Fast Statistic";

    if (type == UDPX_QOS_STATISTIC_TYPE_SLOW) {
        // 慢速统计.
        qos_backup = &udpx_instance.qos_slow;
        tag = "Slow Statistic";
    }

    LOGI("%s: ===== ===== %s ===== =====\n", __func__, tag);

    // 加锁
    pthread_mutex_lock(&qos->mutex);

    int timestamp_diff = (int) (timestamp - qos_backup->timestamp);
    LOGI("%s: timestamp(%d).\n", __func__, timestamp_diff);

    // send_packets_loss_rate
    int success_send_packets_diff = qos->success_send_packets - qos_backup->success_send_packets;
    int real_send_packets_diff = qos->real_send_packets - qos_backup->real_send_packets;
    if (real_send_packets_diff >= UDPX_QOS_SEND_PACKETS_DIFF_MIN) {
        qos->send_packets_loss_rate = 100 - 100 * success_send_packets_diff / real_send_packets_diff;
    } else {
        qos->send_packets_loss_rate = 0;
    }
    LOGI("%s: real_send_packets(%d), success_send_packets(%d).\n", __func__, real_send_packets_diff, success_send_packets_diff);

    // round_trip_time
    int round_trip_time_sum_diff = (int) (qos->round_trip_time_sum - qos_backup->round_trip_time_sum);
    if (success_send_packets_diff > 0) {
        qos->round_trip_time = round_trip_time_sum_diff / success_send_packets_diff;
    }
    /**
     * RTT小于1ms时, {@var round_trip_time}取值等于0, 不利于策略应用.
     * 这里做一个修正, 取最小值为1ms.
     */
    if (qos->round_trip_time <= 0) {
        qos->round_trip_time = 1;
    }
    LOGI("%s: round_trip_time_sum(%d).\n", __func__, round_trip_time_sum_diff);

    // send_bandwidth
    long send_bytes_diff = qos->send_bytes - qos_backup->send_bytes;
    qos->send_bandwidth = (int) (send_bytes_diff * 1000 / timestamp_diff);
    LOGI("%s: send_bytes(%lu).\n", __func__, send_bytes_diff);

    // real_send_bandwidth
    long real_send_bytes_diff = qos->real_send_bytes - qos_backup->real_send_bytes;
    qos->real_send_bandwidth = (int) (real_send_bytes_diff * 1000 / timestamp_diff);
    LOGI("%s: real_send_bytes(%lu).\n", __func__, real_send_bytes_diff);

    // success_send_bandwidth
    long success_send_bytes_diff = qos->success_send_bytes - qos_backup->success_send_bytes;
    qos->success_send_bandwidth = (int) (success_send_bytes_diff * 1000 / timestamp_diff);
    LOGI("%s: success_send_bytes(%lu).\n", __func__, success_send_bytes_diff);

    // recv_bandwidth
    long recv_bytes_diff = qos->recv_bytes - qos_backup->recv_bytes;
    qos->recv_bandwidth = (int) (recv_bytes_diff * 1000 / timestamp_diff);
    LOGI("%s: recv_bytes(%ld).\n", __func__, recv_bytes_diff);

    // 备份QoS.
    udpx_qos_copy(qos_backup, qos);
    qos_backup->timestamp = timestamp;

    // 解锁
    pthread_mutex_unlock(&qos->mutex);

    LOGI("%s: send_packets_loss_rate(%d %%), round_trip_time(%d ms), send_bandwidth(%d Bps), real_send_bandwidth(%d Bps), success_send_bandwidth(%d Bps), recv_bandwidth(%d Bps).\n", 
            __func__, qos_backup->send_packets_loss_rate, qos_backup->round_trip_time, qos_backup->send_bandwidth, qos_backup->real_send_bandwidth, qos_backup->success_send_bandwidth, qos_backup->recv_bandwidth);
}

/**
 * @brief QoS统计线程.
 */
void *udpx_qos_statistic_thread(void *param) {
    long timestamp = 0;
    udpx_qos *qos = NULL;
    
    LOGI("%s: pid is %d.\n", __func__, gettid());

    while (udpx_instance.state == UDPX_RUNNING) {
        usleep(UDPX_QOS_FAST_STATISTIC_INTERVAL * 1000);

        timestamp = udpx_get_timestamp();

        // 快速QoS统计.
        qos = &udpx_instance.qos_fast;
        if (timestamp - qos->timestamp >= qos->statistic_interval) {
            udpx_qos_statistic(UDPX_QOS_STATISTIC_TYPE_FAST);
        }
        // 慢速QoS统计.
        qos = &udpx_instance.qos_slow;
        if (timestamp - qos->timestamp >= qos->statistic_interval) {
            udpx_qos_statistic(UDPX_QOS_STATISTIC_TYPE_SLOW);
        }
    }

    return NULL;
}

/**************************************************
 * UDPX启动/停止.
 **************************************************/

/**
 * @brief 启动udpx
 *
 * @return
 * @retval {@enum SUCCESS}  成功
 * @retval {@enum EALREADY_STARTED} udpx已经在运行
 * @retval {@enum EUNKNOWN} 未知错误
 */
int udpx_start() {
    if (udpx_instance.state == UDPX_RUNNING) {
        LOGE("%s: udpx already started.\n", __func__);
        return EALREADY_STARTED;
    }

    int err = 0;
    udpx_instance.state = UDPX_RUNNING;
    err = pthread_create(&udpx_instance.recv_thread_pid, NULL, &udpx_msg_recv_thread, NULL);
    if (err != 0) {
        udpx_instance.recv_thread_pid = 0;
        LOGE("%s: recv thread: pthread_create() failed: %s.\n", __func__, strerror(err));
        goto ERR;
    }

    err = pthread_create(&udpx_instance.send_policy_thread_pid, NULL, &udpx_packet_send_policy_thread, NULL);
    if (err != 0) {
        udpx_instance.send_policy_thread_pid = 0;
        LOGE("%s: send policy thread: pthread_create() failed: %s.\n", __func__, strerror(err));
        goto ERR;
    }

    err = pthread_create(&udpx_instance.qos_statistic_thread_pid, NULL, &udpx_qos_statistic_thread, NULL);
    if (err != 0) {
        udpx_instance.qos_statistic_thread_pid = 0;
        LOGE("%s: qos statistic thread: pthread_create() failed: %s.\n", __func__, strerror(err));
        // goto ERR;
    }

    return SUCCESS;

ERR:
    udpx_stop();
    return EUNKNOWN;
}

/**
 * @brief 停止udpx
 */
void udpx_stop() {
    if (udpx_instance.state == UDPX_STOPED) {
        LOGI("%s: udpx already stoped.\n", __func__);
        return;
    }

    int err = 0;
    udpx_instance.state = UDPX_STOPED;

    // 唤醒阻塞线程{@func udpx_get_message_timeout()}.
    // 加锁
    pthread_mutex_lock(&udpx_get_message_cond_mutex);
    pthread_cond_broadcast(&udpx_get_message_cond);
    // 解锁
    pthread_mutex_unlock(&udpx_get_message_cond_mutex);

    // 唤醒阻塞线程{@func udpx_packet_send_policy_thread()}.
    // 加锁
    pthread_mutex_lock(&udpx_instance.ack_cond_mutex);
    pthread_cond_broadcast(&udpx_instance.ack_cond);
    // 解锁
    pthread_mutex_unlock(&udpx_instance.ack_cond_mutex);

    err = pthread_join(udpx_instance.recv_thread_pid, NULL);
    if (err != 0) {
        LOGW("%s: recv thread: pthread_join() failed: %s.\n", __func__, strerror(err));
    }
    udpx_instance.recv_thread_pid = 0;

    err = pthread_join(udpx_instance.send_policy_thread_pid, NULL);
    if (err != 0) {
        LOGW("%s: send policy thread: pthread_join() failed: %s.\n", __func__, strerror(err));
    }
    udpx_instance.send_policy_thread_pid = 0;

    err = pthread_join(udpx_instance.qos_statistic_thread_pid, NULL);
    if (err != 0) {
        LOGW("%s: qos statistic thread: pthread_join() failed: %s.\n", __func__, strerror(err));
    }
    udpx_instance.qos_statistic_thread_pid = 0;
}

/**************************************************
 * UDPX杂项.
 **************************************************/

/**
 * @brief 业务层更新会话ID
 *
 * @param session_id    会话ID
 */
void udpx_set_session_id(int session_id) {
    udpx_instance.session_id = session_id;

    // 注意: 每次接收到新会话, 需重置接收序号.
    udpx_instance.recv_seq_no = 0;
    udpx_instance.recv_msg_no = 0;

    LOGI("%s: new session id is 0x%x.\n", __func__, session_id);
}

/**************************************************
 * UDPX发送.
 **************************************************/

/**
 * 发送消息类型.
 */
#define UDPX_SEND_FLAG_REGISTER (1)
#define UDPX_SEND_FLAG_DATA (2)

/**
 * 一个消息允许发送失败最大次数.
 */
#define UDPX_SEND_FAILED_TIME_MAX (3)

/**
 * @brief socket拆包发送
 *
 * @param buffer    发送消息buffer
 * @param flag  消息类型
 *      {@val UDPX_SEND_FLAG_REGISTER}
 *      {@val UDPX_SEND_FLAG_DATA}
 *
 * @return 非负值表示已发送的数据长度
 * @retval {@enum EINVALID_STATE}   udpx未运行
 * @retval {@enum ENULL_POINTER}    发送buffer为空
 * @retval {@enum EWRONG_BUFFER}    发送buffer不是通过{@func udpx_get_buffer()}获取的
 * @retval {@enum EINVALID_PARAMETER}   发送buffer的offset太小
 * @retval {@enum ESOCKET_SEND_FAILED}  socket发送失败次数超过上限
 */
int udpx_send(udpx_buffer *buffer, int flag) {
    int send_total_length = 0;
    int length = buffer->payload_length;
    int header_length = sizeof(udpx_header);

    if (udpx_instance.state != UDPX_RUNNING) {
        LOGE("%s: udpx state is invalid.\n", __func__);
        return EINVALID_STATE;
    }
    if (buffer == NULL) {
        LOGE("%s: buffer is null.\n", __func__);
        return ENULL_POINTER;
    }
    if (buffer->pool != udpx_instance.send_buffer_pool) {
        LOGE("%s: invalid buffer.\n", __func__);
        return EWRONG_BUFFER;
    }
    if (buffer->offset < header_length) {
        LOGE("%s: offset is too short.\n", __func__);
        return EINVALID_PARAMETER;
    }

    udpx_header *header = NULL;
    unsigned char *pData = buffer->content + buffer->offset;

    int failed_time = 0;
    int start_bit = 1;
    int stop_bit = 0;
    int send_length = UDPX_PAYLOAD_MAX_LENGTH;

    int seq_no = 0;
    int ret = 0;

    // 
    struct sockaddr *sock_addr = (struct sockaddr *) &udpx_instance.sock_addr;
    socklen_t addrlen = sizeof(udpx_instance.sock_addr);
    unsigned long long timestamp = 0;
    int timestamp_diff = 0;

    // 加锁. 开始发送.
    pthread_mutex_lock(&udpx_send_api_mutex);

    udpx_qos *qos = &udpx_instance.qos_fast;
    int loss_rate = qos->send_packets_loss_rate;

    int socket_send_interval = UDPX_SOCKET_SEND_INTERVAL_MIN;
    if (loss_rate > 0 && loss_rate <= 100) {
        socket_send_interval = UDPX_SOCKET_SEND_INTERVAL_MIN * (100 + loss_rate) / 100;
    }

    int fake = 0;
    int fake_rate = 0;

    while (length > 0) {
        if (length <= UDPX_PAYLOAD_MAX_LENGTH) {
            send_length = length;
            stop_bit = 1;
        }

        header = (udpx_header *) (pData - header_length);
        // socket发送加锁
        // 加锁
        pthread_mutex_lock(&udpx_socket_send_mutex);
        if (flag == UDPX_SEND_FLAG_REGISTER) {
            udpx_header_init_register(header);
            seq_no = 0;
        } else {
            udpx_header_init_data(header, udpx_instance.send_seq_no, udpx_instance.send_msg_no, udpx_instance.session_id);
            udpx_header_data_set_control_bit(header, start_bit, stop_bit);
            seq_no = udpx_instance.send_seq_no;
        }

        // Debug.
        // udpx_header_dump(header);

        // 随机伪发送
        fake = 0;
        if (loss_rate > UDPX_FAKE_SEND_LOSS_RATE_MIN) {
            fake_rate = loss_rate;
            if (udpx_rand_range(100) <= UDPX_FAKE_SEND_FAKE_RATE(fake_rate)) {
                fake = 1;
            }
        }

        // socket发送
        timestamp = udpx_get_timestamp_us();
        timestamp_diff = (int) (timestamp - udpx_instance.send_timestamp);
        if (timestamp_diff <= socket_send_interval) {
            usleep(socket_send_interval - timestamp_diff);
        }

        if (fake == 0) {
            ret = sendto(udpx_instance.sock_fd, header, header_length + send_length, 0, sock_addr, addrlen);
        } else {
            // 伪发送.
            ret = header_length + send_length;
        }
        if (ret == -1) {
            LOGW("%s: sendto() failed: %s.\n", __func__, strerror(errno));

            udpx_instance.send_timestamp = udpx_get_timestamp_us();
            // 发送错误分支, 解锁
            pthread_mutex_unlock(&udpx_socket_send_mutex);

            // 一个消息发送失败次数超过上限, 直接退出发送.
            if (++failed_time >= UDPX_SEND_FAILED_TIME_MAX) {
                send_total_length = ESOCKET_SEND_FAILED;
                break;
            } else {
                continue;
            }
        } else {
            udpx_instance.send_timestamp = udpx_get_timestamp_us();
            udpx_instance.send_seq_no = (udpx_instance.send_seq_no + 1) & UDPX_SEQ_NO_MASK;
            // 发送成功分支, 解锁
            pthread_mutex_unlock(&udpx_socket_send_mutex);

            // 注册报文不做QoS统计.
            if (flag != UDPX_SEND_FLAG_REGISTER) {
                // QoS
                // 加锁
                pthread_mutex_lock(&udpx_instance.qos.mutex);
                udpx_instance.qos.send_bytes += ret;
                if (fake == 0) {
                    udpx_instance.qos.real_send_packets++;
                    udpx_instance.qos.real_send_bytes += ret;
                }
                // 解锁
                pthread_mutex_unlock(&udpx_instance.qos.mutex);
            }

            start_bit = 0;
            send_total_length += send_length;
            pData += send_length;
            length -= send_length;

            LOGD("%s: send packet list length is %d, send length is %d, seq_no is 0x%x.\n", __func__, udpx_instance.send_packet_list_length, ret, seq_no);

            // 缓存发送数据包. 用于丢包重传.
            if (flag == UDPX_SEND_FLAG_DATA) {
                // 数据报文
                udpx_buffer *pBuffer = udpx_get_buffer(UDPX_PAYLOAD_MAX_LENGTH);
                if (pBuffer != NULL) {
                    pBuffer->offset = 0;
                    memcpy(pBuffer->content + pBuffer->offset, header, header_length + send_length);
                    pBuffer->payload_length = header_length + send_length;
                    // 数据包发送时时间戳.
                    pBuffer->timestamp = udpx_get_timestamp();
                    // 数据包失效时间
                    pBuffer->deadline = pBuffer->timestamp + UDPX_PACKET_LIFE_TIME;
                    // 添加到发送数据包缓存列表中.
                    udpx_send_packet_list_add(pBuffer);
                }
            } else if (flag == UDPX_SEND_FLAG_REGISTER) {
                // 注册报文无ACK, 不做丢包重传处理.
            }
        }
    }
    udpx_instance.send_msg_no = (udpx_instance.send_msg_no + 1) & UDPX_MSG_NO_MASK;
    // 解锁. 结束发送.
    pthread_mutex_unlock(&udpx_send_api_mutex);

    udpx_free_buffer(buffer);

    return send_total_length;
}

/**
 * @brief 发送注册消息
 *
 * @param buffer    待发送消息
 *
 * @return 非负值表示已发送的数据长度
 * @retval {@enum EINVALID_STATE}   udpx未运行
 * @retval {@enum ENULL_POINTER}    发送buffer为空
 * @retval {@enum EWRONG_BUFFER}    发送buffer不是通过{@func udpx_get_buffer()}获取的
 * @retval {@enum EINVALID_PARAMETER}   发送buffer的offset太小
 * @retval {@enum ESOCKET_SEND_FAILED}  socket发送失败次数超过上限
 *
 * @note buffer只能通过{@func udpx_get_buffer()}获取, 且发送成功自动释放
 */
int udpx_send_register(udpx_buffer *buffer) {
    // 清空ACK缓存
    udpx_int_ring_buffer_clear(udpx_instance.ack_buffer);
    // 清空发送报文链表
    udpx_send_packet_list_clear();
    // 清空接收消息链表
    udpx_recv_message_list_clear();

    return udpx_send(buffer, UDPX_SEND_FLAG_REGISTER);
}

/**
 * @brief 发送数据消息
 *
 * @param buffer    待发送消息
 *
 * @return 非负值表示已发送的数据长度
 * @retval {@enum EINVALID_STATE}   udpx未运行
 * @retval {@enum ENULL_POINTER}    发送buffer为空
 * @retval {@enum EWRONG_BUFFER}    发送buffer不是通过{@func udpx_get_buffer()}获取的
 * @retval {@enum EINVALID_PARAMETER}   发送buffer的offset太小
 * @retval {@enum ESOCKET_SEND_FAILED}  socket发送失败次数超过上限
 *
 * @note buffer只能通过{@func udpx_get_buffer()}获取, 且发送成功自动释放
 */
int udpx_send_data(udpx_buffer *buffer) {
    return udpx_send(buffer, UDPX_SEND_FLAG_DATA);
}

/**
 * @brief 获取发送buffer
 *
 * @param length    需要的buffer长度
 *
 * @return 可能为空
 */
udpx_buffer *udpx_get_buffer(int length) {
    /**
     * note: 申请长度须考虑预留offset.
     */
    udpx_buffer *pBuffer = udpx_buffer_get(udpx_instance.send_buffer_pool, sizeof(udpx_header) + length);
    if (pBuffer != NULL) {
        pBuffer->offset = sizeof(udpx_header);
        pBuffer->payload_length = 0;
    }
    return pBuffer;
}

/**
 * @brief 用于释放未发送的发送buffer
 *
 * @param buffer    待释放的buffer
 */
void udpx_free_buffer(udpx_buffer *buffer) {
    if (buffer == NULL) {
        LOGD("%s: buffer is null.\n", __func__);
        return;
    }
    if (buffer->pool != udpx_instance.send_buffer_pool) {
        LOGW("%s: invalid buffer.\n", __func__);;
        return;
    }
    udpx_buffer_free(buffer);
}

/**************************************************
 * UDPX消息获取.
 **************************************************/

/**
 * @brief 获取收到的消息. 不阻塞.
 *
 * @return 可能为空
 */
udpx_buffer *udpx_get_message() {
    if (udpx_instance.state != UDPX_RUNNING) {
        LOGE("%s: udpx state is invalid.\n", __func__);
        return NULL;
    }
    udpx_buffer *pBuffer = udpx_recv_message_list_del();
    return pBuffer;
}

/**
 * @brief 获取收到的消息. 带超时阻塞.
 * 
 * @param timeout   超时时间, 单位ms.
 *
 * @return 可能为空
 */
udpx_buffer *udpx_get_message_timeout(int timeout) {
    struct timespec out_time;

    if (udpx_instance.state != UDPX_RUNNING) {
        LOGE("%s: udpx state is invalid.\n", __func__);
        return NULL;
    }

    udpx_buffer *pBuffer = udpx_recv_message_list_del();
    if (pBuffer != NULL) {
        return pBuffer;
    }

    // 阻塞线程.
    // 加锁
    pthread_mutex_lock(&udpx_get_message_cond_mutex);
    if (udpx_set_timeout(&out_time, timeout) == -1) {
        LOGW("%s: udpx_set_timeout() failed.\n", __func__);
        usleep(timeout * 1000 / 2);
        // 解锁. 错误分支.
        pthread_mutex_unlock(&udpx_get_message_cond_mutex);
        return NULL;
    }

    pthread_cond_timedwait(&udpx_get_message_cond, &udpx_get_message_cond_mutex, &out_time);
    // 解锁
    pthread_mutex_unlock(&udpx_get_message_cond_mutex);

    return udpx_recv_message_list_del();
}

/**
 * @brief 释放消息
 *
 * @param buffer    待释放的buffer
 */
void udpx_free_message(udpx_buffer *buffer) {
    if (buffer == NULL) {
        LOGD("%s: buffer is null.\n", __func__);
        return;
    }
    if (buffer->pool != udpx_instance.recv_buffer_pool) {
        LOGW("%s: invalid buffer.\n", __func__);;
        return;
    }
    udpx_buffer_free(buffer);
}

