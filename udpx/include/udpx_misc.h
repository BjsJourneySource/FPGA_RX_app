/*************************************************************************
	> File Name: udpx_misc.h
	> Author: 
	> Mail: 
	> Created Time: Fri 27 Jul 2018 05:28:56 PM CST
 ************************************************************************/

#ifndef _MISC_H
#define _MISC_H

#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif
    int udpx_rand();
    int udpx_rand_range(int range);
    int udpx_set_timeout(struct timespec *time, int timeout);
    unsigned long udpx_get_timestamp();
    unsigned long long udpx_get_timestamp_us();

#ifndef __ANDROID__
    pid_t gettid(void);
#endif

#ifdef __cplusplus
}
#endif

#endif
