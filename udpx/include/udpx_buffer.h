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
 * buffer��󳤶�.
 * Сbuffer��󳤶�Ϊ4096 Bytes(4KB), ������Ϊ2000.
 * ��buffer��󳤶�Ϊ512KB, ������Ϊ50.
 */
#define UDPX_SMALL_BUFFER_LENGTH_MAX ((2<<12) - sizeof(udpx_buffer))
#define UDPX_SMALL_BUFFER_COUNT_MAX (2000)
#define UDPX_BIG_BUFFER_LENGTH_MAX ((2<<19) - sizeof(udpx_buffer))
#define UDPX_BIG_BUFFER_COUNT_MAX (100)

/**
 * buffer����.
 * �ɸ��õ�buffer����.
 */
#define UDPX_BUFFER_TYPE_REUSE (1)

/**
 * buffer����.
 * ��ʱ�����buffer����.
 */
#define UDPX_BUFFER_TYPE_TEMP (2)

/**
 * buffer״̬.
 * buffer������udpx_buffer_pool����buffer������.
 */
#define UDPX_BUFFER_STATE_LINKED (1)

/**
 * buffer״̬.
 * bufferδ������udpx_buffer_pool����buffer������.
 */
#define UDPX_BUFFER_STATE_UNLINKED (2)

/**
 * buffer�ؽڵ�.
 */
typedef struct _udpx_buffer_pool {
    /**
     * ������.
     */
    pthread_mutex_t mutex;

    /**
     * ��������Ԥ�����buffer.
     */
    struct list_head real_list_head;

    /**
     * ����������ʱ�����buffer.
     */
    struct list_head temp_list_head;

    /**
     * Ԥ����buffer��С������.
     * �������е�ǰ����Сbuffer.
     */
    struct list_head small_buffer_list_head;
    /**
     * Ԥ����Сbuffer�ĳ���.
     */
    int small_buffer_length;
    /**
     * Ԥ����Сbuffer�ĸ���.
     */
    int small_buffer_count;
    /**
     * ����Ԥ����Сbuffer�ĸ���.
     */
    int small_buffer_available_count;

    /**
     * Ԥ����buffer��С������.
     * �������е�ǰ���ô�buffer.
     */
    struct list_head big_buffer_list_head;
    /**
     * Ԥ�����buffer�ĳ���.
     */
    int big_buffer_length;
    /**
     * Ԥ�����buffer�ĸ���.
     */
    int big_buffer_count;
    /**
     * ����Ԥ�����buffer�ĸ���.
     */
    int big_buffer_available_count;
} udpx_buffer_pool;

/**
 * buffer�ڵ�.
 */
typedef struct _udpx_buffer {
    /**
     * buffer����.
     * UDPX_BUFFER_TYPE_REUSE, UDPX_BUFFER_TYPE_TEMP.
     */
    int buffer_type;

    /**
     * buffer״̬.
     * UDPX_BUFFER_STATE_LINKED, UDPX_BUFFER_STATE_UNLINKED.
     */
    int buffer_state;

    /**
     * ʱ���. 
     * ���ڷ���bufferʱ, �����ڷ���ʱ���.
     * ���ڽ���bufferʱ, �����ڽ���ʱ���.
     */
    unsigned long timestamp;

    /**
     * bufferʧЧʱ��.
     */
    unsigned long deadline;

    /**
     * buffer���ڵ�buffer������.
     */
    udpx_buffer_pool *pool;

    /**
     * ���ص�����buffer������.
     */
    struct list_head real_node;

    /**
     * ���ص�����buffer������.
     */
    struct list_head available_node;

    /**
     * �غɿ��ó���.
     */
    int length;

    /**
     * �غɿ��ÿ�ʼλ��.
     */
    int offset;

    /**
     * ��offset��ʼ����Ч�غɳ���.
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
