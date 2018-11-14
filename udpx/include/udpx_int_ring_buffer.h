/*************************************************************************
	> File Name: udpx_int_ring_buffer.h
	> Author: 
	> Mail: 
	> Created Time: Tue 07 Aug 2018 08:54:02 PM CST
 ************************************************************************/

#ifndef _UDPX_INT_RING_BUFFER_H
#define _UDPX_INT_RING_BUFFER_H

#include <pthread.h>

#define UDPX_RING_BUFFER_LENGTH_MAX (1000)

typedef struct _udpx_int_ring_buffer {
    /**
     * 互斥锁.
     */
    pthread_mutex_t mutex;

    /**
     * 头下标.
     */
    int head_index;

    /**
     * 尾下标.
     */
    int tail_index;

    /**
     * buffer长度.
     */
    int length;

    /**
     * buffer.
     */
    int buffer[0];
} udpx_int_ring_buffer;

#ifdef __cplusplus
extern "C" {
#endif
    udpx_int_ring_buffer *udpx_int_ring_buffer_init(int length);
    void udpx_int_ring_buffer_fini(udpx_int_ring_buffer *buffer);
    int udpx_int_ring_buffer_get(udpx_int_ring_buffer *buffer);
    void udpx_int_ring_buffer_put(udpx_int_ring_buffer *buffer, int val);
    void udpx_int_ring_buffer_clear(udpx_int_ring_buffer *buffer);
    int udpx_int_ring_buffer_is_empty(udpx_int_ring_buffer *buffer);
#ifdef __cplusplus    
}
#endif

#endif
