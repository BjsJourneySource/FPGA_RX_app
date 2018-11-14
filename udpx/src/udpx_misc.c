/*************************************************************************
	> File Name: udpx_misc.c
	> Author: 
	> Mail: 
	> Created Time: Mon 30 Jul 2018 07:18:36 PM CST
 ************************************************************************/

#include <stdlib.h>
#include <sys/time.h>

#include "udpx_misc.h"

/**
 * @brief 生成随机数
 * 范围: 1~65536
 */
int udpx_rand() {
    return udpx_rand_range(65536);
}

/**
 * @brief 生成随机数
 *
 * @param range 随机数范围. 
 *      若为非正值, 生成值为0
 */
int udpx_rand_range(int range) {
    if (range <= 0) {
        return 0;
    }

    struct timeval now;
    gettimeofday(&now, NULL);
    unsigned int seed = now.tv_usec;
    srand(seed);
    return (rand() % range) + 1;
}

/**
 * @brief 设置超时时间
 *
 * @param time  传出参数
 * @param timeout   超时时间. 单位ms
 *
 * @return
 * @retval 0    成功
 * @retval -1   失败
 */
int udpx_set_timeout(struct timespec *time, int timeout) {
    struct timeval now;

    if (gettimeofday(&now, NULL) == -1) {
        return -1;
    }
    now.tv_usec += (timeout % 1000) * 1000;
    now.tv_sec += (timeout / 1000) + (now.tv_usec / 1000000);
    now.tv_usec %= 1000000;
    time->tv_sec = now.tv_sec;
    time->tv_nsec = now.tv_usec * 1000;

    return 0;
}

/**
 * @brief 获取当前时间戳
 *
 * @return 当前时间戳. 单位: ms
 */
unsigned long udpx_get_timestamp() {
    unsigned long ts = 0;
    struct timeval now;
    if (gettimeofday(&now, NULL) == 0) {
        ts = (now.tv_sec * 1000) + (now.tv_usec / 1000);
    }
    return ts;
}

/**
 * @brief 获取当前时间戳
 *
 * @return 当前时间戳. 单位: us
 */
unsigned long long udpx_get_timestamp_us() {
    unsigned long long ts = 0;
    struct timeval now;
    if (gettimeofday(&now, NULL) == 0) {
        ts = (now.tv_sec * 1000000) + (now.tv_usec / 1000000);
    }
    return ts;
}

#ifndef __ANDROID__
#include <unistd.h>
#include <sys/syscall.h>

/**
 * @brief 获取调用线程ID.
 */
pid_t gettid(void) {
    return syscall(SYS_gettid);
}
#endif

