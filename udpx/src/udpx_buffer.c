/*************************************************************************
	> File Name: udpx_buffer.c
	> Author: 
	> Mail: 
	> Created Time: Tue 31 Jul 2018 01:57:15 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

// #define UDPX_DEBUG
#define LOG_TAG "udpx_buffer"

#include "list.h"
#include "udpx_log.h"
#include "udpx_buffer.h"

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
 * @brief buffer池初始化
 *
 * @param small_length  预分配小buffer的长度
 * @param small_count   预分配小buffer的个数
 * @param big_length    预分配大buffer的长度
 * @param big_count     预分配大buffer的个数
 *
 * @return 初始话后的buffer池
 */
udpx_buffer_pool *udpx_buffer_init(int small_length, int small_count, int big_length, int big_count) {
    if (small_length < 0 || small_count < 0 || big_length < 0 || big_count < 0) {
        LOGE("%s: invalid parameters(negative value): small_length(%d), small_count(%d), big_length(%d), big_count(%d).\n", __func__, small_length, small_count, big_length, big_count);
        return NULL;
    }
    if (small_length > UDPX_SMALL_BUFFER_LENGTH_MAX || small_count > UDPX_SMALL_BUFFER_COUNT_MAX || big_length > UDPX_BIG_BUFFER_LENGTH_MAX || big_count > UDPX_BIG_BUFFER_COUNT_MAX) {
        LOGE("%s: invalid parameters(overranging): small_length(%d), small_count(%d), big_length(%d), big_count(%d).\n", __func__, small_length, small_count, big_length, big_count);
        return NULL;
    }

    int i = 0;
    udpx_buffer *pBuffer = NULL;
    udpx_buffer_pool *pool = (udpx_buffer_pool *) malloc(sizeof(udpx_buffer_pool));
    if (pool == NULL) {
        LOGE("%s: malloc pool failed.\n", __func__);
        goto ERR_1;
    }

    pthread_mutex_init(&pool->mutex, NULL);
    INIT_LIST_HEAD(&pool->real_list_head);
    INIT_LIST_HEAD(&pool->temp_list_head);

    INIT_LIST_HEAD(&pool->small_buffer_list_head);
    pool->small_buffer_length = 0;
    pool->small_buffer_count = 0;
    pool->small_buffer_available_count = 0;

    INIT_LIST_HEAD(&pool->big_buffer_list_head);
    pool->big_buffer_length = 0;
    pool->big_buffer_count = 0;
    pool->big_buffer_available_count = 0;

    if (small_length != 0) {
        for (i = 0; i < small_count; i++) {
            pBuffer = (udpx_buffer *) malloc(sizeof(udpx_buffer) + small_length);
            if (pBuffer == NULL) {
                LOGE("%s: malloc small buffer failed.\n", __func__);
                goto ERR_2;
            }
            pBuffer->pool = pool;
            pBuffer->buffer_type = UDPX_BUFFER_TYPE_REUSE;
            list_add_tail(&pBuffer->real_node, &pool->real_list_head);
            list_add_tail(&pBuffer->available_node, &pool->small_buffer_list_head);
            pBuffer->buffer_state = UDPX_BUFFER_STATE_LINKED;
            pBuffer->length = small_length;
            pBuffer->offset = 0;
            pBuffer->payload_length = 0;
        }
        pool->small_buffer_length = small_length;
        pool->small_buffer_count = small_count;
        pool->small_buffer_available_count = small_count;
    }

    if (big_length != 0) {
        for (i = 0; i < big_count; i++) {
            pBuffer = (udpx_buffer *) malloc(sizeof(udpx_buffer) + big_length);
            if (pBuffer == NULL) {
                LOGE("%s: malloc big buffer failed.\n", __func__);
                goto ERR_2;
            }
            pBuffer->pool = pool;
            pBuffer->buffer_type = UDPX_BUFFER_TYPE_REUSE;
            list_add_tail(&pBuffer->real_node, &pool->real_list_head);
            list_add_tail(&pBuffer->available_node, &pool->big_buffer_list_head);
            pBuffer->buffer_state = UDPX_BUFFER_STATE_LINKED;
            pBuffer->length = big_length;
            pBuffer->offset = 0;
            pBuffer->payload_length = 0;
        }
        pool->big_buffer_length = big_length;
        pool->big_buffer_count = big_count;
        pool->big_buffer_available_count = big_count;
    }
    LOGD("%s: init pool success. small buffer(length: %d, total count: %d, available count: %d), big buffer(length: %d, total count: %d, available count: %d).\n", __func__, 
         pool->small_buffer_length, pool->small_buffer_count, pool->small_buffer_available_count, 
         pool->big_buffer_length, pool->big_buffer_count, pool->big_buffer_available_count);

    return pool;

ERR_2:
    udpx_buffer_fini(pool);
ERR_1:
    return NULL;
}

/**
 * @brief buffer池释放
 *
 * @param pool  待释放的buffer池指针
 */
void udpx_buffer_fini(udpx_buffer_pool *pool) {
    if (pool == NULL) {
        LOGW("%s: pool is null.\n", __func__);
        return;
    }

    LOGD("%s: fini pool. small buffer(length: %d, total count: %d, available count: %d), big buffer(length: %d, total count: %d, available count: %d).\n", __func__, 
         pool->small_buffer_length, pool->small_buffer_count, pool->small_buffer_available_count, 
         pool->big_buffer_length, pool->big_buffer_count, pool->big_buffer_available_count);

    udpx_buffer *pBuffer = NULL;
    struct list_head *pNode = NULL;
    struct list_head *pTempNode = NULL;

    mutex_lock(&pool->mutex);

    // UDPX_BUFFER_TYPE_REUSE
    list_for_each_safe(pNode, pTempNode, &pool->real_list_head) {
        pBuffer = list_entry(pNode, udpx_buffer, real_node);
        if (pBuffer != NULL) {
            list_del_init(&pBuffer->real_node);
            memset(pBuffer, 0, sizeof(udpx_buffer));
            free(pBuffer);
        }
    }

    // UDPX_BUFFER_TYPE_TEMP
    list_for_each_safe(pNode, pTempNode, &pool->temp_list_head) {
        pBuffer = list_entry(pNode, udpx_buffer, real_node);
        if (pBuffer != NULL) {
            list_del_init(&pBuffer->real_node);
            memset(pBuffer, 0, sizeof(udpx_buffer));
            free(pBuffer);
        }
    }

    mutex_unlock(&pool->mutex);

    pthread_mutex_destroy(&pool->mutex);
    memset(pool, 0, sizeof(udpx_buffer_pool));
    free(pool);
}

/**
 * @brief 获取一个buffer
 *
 * @param pool  预分配的buffer池指针
 * @param length    需要buffer的长度
 *
 * @return 从{@param pool}中取得的长度不低于{@param length}的buffer
 */
udpx_buffer *udpx_buffer_get(udpx_buffer_pool *pool, int length) {
    if (pool == NULL) {
        LOGE("%s: pool is null.\n", __func__);
        return NULL;
    }

    udpx_buffer *pBuffer = NULL;

    mutex_lock(&pool->mutex);

    if (length <= pool->small_buffer_length && !list_empty(&pool->small_buffer_list_head)) {
        pBuffer = list_first_entry(&pool->small_buffer_list_head, udpx_buffer, available_node);
        if (pBuffer != NULL) {
            list_del_init(&pBuffer->available_node);
            pool->small_buffer_available_count -= 1;
            pBuffer->buffer_state = UDPX_BUFFER_STATE_UNLINKED;
            goto OUT;
        }
    }

    if (length <= pool->big_buffer_length && !list_empty(&pool->big_buffer_list_head)) {
        pBuffer = list_first_entry(&pool->big_buffer_list_head, udpx_buffer, available_node);
        if (pBuffer != NULL) {
            list_del_init(&pBuffer->available_node);
            pool->big_buffer_available_count -= 1;
            pBuffer->buffer_state = UDPX_BUFFER_STATE_UNLINKED;
            goto OUT;
        }
    }

    LOGW("%s: small buffer(length: %d, total count: %d, available count: %d), big buffer(length: %d, total count: %d, available count: %d).\n", __func__, 
         pool->small_buffer_length, pool->small_buffer_count, pool->small_buffer_available_count, 
         pool->big_buffer_length, pool->big_buffer_count, pool->big_buffer_available_count);

    if (length > 0) {
        pBuffer = (udpx_buffer *) malloc(sizeof(udpx_buffer) + length);
        if (pBuffer == NULL) {
            LOGE("%s: malloc temp buffer(%d) failed.\n", __func__, length);
            goto OUT;
        }

        pBuffer->pool = pool;
        pBuffer->buffer_type = UDPX_BUFFER_TYPE_TEMP;
        list_add_tail(&pBuffer->real_node, &pool->temp_list_head);
        // pBuffer->buffer_state = UDPX_BUFFER_STATE_UNLINKED;
        pBuffer->length = length;
        pBuffer->offset = 0;
        pBuffer->payload_length = 0;
    }

OUT:
    mutex_unlock(&pool->mutex);
    return pBuffer;
}

/**
 * @brief 释放buffer
 *
 * @param buffer    待释放的buffer指针
 */
void udpx_buffer_free(udpx_buffer *buffer) {
    if (buffer == NULL) {
        LOGD("%s: buffer is null.\n", __func__);
        return;
    }

    udpx_buffer_pool *pool = buffer->pool;
    if (pool == NULL) {
        LOGW("%s: unknown buffer.\n", __func__);
        list_del_init(&buffer->real_node);
        free(buffer);
        return;
    }

    mutex_lock(&pool->mutex);

    if (buffer->buffer_type == UDPX_BUFFER_TYPE_REUSE) {
        if (buffer->buffer_state == UDPX_BUFFER_STATE_LINKED) {
            LOGW("%s: buffer is already freed.\n", __func__);
            goto OUT;
        }
        if (buffer->length == pool->small_buffer_length) {
            list_add_tail(&buffer->available_node, &pool->small_buffer_list_head); 
            pool->small_buffer_available_count += 1;
            buffer->buffer_state = UDPX_BUFFER_STATE_LINKED;
        } else if (buffer->length == pool->big_buffer_length) {
            list_add_tail(&buffer->available_node, &pool->big_buffer_list_head); 
            pool->big_buffer_available_count += 1;
            buffer->buffer_state = UDPX_BUFFER_STATE_LINKED;
        } else {
            LOGW("%s: buffer length is invalid.\n", __func__);
            list_del_init(&buffer->real_node);
            free(buffer);
        }
    } else if (buffer->buffer_type == UDPX_BUFFER_TYPE_TEMP) {
        list_del_init(&buffer->real_node);
        memset(buffer, 0, sizeof(udpx_buffer));
        free(buffer);
    } else {
        LOGW("%s: unknown buffer type.\n", __func__);
        list_del_init(&buffer->real_node);
        free(buffer);
    }

OUT:
    mutex_unlock(&pool->mutex);
}

