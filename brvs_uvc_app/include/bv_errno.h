#ifndef __BV_ERRNO_H__
#define __BV_ERRNO_H__

#include "bv_debug.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

typedef enum bvERR_LEVEL_E
{
	BV_ERR_LEVEL_FATAL,      /* just for compatibility with previous version */
	BV_ERR_LEVEL_ALERT,      /* action must be taken immediately             */
	BV_ERR_LEVEL_CRIT,       /* critical conditions                          */
	BV_ERR_LEVEL_ERROR,      /* error conditions                             */
	BV_ERR_LEVEL_WARNING,    /* warning conditions                           */
	BV_ERR_LEVEL_NOTICE,     /* normal but significant condition             */
	BV_ERR_LEVEL_INFO,       /* informational                                */
	BV_ERR_LEVEL_DEBUG,  /* debug-level                                  */
    BV_ERR_LEVEL_RESERVED,
}ERR_LEVEL_E;

/******************************************************************************
|----------------------------------------------------------------|
| 1 |   APP_ID   |   MOD_ID    | ERR_LEVEL |   ERR_ID            |
|----------------------------------------------------------------|
|<--><--7bits----><----8bits---><--4bits---><------12bits------->|
******************************************************************************/

#define BV_DEF_ERR(appid, module, level, errid) \
    ((BV_S32)( (1 << 31) | ((appid) << 24) | ((module) << 16 ) | ((level)<<12) | (errid) ))

/* NOTE! the following defined all common error code,
** all module must reserved 0~63 for their common error code
*/
typedef enum bvEN_ERR_CODE_E
{
	BV_ERR_PERM  = 1     ,	/* Operation not permitted */
	BV_ERR_NOENT = 2		 ,	/* No such file or directory */
	BV_ERR_SRCH	 = 3	 ,	/* No such process */
	BV_ERR_INTR	 = 4	 ,	/* Interrupted system call */
	BV_ERR_IO	 = 5	     ,	/* I/O error */
	BV_ERR_NXIO	 = 6	 ,	/* No such device or address */
	BV_ERR_AGAIN = 7		,	/* Try again */
	BV_ERR_NOMEM = 8	,	/* Out of memory */
	BV_ERR_ACCES = 9	,	/* Permission denied */
	BV_ERR_FAULT = 10		,	/* Bad address */
	BV_ERR_BUSY	 = 11	,	/* Device or resource busy */
	BV_ERR_EXIST = 12		,	/* File exists */
	BV_ERR_NODEV = 13		,	/* No such device */
	BV_ERR_INVAL = 14		,	/* Invalid argument */
	BV_ERR_NFILE = 15		,	/* File table overflow */
	BV_ERR_MFILE = 16		,	/* Too many open files */
	BV_ERR_TXTBSY= 17		,	/* Text file busy */
	BV_ERR_FBIG	 = 18	,	/* File too large */
	BV_ERR_NOSPC = 19		,	/* No space left on device */
	BV_ERR_NOSYS = 20		,	/* Function not implemented */
	BV_ERR_NOMSG = 21		,	/* No message of desired type */
	BV_ERR_CHRNG = 22		,	/* Channel number out of range */
	BV_ERR_LNRNG = 23		,	/* Link number out of range */
	BV_ERR_UNATCH= 24		,	/* Protocol driver not attached */
	BV_ERR_BADRQC= 25		,	/* Invalid request code */
	BV_ERR_BADSLT= 26		,	/* Invalid slot */

	BV_ERR_NODATA = 27		,	/* No data available */
	BV_ERR_TIME	  = 28	,	/* Timer expired */
	BV_ERR_COMM	  = 29	,	/* Communication error on send */
	BV_ERR_PROTO  = 30		,	/* Protocol error */
	BV_ERR_BADMSG = 31		,	/* Not a data message */
	BV_ERR_OVERFLOW	= 32,	/* Value too large for defined data type */
	BV_ERR_RESTART	= 33,	/* Interrupted system call should be restarted */
	BV_ERR_USERS	= 34	,	/* Too many users */
	BV_ERR_MSGSIZE	= 35,	/* Message too long */
	BV_ERR_PROTOTYPE= 36	,	/* Protocol wrong type for socket */
	BV_ERR_NOPROTOOPT = 37	,	/* Protocol not available */
	BV_ERR_PROTONOSUPPORT = 38	,	/* Protocol not supported */
	BV_ERR_SOCKTNOSUPPORT = 39	,	/* Socket type not supported */
	BV_ERR_OPNOTSUPP	= 40,	/* Operation not supported on transport endpoint */
	BV_ERR_PFNOSUPPORT	= 41,	/* Protocol family not supported */
	BV_ERR_AFNOSUPPORT	= 42,	/* Address family not supported by protocol */
	BV_ERR_ADDRINUSE	= 43,	/* Address already in use */
	BV_ERR_ADDRNOTAVAIL	= 44,	/* Cannot assign requested address */
	BV_ERR_NETDOWN	    = 45,	/* Network is down */
	BV_ERR_NETUNREACH	= 46,	/* Network is unreachable */
	BV_ERR_NETRESET	    = 47,	/* Network dropped connection because of reset */
	BV_ERR_CONNRESET	= 48,	/* Connection reset by peer */
	BV_ERR_NOBUFS		= 49,	/* No buffer space available */
	BV_ERR_ISCONN		= 50,	/* Transport endpoint is already connected */
	BV_ERR_NOTCONN  	= 51,	/* Transport endpoint is not connected */
	BV_ERR_TIMEDOUT	    = 52,	/* Connection timed out */
	BV_ERR_CONNREFUSED	= 53,	/* Connection refused */
	BV_ERR_ALREADY	    = 54,	/* Operation already in progress */
	BV_ERR_INPROGRESS	= 55,	/* Operation now in progress */
	BV_ERR_CANCELED	    = 56,	/* Operation Canceled */
	
	BV_ERR_INVALID_DEVID = 256, /* invlalid device ID                           */
    BV_ERR_INVALID_CHNID = 257, /* invlalid channel ID                          */
    BV_ERR_NULL_PTR      = 258, /* using a NULL point                           */
    BV_ERR_NOT_CONFIG    = 259, /* try to enable or initialize system, device
                              ** or channel, before configing attribute       */
    BV_ERR_SYS_NOTREADY  = 260,/* System is not ready,maybe not initialed or 
                              ** loaded. Returning the error code when opening
                              ** a device file failed.                        */
    BV_ERR_INVALID_USER = 261, /* invlalid device ID                           */
    BV_ERR_RESERVED    = 4095,/* maxium code, private error code of all modules
                              ** must be greater than it                      */
}BV_ERR_CODE_E;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  /* __BV_ERRNO_H__ */

