/*************************************************************************
	> File Name: udpx_log.h
	> Author: 
	> Mail: 
	> Created Time: Mon 30 Jul 2018 09:56:16 AM CST
 ************************************************************************/

#ifndef _PROTOCOL_LOG_H
#define _PROTOCOL_LOG_H

#ifdef __ANDROID__
#include <android/log.h>
#else 
#include <syslog.h>
#endif

#ifndef LOG_TAG
#define LOG_TAG "protocol-yview"
#endif

#ifdef __ANDROID__

#ifdef DEBUG
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#else
#define LOGV(...)
#define LOGD(...)
#endif

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL, LOG_TAG, __VA_ARGS__)

#else // __ANDROID__

#ifdef DEBUG
#define LOGV(msg, ...) syslog(LOG_DEBUG, "%s: "msg, LOG_TAG,##__VA_ARGS__)
#define LOGD(msg, ...) syslog(LOG_DEBUG, "%s: "msg, LOG_TAG,##__VA_ARGS__)
#else
#define LOGV(msg, ...)
#define LOGD(msg, ...)
#endif

#define LOGI(msg, ...) syslog(LOG_INFO, "%s: "msg, LOG_TAG,##__VA_ARGS__)
#define LOGW(msg, ...) syslog(LOG_WARNING, "%s: "msg, LOG_TAG,##__VA_ARGS__)
#define LOGE(msg, ...) syslog(LOG_ERR, "%s: "msg, LOG_TAG,##__VA_ARGS__)
#define LOGF(msg, ...) syslog(LOG_CRIT, "%s: "msg, LOG_TAG,##__VA_ARGS__)

#endif // __ANDROID__

#endif // _PROTOCOL_LOG_H

