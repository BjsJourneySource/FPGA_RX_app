/***************************************************************************************
 * 文 件 名 : yvcommon_data.h
 * 项目名称 : YVGA1207001A                                                            
 * 模 块 名 : 共公头文件
 * 功    能 : 服务器与设备交互的消息结构体的定义                                                          
 * 操作系统 : LINUX                                                                  
 * 修改记录 : 初始版本                                                                
 * 版    本 : Rev 0.1.0                                                               
 *--------------------------------------------------------------------------------------
 * 设    计 : liangy      '2012-07-03
 * 编    码 : liangy      '2012-07-03
 * 修    改 :                                                                          
 ***************************************************************************************
 *- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 * 公司介绍及版权说明                                                                   
 *                                                                                      
 *               (C)Copyright 2012 YView    Corporation All Rights Reserved.            
 *- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 ***************************************************************************************/

#ifndef _YVCOMMON_SRV_DEV_MSG_STRUCT_H_
#define _YVCOMMON_SRV_DEV_MSG_STRUCT_H_

#include "yvcommon.h"
#include "yvcommon_data_struct.h"
#define MAX_VIDEO_COLOR_SCENE_NUM 3
/* 设备认证请求 DEV_REQ_SRV_DEVICE_AUTHEN ，设备->安全认证 */
/* OLD MSG=DEV_REQ_DEVICE_AUTHEN STRUCT=ST_DeviceAuthenReq */
typedef struct SDevReqSrvDeviceAuthen
{
	yv_InterCommHdr stInterHdr;
	ST_DataHdr stDataHdr;
	ST_Device stDevice;
} ST_DevReqSrvDeviceAuthen;


/* 设备认证结果 DEV_RES_SRV_DEVICE_AUTHEN ，安全认证->设备 */
/* OLD MSG=DEV_RES_DEVICE_AUTHEN STRUCT=ST_DeviceAuthenRes */
typedef struct SDevResSrvDeviceAuthen
{
	yv_InterCommHdr stInterHdr;
	ST_DataHdr stDataHdr;
	ST_MsgRespRet stMsgRet;
	ST_Device stDevice;
	unsigned char byBigNumBuf[0];
} ST_DevResSrvDeviceAuthen;


/* 设备注销 DEV_NFY_SRV_DEVICE_LOGOUT ，设备->设备管理 */
/* OLD MSG=DEV_NFY_DEVICE_LOGOUT STRUCT=ST_DeviceLogoutInterCommNfy */
typedef struct SDevNfySrvDeviceLogout
{
	yv_InterCommHdr stInterHdr;
	ST_Device stDevice;
} ST_DevNfySrvDeviceLogout;


/*心跳请求消息 DEV_REQ_SRV_KEEP_ALIVE,用户、设备、服务器管理->用户、设备、服务器*/
/* OLD MSG=COM_REQ_KEEP_ALIVE STRUCT=ST_KeepAliveReq */
typedef struct SDevReqSrvKeepAlive
{
	yv_InterCommHdr stInterHdr;
	ST_DataHdr stDataHdr;
} ST_DevReqSrvKeepAlive;


/*心跳响应消息 DEV_RES_SRV_KEEP_ALIVE,用户、设备、服务器->用户、设备、服务器管理*/
/* OLD MSG=COM_RES_KEEP_ALIVE STRUCT=ST_KeepAliveRes */
typedef struct SDevResSrvKeepAlive
{
	yv_InterCommHdr stInterHdr;
	ST_DataHdr stDataHdr;
	ST_MsgRespRet stMsgRet;
} ST_DevResSrvKeepAlive;

/* SRV_REQ_DEV_STREAM_INFO 查询设备上流信息请求消息*/
/* OLD MSG=CLT_REQ_DEV_STREAM_INFO STRUCT=ST_CltDevStreamInfoReq */
typedef struct SSrvReqDevStreamInfo
{
	yv_InterCommHdr stInterHdr;      /* 业务头   */
	ST_DataHdr stDataHdr;            /* 数据头   */
}ST_SrvReqDevStreamInfo;
/* SRV_RES_DEV_STREAM_INFO 查询设备上流信息响应消息*/
/* OLD MSG=CLT_RES_DEV_STREAM_INFO STRUCT=ST_CltDevStreamInfoRes */
typedef struct SSrvResDevStreamInfo
{
	yv_InterCommHdr stInterHdr;      /* 业务头   */
	ST_DataHdr stDataHdr;            /* 数据头   */
	ST_MsgRespRet stMsgRes;          /* 响应结果 */
	ST_DevStreamInfo stDevStreamInfo;/*设备流信息*/
}ST_SrvResDevStreamInfo;

/* 要求设备发送数据通知 SRV_NFY_DEV_START_MEDIA_STREAM */
/* OLD MSG=SMP_NFY_ORDER_DEV_TRANSMIT STRUCT=ST_MsgSMPDevCtrl(同停止) */
typedef struct SSrvNfyDevStartMediaStream
{
	yv_InterCommHdr stInterHdr;
	ST_SMPDevCtrl stSMPDevCtrl;
} ST_SrvNfyDevStartMediaStream;


/* 要求设备停止发送数据通知 SRV_NFY_DEV_STOP_MEDIA_STREAM */
/* OLD MSG=SMP_NFY_ORDER_DEV_STOPTRANSMIT STRUCT=ST_MsgSMPDevCtrl */
/* 流媒体要求设备发送或停止流指令的消息 */
typedef struct SSrvNfyDevStopMediaStream
{
	yv_InterCommHdr stInterHdr;
	ST_SMPDevCtrl stSMPDevCtrl;
} ST_SrvNfyDevStopMediaStream;

/*流媒体数据报文结构体 DEV_SRM_SRV_MEDIA_STREAM */
/* OLD MSG=MSG_DEV_DAT_STM STRUCT=ST_MsgStreamHdr */
typedef struct SDevSrmSrvMediaStream
{
	yv_InterCommHdr stInterCommHdr; /*业务头部*/
	ST_StreamHdr stStreamHdr;       /*流媒体头部*/
	char Content[0];
}ST_DevSrmSrvMediaStream;

/*DEV_NFY_SRV_STATUS_INFO 设备信息上报通知消息*/
/* OLD MSG=DEV_NFY_DEV_STATUS_INFO STRUCT=ST_DevStatusInfoUploadNfy */
typedef struct SDevNfySrvStatusInfo
{
	yv_InterCommHdr stInterHdr;      /* 业务头   */
	ST_DevStatusInfo stDevStatusInfo;/*设备状态信息*/
} ST_DevNfySrvStatusInfo;

/*SRV_REQ_DEV_CHG_VIDEO_PARAM 更改视采集参数请求*/
/* OLD MSG=CLT_REQ_CHG_VIDEO_PARAM STRUCT=ST_CltChgVideoParamReq */

typedef struct SSrvReqDevChgVideoParam
{
	yv_InterCommHdr stInterHdr;
	ST_DataHdr stDataHdr;
	ST_CltChgVideoParam stCltChgVideoParam;
}ST_SrvReqDevChgVideoParam;

/*SRV_RES_DEV_CHG_VIDEO_PARAM 更改视频采集参数响应*/
/* OLD MSG=CLT_RES_CHG_VIDEO_PARAM STRUCT=ST_CltChgVideoParamRes */
typedef struct SSrvResDevChgVideoParam
{
	yv_InterCommHdr stInterHdr;
	ST_DataHdr stDataHdr;
	ST_MsgRespRet stMsgRet;
}ST_SrvResDevChgVideoParam;

/*设备配置查询请求SRV_REQ_DEV_PARAM_QUERY*/
typedef struct
{
	yv_InterCommHdr stInterHdr;     /* 业务头*/
	ST_DataHdr stDataHdr;           /* 数据头*/
	ST_DevConfigQuery stQuery;      /* 数据部*/
}ST_DevConfigQueryReq;

/*设备配置查询响应SRV_RES_DEV_PARAM_QUERY*/
typedef struct
{
	yv_InterCommHdr stInterHdr;     /* 业务头*/
	ST_DataHdr stDataHdr;           /* 数据头*/
	ST_MsgRespRet stMsgRet;         /* 响应结果*/
	char content[0];                /*TLV形式，后面跟随一项或多项TLV结构的配置*/
}ST_DevConfigQueryRes;


/*设备配置设置请求SRV_REQ_DEV_PARAM_SET*/
typedef struct
{
	yv_InterCommHdr stInterHdr;     /* 业务头*/
	ST_DataHdr stDataHdr;           /* 数据头*/
	ST_DevConfigSet stSet;          /* 数据部*/
}ST_DevConfigSetReq;

/*设备配置设置响应SRV_RES_DEV_PARAM_SET*/
typedef struct
{
	yv_InterCommHdr stInterHdr;     /* 业务头*/
	ST_DataHdr stDataHdr;           /* 数据头*/
	ST_MsgRespRet stMsgRet;         /* 响应结果*/
}ST_DevConfigSetRes;

/*获取设备镜头参数请求  SRV_REQ_DEV_GET_CAMERA_CFG*/
typedef struct SSrvReqDevGetCameraCfg
{
	yv_InterCommHdr stInterHdr;     /* 业务头*/
	ST_DataHdr stDataHdr;           /* 数据头*/
} ST_SrvReqDevGetCameraCfg;


/*获取设备镜头参数响应  SRV_RES_DEV_GET_CAMERA_CFG*/
typedef struct SSrvResDevGetCameraCfg
{
	yv_InterCommHdr stInterHdr;     /* 业务头*/
	ST_DataHdr stDataHdr;           /* 数据头*/
	ST_MsgRespRet stMsgRet;
	ST_CameraCfg stCameraCfg;   /*设备镜头参数描述*/
} ST_SrvResDevGetCameraCfg;


/*获取设备图像属性请求 SRV_REQ_DEV_GET_VIDEO_COLOR_ATTR*/
typedef struct SSrvReqDevGetVideoColorAttr
{
	yv_InterCommHdr stInterHdr;     /* 业务头*/
	ST_DataHdr stDataHdr;           /* 数据头*/
} ST_SrvReqDevGetVideoColorAttr;

/*获取设备图像属性响应  SRV_RES_DEV_GET_VIDEO_COLOR_ATTR*/
typedef struct SSrvResDevGetVideoColorAttr
{
	yv_InterCommHdr stInterHdr;     /* 业务头*/
	ST_DataHdr stDataHdr;           /* 数据头*/
	ST_MsgRespRet stMsgRet;
	ST_VideoColorAttr stVideoColorAttr;        /*设备图像属性描述*/
} ST_SrvResDevGetVideoColorAttr;
/*获取移动侦测属性请求SRV_REQ_DEV_GET_MOTTON_DETECT_ATTR*/
typedef struct SSrvReqDevGetMottonDetectAttr
{
	yv_InterCommHdr stInterHdr;     /* 业务头*/
	ST_DataHdr stDataHdr;           /* 数据头*/
} ST_SrvReqDevGetMottonDetectAttr;
/*获取移动侦测属性响应 SRV_RES_DEV_GET_MOTTON_DETECT_ATTR*/
typedef struct SSrvResDevGetMottonDetectAttr
{
	yv_InterCommHdr stInterHdr;     /* 业务头*/
	ST_DataHdr stDataHdr;           /* 数据头*/
	ST_MsgRespRet stMsgRet;
	ST_MottonDetectAttr stMotDetAttr;   /*移动侦测属性*/
} ST_SrvResDevGetMottonDetectAttr;

/*设置设备图像属性请求  SRV_REQ_DEV_SET_VIDEO_COLOR_ATTR*/
typedef struct SSrvReqDevSetVideoColorAttr
{
	yv_InterCommHdr stInterHdr;     /* 业务头*/
	ST_DataHdr stDataHdr;           /* 数据头*/
	ST_VideoColorAttr stVideoColorAttr;   /*设备图像属性描述*/
} ST_SrvReqDevSetVideoColorAttr;

/*设置设备图像属性响应  SRV_RES_DEV_SET_VIDEO_COLOR_ATTR*/
typedef struct SSrvResDevSetVideoColorAttr
{
	yv_InterCommHdr stInterHdr;     /* 业务头*/
	ST_DataHdr stDataHdr;           /* 数据头*/
	ST_MsgRespRet stMsgRet;
} ST_SrvResDevSetVideoColorAttr;

/*设置设备镜头参数请求 SRV_REQ_DEV_SET_CAMERA_CFG*/
typedef struct SSrvReqDevSetCameraCfg
{
	yv_InterCommHdr stInterHdr;     /* 业务头*/
	ST_DataHdr stDataHdr;           /* 数据头*/
	ST_CameraCfg stCameraCfg;   /*设备镜头参数描述*/
} ST_SrvReqDevSetCameraCfg;

/*设置设备镜头参数响应  SRV_RES_DEV_SET_CAMERA_CFG*/
typedef struct SSrvResDevSetCameraCfg
{
	yv_InterCommHdr stInterHdr;     /* 业务头*/
	ST_DataHdr stDataHdr;           /* 数据头*/
	ST_MsgRespRet stMsgRet;
} ST_SrvResDevSetCameraCfg;

/*设置移动侦测属性请求SRV_REQ_DEV_SET_MOTTON_DETECT_ATTR*/
typedef struct SSrvReqDevSetMottonDetectAttr
{
	yv_InterCommHdr stInterHdr;     /* 业务头*/
	ST_DataHdr stDataHdr;           /* 数据头*/
	ST_MottonDetectAttr stMotDetAttr;   /*移动侦测属性*/
} ST_SrvReqDevSetMottonDetectAttr;

/*设置移动侦测属性响应  SRV_RES_DEV_SET_MOTTON_DETECT_ATTR*/
typedef struct SSrvResDevSetMottonDetectAttr
{
	yv_InterCommHdr stInterHdr;     /* 业务头*/
	ST_DataHdr stDataHdr;           /* 数据头*/
	ST_MsgRespRet stMsgRet;
} ST_SrvResDevSetMottonDetectAttr;

/*设备图像场景查询请求消息 SRV_REQ_DEV_GET_VIDEO_COLOR_SCENE*/
typedef struct SSrvReqDevGetVideoColorScene
{
	yv_InterCommHdr stInterHdr;     /* 业务头*/
	ST_DataHdr stDataHdr;           /* 数据头*/
} ST_SrvReqDevGetVideoColorScene;

/*设备图像场景查询响应消息 SRV_RES_DEV_GET_VIDEO_COLOR_SCENE*/
typedef struct SSrvResDevGetVideoColorScene
{
	yv_InterCommHdr stInterHdr;     /* 业务头*/
	ST_DataHdr stDataHdr;           /* 数据头*/
	ST_MsgRespRet stMsgRet;
	ST_VideoColorScene astVideoColorScene[MAX_VIDEO_COLOR_SCENE_NUM];/*设备图像场景参数描述*/
} ST_SrvResDevGetVideoColorScene;

/*应用设备图像场景请求 SRV_REQ_DEV_USE_VIDEO_COLOR_SCENE*/
typedef struct SSrvReqDevSetVideoColorScene
{
	yv_InterCommHdr stInterHdr;     /* 业务头*/
	ST_DataHdr stDataHdr;           /* 数据头*/
	int lSceneID;/*场景的序号: 1：场景一   2：场景二  3：场景三*/
	char resel[8];
} ST_SrvReqDevSetVideoColorScene;

/*应用设备图像场景响应 SRV_RES_DEV_USE_VIDEO_COLOR_SCENE*/
typedef struct SSrvResDevSetVideoColorScene
{
	yv_InterCommHdr stInterHdr;     /* 业务头*/
	ST_DataHdr stDataHdr;           /* 数据头*/
	ST_MsgRespRet stMsgRet;
} ST_SrvResDevSetVideoColorScene;

/*保存图像场景请求消息  SRV_REQ_DEV_SET_VIDEO_COLOR_SCENE*/
typedef struct SSrvReqDevSevVideoColorScene
{
	yv_InterCommHdr stInterHdr;     /* 业务头*/
	ST_DataHdr stDataHdr;           /* 数据头*/
	ST_VideoColorScene stVideoColorScene;   /*设备图像场景参数描述*/
} ST_SrvReqDevSevVideoColorScene;

/*保存图像场景响应消息  SRV_RES_DEV_SET_VIDEO_COLOR_SCENE*/
typedef struct SSrvResDevSevVideoColorScene
{
	yv_InterCommHdr stInterHdr;     /* 业务头*/
	ST_DataHdr stDataHdr;           /* 数据头*/
	ST_MsgRespRet stMsgRet;
} ST_SrvResDevSevVideoColorScene;

/*恢复图像默认属性请求SRV_REQ_DEV_RESET_VIDEO_COLOR_ATTR*/
typedef struct SSrvReqDevResetVideoColorAttr
{
	yv_InterCommHdr stInterHdr;     /* 业务头*/
	ST_DataHdr stDataHdr;           /* 数据头*/
} ST_SrvReqDevResetVideoColorAttr;

/*恢复设备图像默认属性响应SRV_RES_DEV_RESET_VIDEO_COLOR_ATTR*/
typedef struct SSrvResDevResetVideoColorAttr
{
	yv_InterCommHdr stInterHdr;     /* 业务头*/
	ST_DataHdr stDataHdr;           /* 数据头*/
	ST_MsgRespRet stMsgRet;
} ST_SrvResDevResetVideoColorAttr;
#endif
