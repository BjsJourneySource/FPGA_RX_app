/*************************************************************************
	> File Name: udpx_int_ring_buffer.c
	> Author: 
	> Mail: 
	> Created Time: Tue 07 Aug 2018 08:59:21 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define UDPX_DEBUG
#define LOG_TAG "udpx_int_ring_buffer"

#include "udpx_log.h"
#include "udpx_int_ring_buffer.h"

/**
 * @brief 加锁
 *
 * @param m 互斥锁
 *
 * @return 
 */
static int mutex_lock(pthread_mutex_t *m) {
    return pthread_mutex_lock(m);
}

/**
 * @brief 解锁
 *
 * @param m 互斥锁
 *
 * @return
 */
static int mutex_unlock(pthread_mutex_t *m) {
    return pthread_mutex_unlock(m);
}

/**
 * @brief 初始化ring buffer
 *
 * @param length    buffer长度
 *
 * @return 缓存buffer.
 */
udpx_int_ring_buffer *udpx_int_ring_buffer_init(int length) {
    if (length <= 0) {
        LOGE("%s: negative length(%d).\n", __func__, length);
        return NULL;
    }
    if (length > UDPX_RING_BUFFER_LENGTH_MAX) {
        LOGE("%s: length is too larger(%d).\n", __func__, length);
    }

    udpx_int_ring_buffer *buffer = (udpx_int_ring_buffer *) malloc(sizeof(udpx_int_ring_buffer) + sizeof(int) * length);
    if (buffer == NULL) {
        LOGE("%s: malloc buffer failed.\n", __func__);
        goto ERR;
    }

    pthread_mutex_init(&buffer->mutex, NULL);
    buffer->head_index = 0;
    buffer->tail_index = 0;
    buffer->length = length;

    return buffer;

ERR:
    return NULL;
}

/**
 * @brief 释放ring buffer
 *
 * @param buffer    待释放buffer
 */
void udpx_int_ring_buffer_fini(udpx_int_ring_buffer *buffer) {
    if (buffer == NULL) {
        LOGW("%s: buffer is null.\n", __func__);
        return;
    }

    pthread_mutex_destroy(&buffer->mutex);
    memset(buffer, 0, sizeof(udpx_int_ring_buffer));
    free(buffer);
}

/**
 * @brief 获取buffer头部数据
 *
 * @param buffer    buffer指针
 *
 * @return
 */
int udpx_int_ring_buffer_get(udpx_int_ring_buffer *buffer) {
    if (buffer == NULL) {
        LOGW("%s: buffer is null.\n", __func__);
        return 0;
    }

    int retval = 0;

    mutex_lock(&buffer->mutex);

    if (buffer->head_index == buffer->tail_index) {
        LOGW("%s: buffer is empty.\n", __func__);
        retval = 0;
        goto OUT;
    }

    retval = buffer->buffer[buffer->head_index];
    buffer->head_index = (buffer->head_index + 1) % buffer->length;

OUT:
    mutex_unlock(&buffer->mutex);

    return retval;
}

/**
 * @brief 添加数据到buffer尾部
 *
 * @param buffer    buffer指针
 * @param val   待添加的值
 */
void udpx_int_ring_buffer_put(udpx_int_ring_buffer *buffer, int val) {
    if (buffer == NULL) {
        LOGW("%s: buffer is null.\n", __func__);
        return;
    }

    mutex_lock(&buffer->mutex);

    buffer->buffer[buffer->tail_index] = val;
    buffer->tail_index = (buffer->tail_index + 1) % buffer->length;
    if (buffer->tail_index == buffer->head_index) {
        buffer->head_index = (buffer->head_index + 1) % buffer->length;
    }

    mutex_unlock(&buffer->mutex);
}

/**
 * @brief 清除buffer内容
 *
 * @param buffer    buffer指针
 */
void udpx_int_ring_buffer_clear(udpx_int_ring_buffer *buffer) {
    mutex_lock(&buffer->mutex);
    buffer->head_index = 0;
    buffer->tail_index = 0;
    mutex_unlock(&buffer->mutex);
}

/**
 * @brief 判断buffer是否为空
 *
 * @param buffer buffer指针
 *
 * @return
 * @retval 0    非空
 * @retval 1    空
 */
int udpx_int_ring_buffer_is_empty(udpx_int_ring_buffer *buffer) {
    if (buffer == NULL) {
        LOGW("%s: buffer is null.\n", __func__);
        return 1;
    }

    int empty = 0;
    mutex_lock(&buffer->mutex);
    empty = buffer->head_index == buffer->tail_index ? 1 : 0;
    mutex_unlock(&buffer->mutex);

    return empty;
}

