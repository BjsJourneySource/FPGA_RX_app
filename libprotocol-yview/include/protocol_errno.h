/*************************************************************************
	> File Name: protocol_errno.h
	> Author: 
	> Mail: 
	> Created Time: Wed 26 Sep 2018 04:13:58 PM CST

 Copy from "../../udpx/include/udpx_errno.h". Shoud sync with it.
 ************************************************************************/

#ifndef _PROTOCOL_ERRNO_H
#define _PROTOCOL_ERRNO_H

/**
 * 错误码.
 */
typedef enum _prot_errno {
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
} prot_errno;

#endif
