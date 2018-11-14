/*************************************************************************
	> File Name: udpx_buffer.h
	> Author: 
	> Mail: 
	> Created Time: Tue 31 Jul 2018 01:52:17 PM CST
 ************************************************************************/

#ifndef _UDPX_BUFFER_H
#define _UDPX_BUFFER_H

#include "list.h"

/**
 * buffer最大长度.
 * 小buffer最大长度为4096 Bytes(4KB), 最大个数为2000.
 * 大buffer最大长度为512KB, 最大个数为50.
 */
#define UDPX_SMALL_BUFFER_LENGTH_MAX ((2<<12) - sizeof(udpx_buffer))
#define UDPX_SMALL_BUFFER_COUNT_MAX (2000)
#define UDPX_BIG_BUFFER_LENGTH_MAX ((2<<19) - sizeof(udpx_buffer))
#define UDPX_BIG_BUFFER_COUNT_MAX (100)

/**
 * buffer类型.
 * 可复用的buffer类型.
 */
#define UDPX_BUFFER_TYPE_REUSE (1)

/**
 * buffer类型.
 * 临时申请的buffer类型.
 */
#define UDPX_BUFFER_TYPE_TEMP (2)

/**
 * buffer状态.
 * buffer挂载在udpx_buffer_pool可用buffer链表上.
 */
#define UDPX_BUFFER_STATE_LINKED (1)

/**
 * buffer状态.
 * buffer未挂载在udpx_buffer_pool可用buffer链表上.
 */
#define UDPX_BUFFER_STATE_UNLINKED (2)

/**
 * buffer池节点.
 */
typedef struct _udpx_buffer_pool {
    /**
     * 互斥锁.
     */
    pthread_mutex_t mutex;

    /**
     * 索引所有预分配的buffer.
     */
    struct list_head real_list_head;

    /**
     * 索引所有临时分配的buffer.
     */
    struct list_head temp_list_head;

    /**
     * 预分配buffer大小分两档.
     * 索引所有当前可用小buffer.
     */
    struct list_head small_buffer_list_head;
    /**
     * 预分配小buffer的长度.
     */
    int small_buffer_length;
    /**
     * 预分配小buffer的个数.
     */
    int small_buffer_count;
    /**
     * 可用预分配小buffer的个数.
     */
    int small_buffer_available_count;

    /**
     * 预分配buffer大小分两档.
     * 索引所有当前可用大buffer.
     */
    struct list_head big_buffer_list_head;
    /**
     * 预分配大buffer的长度.
     */
    int big_buffer_length;
    /**
     * 预分配大buffer的个数.
     */
    int big_buffer_count;
    /**
     * 可用预分配大buffer的个数.
     */
    int big_buffer_available_count;
} udpx_buffer_pool;

/**
 * buffer节点.
 */
typedef struct _udpx_buffer {
    /**
     * buffer类型.
     * UDPX_BUFFER_TYPE_REUSE, UDPX_BUFFER_TYPE_TEMP.
     */
    int buffer_type;

    /**
     * buffer状态.
     * UDPX_BUFFER_STATE_LINKED, UDPX_BUFFER_STATE_UNLINKED.
     */
    int buffer_state;

    /**
     * 时间戳. 
     * 用于发送buffer时, 可用于发送时间戳.
     * 用于接收buffer时, 可用于接收时间戳.
     */
    unsigned long timestamp;

    /**
     * buffer失效时间.
     */
    unsigned long deadline;

    /**
     * buffer所在的buffer池索引.
     */
    udpx_buffer_pool *pool;

    /**
     * 挂载到所有buffer链表上.
     */
    struct list_head real_node;

    /**
     * 挂载到可用buffer链表上.
     */
    struct list_head available_node;

    /**
     * 载荷可用长度.
     */
    int length;

    /**
     * 载荷可用开始位置.
     */
    int offset;

    /**
     * 从offset开始的有效载荷长度.
     */
    int payload_length;

    unsigned char content[0];
} udpx_buffer;

#ifdef __cplusplus
extern "C" {
#endif
    udpx_buffer_pool *udpx_buffer_init(int small_length, int small_count, int big_length, int big_count);
    void udpx_buffer_fini(udpx_buffer_pool *pool);
    udpx_buffer *udpx_buffer_get(udpx_buffer_pool *pool, int length);
    void udpx_buffer_free(udpx_buffer *buffer);
#ifdef __cplusplus
}
#endif

#endif
