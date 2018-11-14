/*************************************************************************
	> File Name: udpx_errno.h
	> Author: 
	> Mail: 
	> Created Time: Fri 27 Jul 2018 05:54:47 PM CST
 ************************************************************************/

#ifndef _UDPX_ERRNO_H
#define _UDPX_ERRNO_H

/**
 * 错误码.
 */
typedef enum _udpx_errno {
    SUCCESS = 0, 
    ENULL_POINTER = -1, 
    EINVALID_PARAMETER = -2, 
    ESOCKET_FAILED = -3, 
    EALREADY_STARTED = -4, 
    EUNKNOWN = -5, 
    EMALLOC_FAILED = -6, 
    EWRONG_BUFFER = -7, 
    EINVALID_STATE = -8, 
    ESOCKET_SEND_FAILED = -9, 
    EINVALID_DEVICE_ID = -10, 
    EINVALID_SERVER_IP = -11, 
    EDATA_TOO_LARGE = -12, 
} udpx_errno;

#endif
