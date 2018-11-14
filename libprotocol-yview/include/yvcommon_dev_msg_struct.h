/***************************************************************************************
 * 文 件 名 : yvcommon_dev_msg_struct.h                                            
 * 项目名称 : YVGA1207001A                                                            
 * 模 块 名 :                                                         
 * 功    能 : 设备内部交互的消息结构体的定义                                                          
 * 操作系统 : LINUX                                                                  
 * 修改记录 : 初始版本                                                                
 * 版    本 : Rev 0.1.0                                                               
 *--------------------------------------------------------------------------------------
 * 设    计 : chenlg      '2013-2-22                                                  
 * 编    码 : chenlg      '2013-2-22                                                  
 * 修    改 :                                                                          
 ***************************************************************************************
 *- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 * 公司介绍及版权说明                                                                   
 *                                                                                      
 *               (C)Copyright 2012 YView    Corporation All Rights Reserved.            
 *- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 ***************************************************************************************/

#ifndef _YVCOMMON_DEV_MSG_STRUCT_H
#define _YVCOMMON_DEV_MSG_STRUCT_H


/***************************************************************************************/
/*内部消息数据部定义*/
/*数据访问相关消息数据部定义*/

#include "yvcommon_data_struct.h"
#include "yvcommon.h"

/*跟同信模块交互的部分消息数据部通用结构体*/
typedef struct SDevComSession
{
	unsigned int lSessionID;
}ST_DevComSession;

/*流媒体要求改变的视频采集参数*/
typedef struct SSMPChgVideoParam
{
	int lStreamID;          /* 所要更改的流编号*/
	int lBitrate;           /* 码率*/
	int lFramerate;         /* 帧率*/
	int lDpi;               /* 分辨率*/
}ST_SMPChgVideoParam;

/*服务器通信信息*/
/*服务器通信信息*/
typedef struct SSrvCommInfo
{
	unsigned int lSessionID;            /* 通信session*/
	unsigned int lLocalID;              /* 本地ID*/
	unsigned int lSrvID;                /* 服务器ID*/
	unsigned int lSrvPort;              /* 服务器端口*/
	char szSrvIP[MAX_LEN_IP_STR];       /* 服务器IP*/
	unsigned int lIsEnCyp;			 		 /*是否数据加密*/
	char resvert[32];						 /*保留位*/
}ST_SrvCommInfo;

/*设备登录信息结构体*/
typedef struct SDevLoginInfo
{
	int lLoginFlag;                 /* 是否已登录标志*/
	ST_SrvCommInfo stSrvCommInfo;   /* 服务器通信信息*/
}ST_DevLoginInfo;

typedef struct SOrderedStream
{
	int lStreamID;  /* 所指定的流ID*/
}ST_OrderedStream;

/*SOS状态*/
typedef struct SSosStatus
{
	int lSosIsOpen; /* SOS是否开启*/
}ST_SosStatus;

/*服务器IP端口信息*/
typedef struct SServerIPPort
{
	unsigned int lSrvPort;          /* 服务器端口*/
	char szSrvIP[MAX_LEN_IP_STR];   /* 服务器IP*/
}ST_ServerIPPort;

/*服务器IP端口信息 支持域名*/
typedef struct SDevLoginServerInfo
{
	unsigned int lSrvPort;          /* 服务器端口*/
	char szSrvIpOrDomain[PATHNAME_MAX_LENGTH];   /* 服务器IP*/
}ST_DevLoginServerInfo;

/*设备绑定手机号码信息结构体*/
typedef struct SSTSmsBindTelNumInfo
{
	char szPhoneNO[MAX_LEN_PHONENUM_STR]; /* 要添加的设备激活手机号码*/
	char bIsReciveAlarm;             /*是否接收报警 0:不接收 1:接收*/
	char reserved[7];                /* 保留位*/
} ST_SmsBindTelNumInfo;

/*MSG_REQ_DEV_XXX 远程通用请求, 短信控制->设备管理模块*/
typedef struct SSTDevWorkModeSet
{
	int lType;
	int lReserved[2];               /* 预留*/
} ST_DevWorkModeSet;

/*播放的音频流信息*/
typedef struct SAudioInfo
{
	int lStreamID;  /* 流ID*/
}ST_AudioInfo;

/*链路成员状态信息*/
typedef struct SUpLinkVectorInfo                          
{
	unsigned int lObjID;
	int lIfindex;                           /* 接口ID*/
	int lWorkStandard;                      /* 制式wcdma or evdo*/
	int lOperateFlag;                       /* 发送操作对应的flag*/
	char szIfName[MAX_LEN_IFNAME_STR];      /* PPP接口名称*/
	ST_3GSysInfo st3GSysInfo;               /* 连接状态信息*/
}ST_UpLinkVectorInfo;

/*链路状态信息*/
typedef struct S3GUpLinksInfo                               
{                               
	int lCount; /* 接口个数,针对3G而言*/
	ST_UpLinkVectorInfo stUpLinkVectorInfo[MAX_SUPPORT_3G_DEV_COUNT];
}ST_3GUpLinksInfo;

/******DEL_S_by_Wangly 此部分结构体移动至yvcommon_srv_dev_data_struct.h中******/
#if 0
/*wifi状态信息*/
typedef struct SDevWifiStatusInfo
{
	int lInUse;                             /* 启用标志*/
	int lUpFlag;                            /* 连接标志*/
	int lStatus;                            /* 状态，未定义*/
	int lSignalStrength;                    /* 信号强度*/
	char szIfName[MAX_LEN_IFNAME_STR];      /* 接口名称*/
}ST_DevWifiStatusInfo;

typedef struct S3GSysInfo
{
	int lSignalStrength;    /* 信号强度 0-4 0:0 , 1:20% 2:40% 3:60% 4:80%*/
	int lConnectErrorFlag;  /* 连接错误状态0-1   0:无错误 1:连接错误*/
	int lWorkMode;          /* 系统模式0-1    0:2G   1:3G*/
	int lSimExist;          /* SIM卡是否存在状态 0-1   0:无sim卡  1：有sim卡*/
}ST_3GSysInfo;

typedef struct S3GLinkVectorStatusInfo
{
	int lInUse;                        /* 启用标志*/
	int lIfIndex;                      /* 接口ID*/
	int lUpFlag;                       /* 连接标志*/
	int lWorkStandard;                 /* 工作制式wcdma/evdo*/
	char szIfName[MAX_LEN_IFNAME_STR]; /* PPP接口名称*/
	ST_3GSysInfo st3GSysInfo;
}ST_3GLinkVectorStatusInfo;

/*3G状态信息*/
typedef struct SDev3GStatusInfo
{
	int lCount;         /* 3G模块数目*/
	ST_3GLinkVectorStatusInfo st3GLinkVectorStatusInfo[MAX_SUPPORT_3G_DEV_COUNT];
}ST_Dev3GStatusInfo;

typedef struct SDevLinkStatusInfo
{
	ST_DevWifiStatusInfo stDevWifiStatusInfo;   /* WIFI状态信息*/
	ST_Dev3GStatusInfo  stDev3GStatusInfo;      /* 3G状态信息*/
}ST_DevLinkStatusInfo;

#endif

/******DEL_S_by_Wangly 此部分结构体移动至yvcommon_srv_dev_data_struct.h中******/

/*链路状态信息*/
typedef struct SUpLinksInfo                               
{
	int lLinkType;                              /* 链路类别3G or WIFI,or 3G+WIFI*/
	ST_DevWifiStatusInfo stDevWifiStatusInfo;   /* wifi连接信息*/
	ST_3GUpLinksInfo st3GUpLinksInfo;           /* 3G连接信息,包含已连接的多个3G模块信息*/
}ST_UpLinksInfo;

/*设备温度*/
typedef struct SDevTempatureInfo
{
	int lTemprature;
}ST_DevTempatureInfo;

/*语音对讲状态信息*/
typedef struct SDevPTTStatusInfo
{
	int lPTTStatus;
}ST_DevPTTStatusInfo;

/*传输状态信息*/
typedef struct SDevTransmitStatusInfo
{
	int lStatus;    /* 传输状态 0没有传输 1低速 2中速 3高速*/
}ST_DevTransmitStatusInfo;

/*设备管理*/
typedef enum EPttStatus
{
	PTT_FREE = 0,       /*没有加入任何群*/
	PTT_ODINARY,        /*普通语音集群*/
	PTT_LOCK_OTHER,     /*普通语音集群被他人锁定*/
	PTT_LOCK_SELF,      /*普通语音集群被自己锁定*/
	PTT_LOCK_FAILED,       /*普通语音锁定失败*/
	PTT_SOS,            /*SOS集群*/
	PTT_SOS_LOCK_OTHER, /*SOS集群被他人锁定*/
	PTT_SOS_LOCK_SELF,  /*SOS集群被自己锁定*/
	PTT_SOS_LOCK_FAILED,       /*SOS语音锁定失败*/
	PTT_STS_MAX
}EN_PttStatus;          /*语音集群状态：设备端设备管理 <-->状态指示*/

/*GPS接口库定义结构体*/
typedef struct SLibGPSSetting
{
	char szGPSDevName[MAX_LEN_DEVFILENAME_STR];
	ST_GPSDevComSetting stGPSDevComSetting;
}ST_LibGPSSetting;

/*流控请求数据部结构体*/
typedef struct STrafficCtlDemand
{
	int lTrafficCtlType;/*控制类别*/
	int lAmplitude; /*幅度*/
	int lReason;    /*原因*/
	int lReserved[20]; /*预留值，以后可考虑填充入一部分当前流控细节*/
}ST_TrafficCtlDemand;
/*流控参数修改数据部结构体*/
typedef struct SStmTcParam
{
	int lAllowedRetranPercent;  /*进行流控判断时进行调整所允许的重发率*/
	int lNotAllowedRetranPercent;   /*进行流控判断时进行调整所不允许的重发率，超过该值则需要进行调整*/
	int lForcedownMinAmpWhenDrop;    /*出现丢包时，最起码要降低的百分比*/
	int lForcedownMinAmpWhenRetransmit; /*出现重发时，最起码要降低的百分比*/
	int lAllowupAmpPercent;     /*允许上调时候的每次上调幅度*/
	int lForcedwonMaxAmpPercent;    /*每次要求下降传输时，最大的下降幅度*/
	int lAjustNfyInterval;  /*每次发送调整请求的间隔，秒值*/
}ST_StmTcParam;

/***************************************************************************************/
/*以下为对内部消息的定义 */
/***************************************************************************************/
/*通用消息定义*/

/*采集压缩模块/扩展设备管理模块<-------->流媒体模块*/
/* MSG_COM_NFY_STREAM_INFO_UPDATE 流信息更新通知,采集压缩模块/扩展设备管理管理模块->流媒体模块*/
typedef struct SStreamInfoUpdateNfy
{
	ST_VectorStreamInfo stVectorStreamInfo; /* 数据部  */
}ST_StreamInfoUpdateNfy;

/* MSG_COM_NFY_ORDER_STREAM_INFO_UPDATE 要求流信息上传通知,流媒体模块->采集压缩模块/扩展设备管理管理模块*/
typedef struct SOrderStreamInfoUpdateNfy
{
	ST_OrderedStream stOrderedStream;       /* 数据部  */
} ST_OrderStreamInfoUpdateNfy;

/* 流媒体数据通知消息MSG_NFY_AVCAP_AV,音视频捕捉->流媒体模块  */
typedef struct SInterMsgStreamHdr
{
	ST_StreamHdr stStreamHdr;                           /* 数据头,流媒体头部*/
	char Content[0];                                            /*数据部,流媒体数据*/
}ST_InterMsgStreamHdr;


/*设备管理模块<-------->其他模块*/
/*MSG_COM_NFY_SOS_STATUS SOS报警状态通知 设备管理模块->其他模块*/
typedef struct SSosStatusNfy
{
	ST_SosStatus stSosStatus;       /* 数据部  */
} ST_SosStatusNfy;


/* MSG_COM_REQ_LOGIN_INFO 设备登陆信息查询请求,其他模块->设备管理模块*/
typedef struct SDevLoginInfoReq
{
	ST_DataHdr stDataHdr;           /* 数据头  */
} ST_DevLoginInfoReq;

/* MSG_COM_RES_LOGIN_INFO 返回登陆信息查询结果,设备管理模块->其他模块*/
typedef struct SDevLoginInfoRes
{
	ST_DataHdr stDataHdr;           /* 数据头   */
	ST_MsgRespRet stMsgRet;         /* 响应结果 */
	ST_DevLoginInfo stDevLoginInfo; /* 数据部  */
} ST_DevLoginInfoRes;

/*MSG_COM_REQ_DEV_QUERY_SUB_INFO 查询子设备基本信息请求*/
typedef struct SMsgComReqDevQuerySubInfo
{
	ST_DataHdr stDataHdr;           /* 数据头   */
	char reserved[8];               /*保留位*/
} ST_MsgComReqDevQuerySubInfo;

/*MSG_COM_RES_DEV_QUERY_SUB_INFO 查询子设备基本信息响应*/
typedef struct SMsgComResDevQuerySubInfo
{
	ST_DataHdr stDataHdr;
	ST_MsgRespRet stMsgRet;
	int lSubInfoCount;                          /*子设备信息列表个数(最大不超过16个)*/
	char reserved[8];                           /*保留位*/
	ST_SubDeviceInfo astSubInfo[0];             /*子设备信息列表*/
} ST_MsgComResDevQuerySubInfo;

/*MSG_COM_REQ_DEV_QUERY_SUB_DETAILS 查询子设备详细信息请求*/
typedef struct SMsgComReqDevQuerySubDetails
{
	ST_DataHdr stDataHdr;            /*数据头   */
	short nSubID;                    /*子设备编号*/
	short nReserved;                 /*保留位兼顾结构体对齐*/
	char reserved[8];                /*保留位*/
} ST_MsgComReqDevQuerySubDetails;

/*MSG_COM_RES_DEV_QUERY_SUB_DETAILS 查询子设备详细信息响应*/
typedef struct SMsgComResDevQuerySubDetails
{
	ST_DataHdr stDataHdr;            /*数据头   */
	ST_MsgRespRet stMsgRet;          /*响应结果 */
	ST_SubDeviceConfig stDetails;    /*子设备详细信息*/
	char reserved[8];                /*保留位*/
} ST_MsgComResDevQuerySubDetails;

/***************************************************************************************/
/*扩展设备管理模块消息定义*/

/*扩展设备管理模块<-------->信息采集模块*/
/*MSG_EXP_NFY_POSITIONDATA 定位信息通知,扩展设备管理模块<-------->信息采集模块*/
typedef struct SPositionDataNfy
{
	ST_PositionData stPositionData;     /*定位信息*/
} ST_PositionDataNfy; 

/*MSG_EXP_NFY_PLAY_STREAM 扩展外设通知流媒体接收需要播放的音频流,扩展设备管理模块->流媒体模块*/
typedef struct SExpPlayStreamNfy             
{
	ST_AudioInfo stAudioInfo;
}ST_ExpPlayStreamNfy; 

/***************************************************************************************/
/*设备端流媒体模块消息定义*/

/*流媒体模块<-------->采集压缩模块/扩展设备管理模块*/
/* MSG_SMP_REQ_CHANGE_VIDEO_PARAM 流媒体要求更改视频采集参数请求,流媒体模块->采集压缩模块/扩展设备管理模块 */
typedef struct SSMPChgVideoParamReq
{
	ST_DataHdr stDataHdr;         /* 数据头  */
	ST_SMPChgVideoParam stSMPChgVideoParam;     /*数据部*/
} ST_SMPChgVideoParamReq;

/* MSG_SMP_RES_CHANGE_VIDEO_PARAM 流媒体要求更改视频采集参数响应,采集压缩模块->流媒体模块/扩展设备管理模块 */
typedef struct SSMPChgVideoParamRes
{
	ST_DataHdr stDataHdr;         /* 数据头   */
	ST_MsgRespRet stMsgRet;       /* 响应结果 */
} ST_SMPChgVideoParamRes;


/***************************************************************************************/
/*设备端的设备管理模块消息定义*/

/*设备管理模块--------->所有模块*/
/* MSG_DEV_NFY_LOGIN_BRC 设备登录广播,设备管理模块->所有模块*/
typedef struct SDevLoginBrc
{
	ST_SrvCommInfo stSrvCommInfo;   /* 数据部  */
} ST_DevLoginBrc;

/* MSG_DEV_NFY_LOGOUT_BRC 设备注销广播,设备管理模块->所有模块*/
typedef struct SDevLogoutBrc
{
} ST_DevLogoutBrc;

/* MSG_DEV_NFY_SYSCONFIG_CHANGE_BRC 系统配置更改广播,设备管理模块->所有模块*/
typedef struct SDevConfigChgBrc
{
} ST_DevConfigChgBrc;

/*设备管理模块<-------->通信模块*/
/* MSG_DEV_NFY_SERVER_IPPORT_INFO 服务器信息更新通知,设备管理模块->通信模块*/
typedef struct SChgServerIPPortNfy
{
	ST_ServerIPPort stServerIPPORT;/* 数据部  */
} ST_ChgServerIPPortNfy;

/*设备管理模块<-------->流媒体模块*/
/* MSG_DEV_NFY_PTT_CREAT PTT建立通知,设备管理模块->流媒体模块*/
typedef struct SPTTCreatNfy
{
	ST_VoiceGroupInfo stPttInfo; /*集群信息*/
}ST_PTTCreatNfy;

/* MSG_DEV_NFY_PTT_DESTRUCT PTT销毁通知,设备管理模块->流媒体模块*/
typedef struct SPTTDestructNfy
{
}ST_PTTDestructNfy;

/* MSG_DEV_NFY_PTT_LOCK PTT锁定通知,设备管理模块->流媒体模块*/
typedef struct SPTTLockNfy
{
} ST_PTTLockNfy;

/* MSG_DEV_NFY_PTT_UNLOCK PTT解锁通知,设备管理模块->流媒体模块*/
typedef struct SPTTUnlockNfy
{
}ST_PTTUnlockNfy;

/*ADD_S_by_chenlg 手电筒上视频接口库与MediaServer新增消息类型定义*/

/*注册ID*/
typedef struct
{
	int lRegId;         /*注册返回的ID*/
}ST_VdcRegIdentity;

/*支持的分辨率列表*/
typedef struct
{
	unsigned short sWidth;  /*宽*/
	unsigned short sHeight; /*高*/
}ST_CameraResolutionList;
/*摄像头能力*/
typedef struct
{
	ST_CameraResolutionList stResolution[CAMERA_MAX_RESOLUTION_NUM];    /*分辨率列表*/
	char reserved[80];                          /*预留*/
}ST_CameraAbilty;
/*注册响应结构体数据部*/
typedef struct
{
	ST_VdcRegIdentity stIdentity;   /*注册标志*/
	int lCameraNum;     /*系统的摄像头数目*/
	ST_CameraAbilty stCameraAblity[AVC_MAX_CAMERA_NUM]; /*摄像头能力描述*/
	int lReserved[100];  /*预留*/
}ST_VdcSysRegRes;

/*注销响应结构体数据部*/
typedef struct
{
	ST_VdcRegIdentity stIdentity;           /*注册标志*/
}ST_VdcSysUnregReq;

/*一个摄像头上分出来的流的不同类型定义*/
typedef enum
{
	YV_VIDEO_USDFOR_RECORD, /*录像流*/
	YV_VIDEO_USDFOR_STREAM, /*实时流*/
}EN_VIDEO_USDFOR;

/*每一个视频流通道*/
typedef struct
{
	int lCameraIndex;                   /*摄像头INDEX*/
	EN_VIDEO_USDFOR enUsdfor;           /*摄像头的流的用途分类*/
}ST_VideoChannel;

/*视频采集器参数*/
typedef struct
{
	int lWidth;     /*宽度*/
	int lHeight;    /*高度*/
	int lFramerate; /*帧率*/
	int lReserved[20];  /*预留*/
}ST_VdcCapParam;

/*通道关联的视频采集参数结构*/
typedef struct
{
	ST_VideoChannel stChannel;      /*视频通道标识*/ 
	ST_VdcCapParam stParam;      /*视频采集参数*/
}ST_VdcChnCapParam;


/*参数配置类型枚举值*/
typedef enum
{
	VAR_PARAM_CONFIG_TYPE_SET = 1,   /*设置参数，即要求使用该参数进行设置*/
	VAR_PARAM_CONFIG_TYPE_GET,       /*输出参数，即由内部返回*/
}EN_PARAM_CONFIG_TYPE_T;

/*视频通道参数设置结构*/
typedef struct
{
	EN_PARAM_CONFIG_TYPE_T enType;           /*参数是输入还是传出的*/
	ST_VdcChnCapParam stChnParam;            /*通道参数*/
}ST_VdcChnCapParamConfig;

/*视频通道开始采集请求数据部*/
typedef struct
{
	ST_VdcRegIdentity stIdentity;       /*注册标志*/
	ST_VdcChnCapParamConfig stConfig;   /*视频配置*/
}ST_VdcChnCapStartReq;
/*视频通道开始采集响应数据部*/
typedef struct
{
	ST_VdcChnCapParamConfig stConfig;   /*视频配置*/
}ST_VdcChnCapStartRes;
/*视频通道结束采集请求数据部*/
typedef struct
{
	ST_VdcRegIdentity stIdentity;   /*注册标志*/
	ST_VideoChannel stVideoChannel; /*要求停止的通道标识*/
}ST_VdcChnCapStopReq;

/*视频通道视频参数设置请求数据部*/
typedef struct
{
	ST_VdcRegIdentity stIdentity;       /*注册标志*/
	ST_VdcChnCapParamConfig stConfig;   /*视频参数配置*/
}ST_VdcChnCapParamConfigReq;

/*视频通道视频参数设置响应数据部*/
typedef struct
{
	ST_VdcChnCapParamConfig stConfig;   /*视频参数配置*/
}ST_VdcChnCapParamConfigRes;

/*心跳请求数据部*/
typedef struct
{
	ST_VdcRegIdentity stIdentity;
}ST_VdcKeepAliveReq;

/*帧物理地址通知数据部*/
typedef struct
{
	ST_VideoChannel stChannel;  /*视频通道标识*/
	unsigned int lTimeStamp;    /*时间戳*/
	unsigned int YPhyAddr;      /*采集后帧的物理地址*/
	unsigned int CPhyAddr;      /*采集后帧的物理地址*/  
	int lIndex;                 /*索引*/
	int lKey;                   /*校验用关键字*/
	int lReserved[20];          /*预留*/
}ST_CapFrameAddrNfy;

/*通知释放帧通知数据部*/
typedef struct
{
	ST_VdcRegIdentity stIdentity;   /*注册标志*/
	ST_VideoChannel stChannel;      /*视频通道标识*/
	int lIndex;                     /*释放的索引*/
	int lReserved[10];              /*预留*/
}ST_CapReleaseFrameNfy;

/*ADD_E_by_chenlg 手电筒上视频接口库与MediaServer新增消息类型定义*/

/*设备管理模块<-------->音视频解码模块*/
/* MSG_DEV_NFY_START_PLAY_AUDIO 播放音频通知,设备管理模块->音视频解码模块*/
typedef struct SStartPlayAudioNfy
{
	ST_AudioInfo stAudioInfo;/*音频信息*/
}ST_StartPlayAudioNfy;

/* MSG_DEV_NFY_STOP_PLAY_AUDIO 停止播放音频通知,设备管理->音视频解码模块*/
typedef struct SStopPlayAudioNfy
{
	ST_AudioInfo stAudioInfo;/*音频信息*/
} ST_StopPlayAudioNfy;

/*设备管理模块<-------->状态指示模块*/
/*MSG_DEV_NFY_PTT_STATUS PTT状态指示通知,设备管理模块<-------->状态指示模块*/
typedef struct SDevPTTStatusNfy
{
	ST_DevPTTStatusInfo stDevPTTStatusInfo;/*PTT状态信息*/
}ST_DevPTTStatusNfy;

/*MSG_DEV_NFY_DH_KEY_BRC 密钥更新广播,设备管理->所有模块*/
typedef struct SMsgDevDHKeyBrc
{
	unsigned int lDeviceID;          /* 设备ID*/
	int lKeyLength;                  /* 密钥长度*/
	char reserved[8];                /* 保留位*/
	unsigned char byDHKey[MAX_DH_KEY_LENGTH];        /* 密钥*/
} ST_MsgDevDHKeyBrc;

/*ADD_S by liangy*/
/*A0二期新增消息定义*/
/*MSG_DEV_REQ_RSM_FILELIST 查询设备文件列表请求,设备管理->录像管理*/
typedef struct SMsgDevReqRsmFilelist
{
	ST_DataHdr stDataHdr;                   /* 数据头  */
	int lSearchType;                        /* 查询类型,按位表示:*/
	/* RECORD_SEARCH_TYPE_TIME | RECORD_SEARCH_TYPE_SIZE|RECORD_SEARCH_TYPE_FILE_TYPE*/
	unsigned int lReservedID1;              /* 预留*/
	unsigned int lReservedID2;              /* 预留*/
	unsigned int lChannelID;                /* 设备通道ID*/
	unsigned int lBeginTime;                /* 起始时间*/
	unsigned int lEndTime;                  /* 终止时间*/
	ST_UInt64 stMinSize;                    /* 最小size*/
	ST_UInt64 stMaxSize;                    /* 最大size*/
	char cFileType;                         /* 文件类型 */
	char reserved[7];                       /* 保留位*/
} ST_MsgDevReqRsmFilelist;

/*MSG_DEV_RES_RSM_FILELIST 查询设备文件列表响应,录像管理->设备管理*/
typedef struct SMsgDevResRsmFilelist
{
	ST_DataHdr stDataHdr;           /* 数据头  */
	ST_MsgRespRet stMsgRet;         /*响应结果*/
	int lTotalNum;                  /*总文件数,不超过500个*/
	char reserved[8];               /*保留位*/
	ST_RecordFile astFiles[0];      /*文件列表,不大于500个*/
} ST_MsgDevResRsmFilelist;

/*MSG_DEV_REQ_RSM_SYNC_FILE 文件同步上传请求,设备管理->录像管理*/
typedef struct SMsgDevReqRsmSyncFile
{
	ST_DataHdr stDataHdr;           /* 数据头  */
	unsigned int lSyncID;           /* 与服务器同步会话ID*/
	char szFileName[FILENAME_MAX_LENGTH];      /*文件名64*/
	char reserved[8];               /* 保留位*/
} ST_MsgDevReqRsmSyncFile;

/*MSG_DEV_RES_RSM_SYNC_FILE 文件同步上传响应,录像管理->设备管理*/
typedef struct SMsgDevResRsmSyncFile
{
	/* Mcs头部 */
	ST_DataHdr stDataHdr;           /* 数据头  */
	ST_MsgRespRet stMsgRet;         /* 响应结果*/
	char reserved[8];               /* 保留位*/
} ST_MsgDevResRsmSyncFile;

/*MSG_RSM_NFY_SYNC_FILE_STATUS 文件同步上传状态进度通知,设备管理->来源模块*/
typedef struct SMsgRsmNfySyncFileStatus
{
	/*Mcs头部*/
	unsigned int lSyncID;                   /*同步会话*/
	ST_UInt64 stFileSize;                   /*文件大小*/
	unsigned int lBlockSize;                /*分块大小*/
	int lTotalBlock;                        /*文件总块数*/
	int lSuccessCount;                      /*分块上传完成统计*/
	char szCurFilename[FILENAME_MAX_LENGTH];/*当前上传文件名称*/
	char cCurFileStatus;                    /*当前上传文件进度:-1:失败;0-99:正在上传;100:完成;*/
	char reserved[7];                       /*保留位*/
} ST_MsgRsmNfySyncFileStatus;
/*ADD_E by liangy*/

/*ADD_S by ful*/
/*MSG_REQ_DEV_START 远程开机请求,设备进入正常模式 短信控制->设备管理模块*/
typedef struct SDevStartReq
{
	/* Mcs头部*/
	ST_DataHdr stDataHdr;           /* 数据头*/
	int lReserved[2];               /* 预留*/
} ST_DevStartReq;

/*MSG_RES_DEV_START 远程开机响应, 设备管理模块->短信控制*/
typedef struct SDevStartRes
{
	/* Mcs头部*/
	ST_DataHdr stDataHdr;           /* 数据头*/
	ST_MsgRespRet stMsgRes;         /* 响应结果*/
} ST_DevStartRes;

/*MSG_REQ_DEV_STOP 远程关机请求,设备进入低功耗模式 短信控制->设备管理模块*/
typedef struct SDevStopReq
{
	/* Mcs头部*/
	ST_DataHdr stDataHdr;           /* 数据头*/
	int lReserved[2];               /*预留*/
} ST_DevStopReq;

/*MSG_RES_DEV_STOP 远程关机响应, 设备管理模块->短信控制*/
typedef struct SDevStopRes
{
	/* Mcs头部*/
	ST_DataHdr stDataHdr;           /* 数据头*/
	ST_MsgRespRet stMsgRes;         /* 响应结果*/
} ST_DevStopRes;

/*MSG_REQ_DEV_REBOOT 设备重启, 短信控制->设备管理模块*/
typedef struct SDevRebootReq
{
	/* Mcs头部*/
	ST_DataHdr stDataHdr;           /* 数据头*/
	int lReserved[2];               /*预留*/
} ST_DevRebootReq;

/*MSG_RES_DEV_REBOOT 设备重启响应, 设备管理模块->短信控制*/
typedef struct SDevRebootRes
{
	/* Mcs头部*/
	ST_DataHdr stDataHdr;           /* 数据头*/
	ST_MsgRespRet stMsgRes;         /* 响应结果*/
} ST_DevRebootRes;


/*MSG_REQ_SET_DEV_NET_INFO 设置网络参数请求, 短信控制->设备管理模块*/
typedef struct SDevSetNetInfoReq
{
	/* Mcs头部*/
	ST_DataHdr stDataHdr;           /* 数据头*/
	int lNetMode;                   /* 网络模式 0:3G,1:WIFI */
	int lNetSts;                    /* 网络状态 0:关闭,1:打开 */
	char szNetParam1[32];           /* 网络参数1:WIFI的SSID */
	char szNetParam2[32];           /* 网络参数2:WIFI是否加密,"YES""NO" */
	char szNetParam3[32];           /* 网络参数3:WIFI的密码 */
	char szNetParam4[32];           /* 网络参数4:预留 */
	char szNetParam5[32];           /* 网络参数5:预留 */
} ST_DevSetNetInfoReq;

/*MSG_RES_SET_DEV_NET_INFO 设置网络参数响应, 设备管理模块->短信控制*/
typedef struct SDevSetNetInfoRes
{
	/* Mcs头部*/
	ST_DataHdr stDataHdr;           /* 数据头*/
	ST_MsgRespRet stMsgRes;         /* 响应结果*/
} ST_DevSetNetInfoRes;


/*ADD_E by ful*/


/*ADD_S by chenlg*/

/*视频切换消息数据部*/   
typedef struct
{
	int lSwitchInputType;   /*视频输入类型，SDI,HDMI,CVBS ... */
	char reserved[20];      /*预留*/
}ST_DevVideoInputSwitch;
/*MSG_REQ_DEV_VIDEOINPUT_SWITCH 视频输入源切换请求, 设备管理模块->安卓界面模块*/
typedef struct
{
	/* Mcs头部*/
	ST_DataHdr stDataHdr;           /* 数据头*/
	ST_DevVideoInputSwitch stSwitch;    /* 切换结构体*/
}ST_DevVideoInputSwitchReq;
/*MSG_RES_DEV_VIDEOINPUT_SWITCH 视频输入源切换请求, 安卓界面模块->设备管理模块*/
typedef struct
{
	/* Mcs头部*/
	ST_DataHdr stDataHdr;           /* 数据头*/
	ST_MsgRespRet stMsgRet;         /* 响应结果*/
}ST_DevVideoInputSwitchRes;

/*ADD_E by chenlg*/

/*ADD_S by louzf 2014.11.21*/
/*事件触发录像*/
typedef struct
{
	int lTimingRecordFlag;			/*是否定时录像标志*/
	int lTimingRecordStyle;			/*定时录像样式，按星期/按月*/
	int lTimingEveryDay[32];		/*每天的定时时间，每一个Int的前24位表示一天的24个小时，1为定时，0为不定时,比如样式为星期时，前七个int表示一星期的七天，为月时，前28-
									  31天为这个月的每天,每个int的32位中的24位 表示24个小时*/
}ST_DevTimingRecord;

typedef struct
{
	int  lRecordInUseFlag;
	int  lStreamRecordSwitch;	         /*全局录像开关*/
	int  lRecordFileMaxTime;	 /*录像文件最长时间*/
	int  lRecordFileMaxSize;	 /*录像文件最大空间*/
	int  lRecordFileOverlagTime;	/*相邻录像文件重叠时间,0 ~ 20 秒有效*/
	int  lRecordDiscCleantype;	/*录像磁盘满自动清理最老录像0，手动清理录像1*/
	int  lRecordPermitMinResStorage;		/*允许系统保留的最小空间 MB，默认200M*/
	char szRecordPath[PATHNAME_MAX_LENGTH];	        /*录像存储目录*/
	ST_DevTimingRecord stDevTimingRecord;
	char reserved[512];
}ST_DevRecordCtrl;



/*1.录像配置请求(设备管理->流媒体)  MSG_REQ_DEV_RECORD_CTRL_SET */
typedef struct
{
	/* Mcs头部*/
	ST_DataHdr stDataHdr;           /* 数据头*/
	ST_DevRecordCtrl stCtrl;    		/* 配置结构体*/
}ST_DevRecordCtrlSetReq;


/*2.录像配置请求回应(流媒体->设备管理)  MSG_RES_DEV_RECORD_CTRL_SET */
typedef struct
{
	/* Mcs头部*/
	ST_DataHdr stDataHdr;           /* 数据头*/
	ST_MsgRespRet stMsgRet;         /* 响应结果*/
}ST_DevRecordCtrlSetRes;

/*ADD_E by louzf 2014.11.21*/


/***************************************************************************************/
/*设备端网络管理模块消息定义*/

/*网络管理模块<-------->状态指示模块*/
/*MSG_NMG_NFY_LINK_STATUS 网络连接状态通知,网络管理模块<-------->状态指示模块 信息采集模块*/
typedef struct SDevLinkStatusNfy
{
	/*mcs头部*/
	ST_DevLinkStatusInfo stDevLinkStatusInfo;/*wifi状态*/
} ST_DevLinkStatusNfy;

/*网络管理模块<-------->通信模块*/
/*MSG_NMG_NFY_UPLINK_INFO 网络通道信息通知,网络管理模块<-------->通信模块*/
typedef struct SUpLinksInfoNfy
{

	ST_UpLinksInfo stUpLinksInfo;
}ST_UpLinksInfoNfy;

/***************************************************************************************/
/*设备端下载回放模块消息定义*/
/*TODO*/

/***************************************************************************************/
/*设备端状态指示模块消息定义*/

/*通信模块<-------->状态指示模块*/
/*MSG_STM_NFY_TRANSMIT_STATUS 传输状态指示通知,通信模块<-------->状态指示模块*/
typedef struct SDevTransmitStatusInfoNfy
{
	/*mcs头部*/
	ST_DevTransmitStatusInfo stDevTransmitStatusInfo;/*传输状态*/
}ST_DevTransmitStatusInfoNfy;

/*通信策略查询/设置*/
typedef struct
{
	char szPolicyDesc[PATHNAME_MAX_LENGTH];    /*策略描述*/
}ST_StmPolicyDesc;  
typedef struct 
{
	ST_StmPolicyDesc stPolicy;          /*策略描述*/
	int lSwitchFlag;                    /*切换标志，如是否指定session*/
	int lMatchedSessionID;              /*指定session设置的话，对应的sessionID*/
	int lMatchedPeerID;                 /*指定PeerID设置的话，对应的peerID*/
	int lReserved[8];                   /*预留*/
}ST_StmPolicySwitch;

typedef struct
{
	int lQueryFlag;                     /*查询标志，如是否指定session查询*/
	int lMatchedSessionID;              /*指定session设置的话，对应的sessionID*/
	int lMatchedPeerID;                 /*指定PeerID设置的话，对应的peerID*/
	int lReserved[8];                   /*预留*/
}ST_PolicyQuery;
/*策略查询请求,MSG_STM_REQ_POLICY_QUERY 其他模块<-------->通信模块*/
typedef struct
{
	/*Mcs头部*/
	ST_DataHdr stDataHdr;               /*数据头*/
	ST_PolicyQuery  stQuery;            /*策略查询*/
}ST_StmPolicyQueryReq;
/*策略查询响应, MSG_STM_RES_POLICY_QUERY 通信模块<-------->其他模块*/
typedef struct
{
	/*Mcs头部*/
	ST_DataHdr stDataHdr;               /*数据头*/
	ST_MsgRespRet stMsgRet;             /*响应结果*/
	ST_StmPolicyDesc stPolicy;          /*策略描述  */
}ST_StmPolicyQueryRes;
/*策略设置请求, MSG_STM_REQ_POLICY_SWITCH 其他模块<-------->通信模块*/
typedef struct
{
	/*Mcs头部*/
	ST_DataHdr stDataHdr;               /*数据头*/
	ST_StmPolicySwitch stPolicySwitch;  /*策略切换结构体*/
}ST_StmPolicySwitchReq;
/*策略设置响应, MSG_STM_RES_POLICY_SWITCH 通信模块<-------->其他模块*/
typedef struct
{
	/*Mcs头部*/
	ST_DataHdr stDataHdr;               /*数据头*/
	ST_MsgRespRet stMsgRet;             /*响应结果*/
}ST_StmPolicySwitchRes;


/***************************************************************************************/
/*设备端信息采集监控模块消息定义*/

/*信息采集监控模块<-------->设备管理模块*/
/* MSG_STS_NFY_SOS_ALARM SOS触发通知,信息采集监控模块->设备管理模块*/
typedef struct SStsSOSAlarmstartNfy
{
	/* Mcs头部 */
	ST_DevSOSInfo stSOSInfo;/*SOS信息数据部*/
}ST_StsSOSAlarmStartNfy;

/* MSG_STS_NFY_SOSSTOP_ALARM SOS停止通知,信息采集监控模块->设备管理模块 */
typedef struct SStsSOSAlarmStopNfy
{
	/* Mcs头部 */
}ST_StsSOSAlarmStopNfy;

/* MSG_STS_NFY_LOCK_PTT PTT锁定按键触发通知,信息采集监控模块->设备管理模块*/
typedef struct SStsLockPTTNfy
{
	/* Mcs头部 */
}ST_StsLockPTTNfy;

/* MSG_STS_NFY_UNLOCK_PTT PTT解锁按键触发通知,信息采集监控模块->设备管理模块 */
typedef struct SStsUnlockPTTNfy
{
	/* Mcs头部 */
}ST_StsPTTUnlockNfy;

/*信息采集模块<-------->网络管理模块*/
/*MSG_STS_NFY_MODE_WIFI WIFI模式通知,信息采集模块<-------->网络管理模块*/
typedef struct SStsWorkModeWifiNfy
{
	/* Mcs头部 */
}ST_StsWorkModeWifiNfy;

/*MSG_STS_NFY_MODE_3G 3G模式通知,信息采集模块<-------->网络管理模块*/
typedef struct SStsWorkMode3GNfy
{
	/* Mcs头部 */
}ST_StsWorkMode3GNfy;

/*STS_NFY_SWITCH_STREAM 信息采集模块<-------->扩展外设*/
typedef struct SStsSwitchNfy                
{               
	/*mcs头部*/                                                          
}ST_StsSwitchNfy;

/*STS_NFY_SWITCH_VOLUME 信息采集模块<-------->解码模块*/
typedef struct SStsSwitchVolumeNfy
{
	/*mcs头部*/
	unsigned int lVolume;           /*音量百分比*/
}ST_StsSwitchVolumeNfy;

/*信息采集监控模块<-------->状态指示模块*/
/*MSG_STS_NFY_BATTERAY_STATUS 电池电量状态指示通知,信息采集监控模块<-------->状态指示模块*/
typedef struct SDevBatterayStatusNfy
{
	/*mcs头部*/
	ST_DevBatteryInfo stDevBatteryInfo; /*电池电量*/
}ST_DevBatterayStatusNfy;

/*MSG_STS_NFY_TEMPERATURE_STATUS 设备温度状态指示通知,信息采集监控模块<-------->状态指示模块*/
typedef struct SDevTempatureStatusNfy
{
	/*mcs头部*/
	ST_DevTempatureInfo stDevTempatureInfo; /*设备温度*/
}ST_DevTempatureStatusNfy;

/*MSG_STM_REQ_TRAFFIC_CONTROL  流量控制请求 通信模块---------->流媒体模块*/
typedef struct SDevTrafficControlReq
{
	/*mcs头部*/
	ST_DataHdr stDataHdr;         /* 数据头   */
	ST_TrafficCtlDemand stTrafficCtlDemand;       /* 流控请求 */
}ST_DevTrafficControlReq;

/*MSG_STM_RES_TRAFFIC_CONTROL  流量控制请求 通信模块---------->流媒体模块*/
typedef struct SDevTrafficControlRes
{
	/*mcs头部*/
	ST_DataHdr stDataHdr;         /* 数据头   */
	ST_MsgRespRet stMsgRet;       /* 响应结果 */
}ST_DevTrafficControlRes;

/*MSG_STM_NFY_TC_PARAM  更改流量控制参数*/
typedef struct SStmTcParamNfy
{
	/*mcs头部*/
	ST_StmTcParam stStmTcParam;
}ST_StmTcParamNfy;

/*ADD_S_By_WANGLY VPN二期新增消息定义*/
/*链路传输状态通知消息 MSG_STM_NFY_LINKTRANSIMIT_STATUS 通信模块->信息采集 STM->STS*/
typedef struct
{
	/*mcs头部*/
	int lCount;
	ST_LinkTransmitInfo astLinkTransmitInfo[MAX_SUPPORT_3G_DEV_COUNT]; /*设备链路传输信息*/
} ST_StmNfyChannelStatus;

/*网卡数据传输统计消息 MSG_VPN_NFY_NIC_TRANSMIT_STATUS 网络管理模块->信息采集模块 NMG->STS*/
typedef struct
{
	/*mcs头部*/
	ST_VpnNicTransmitInfo stVpnNicTransmitInfo; /*网卡数据传输统计信息*/
} ST_VpnNicTransmitStatus;

/*设备传输数据统计状态通知消息 MSG_VPN_NFY_DEV_TRANSMIT_STATUS VPN模块->信息采集模块*/
typedef struct
{
	/*mcs头部*/
	ST_VpnDevTransmitInfo stVpnDevTransmitInfo; /*设备数据传输统计信息*/
} ST_VpnNfy;
/*ADD_E_By_WANGLY VPN二期新增消息定义*/

/*删除会话通知MSG_DEV_NFY_DEL_SESSION，所有模块->通信模块*/
typedef struct SDevDelSessionNfy
{
	yv_InterCommHdr stInterCommHdr;     /*业务头*/
	ST_DevComSession stDevComSession;   /* 数据部  */
} ST_DevDelSessionNfy;
/***************************************************************************************/
/*VPN模块消息结构定义*/
/*查询设备在线状态请求 MSG_DEV_VPN_REQ_ONLINE_STATE*/
typedef struct
{
	/* Mcs头部*/
	ST_DataHdr stDataHdr;        /* 数据头*/
} ST_MsgDevOnlineStateReq;

/*查询设备在线状态响应 MSG_DEV_VPN_RES_ONLINE_STATE*/
typedef struct
{
	/* Mcs头部*/
	ST_DataHdr stDataHdr;        /* 数据头*/
	ST_MsgRespRet stMsgRet;      /* 响应结果*/
	ST_SrvCommInfo stSrvCommInfo;   /* 数据部  */
} ST_MsgDevOnlineStateRes;

/*查询密钥请求 MSG_DEV_VPN_REQ_AES_KEY*/
typedef struct
{
	/* Mcs头部*/
	ST_DataHdr stDataHdr;        /* 数据头*/
	unsigned int lDeviceID;      /* 设备ID*/
} ST_MsgDevAesKeyReq;

/*查询密钥响应 MSG_DEV_VPN_RES_AES_KEY*/
typedef struct
{
	/* Mcs头部*/
	ST_DataHdr stDataHdr;        /* 数据头*/
	ST_MsgRespRet stMsgRet;      /* 响应结果*/
	int lAesKeyLength;           /* Aes密钥长度*/
	char byAesKey[0];            /* Aes密钥*/
} ST_MsgDevAesKeyRes;

/***************************************************************************************/
/*ADD_S_by_chenlg 手电筒上视频接口库与MediaServer新增消息类型定义*/


/*MSG_DEV_VDC_REQ_SYS_REG 视频采集系统注册请求,VDC库->MediaServer*/
typedef struct
{
	/* Mcs头部*/
	ST_DataHdr stDataHdr;        /* 数据头*/
}ST_MsgVdcSysRegReq;

/*MSG_DEV_VDC_RES_SYS_REG 视频采集系统注册响应,MediaServer->VDC库*/
typedef struct
{
	/* Mcs头部*/
	ST_DataHdr stDataHdr;        /* 数据头*/
	ST_MsgRespRet stMsgRet;      /* 响应结果*/
	ST_VdcSysRegRes stRes;       /*响应数据部*/
}ST_MsgVdcSysRegRes;

/*MSG_DEV_VDC_REQ_SYS_UNREG 视频采集系统注销请求,VDC库->MediaServer*/
typedef struct
{
	/* Mcs头部*/
	ST_DataHdr stDataHdr;        /* 数据头*/
	ST_VdcSysUnregReq stReq;/*注销请求*/
}ST_MsgVdcSysUnregReq;

/*MSG_DEV_VDC_RES_SYS_UNREG 视频采集系统注销响应,MediaServer->VDC库*/
typedef struct
{
	/* Mcs头部*/
	ST_DataHdr stDataHdr;        /* 数据头*/
	ST_MsgRespRet stMsgRet;      /* 响应结果*/
}ST_MsgVdcSysUnregRes;

/*MSG_DEV_VDC_REQ_CHN_CAP_START 视频通道启动采集请求,VDC库->MediaServer*/
typedef struct
{
	/* Mcs头部*/
	ST_DataHdr stDataHdr;        /* 数据头*/
	ST_VdcChnCapStartReq stReq; /*要求的参数配置*/
}ST_MsgVdcChnCapStartReq;

/*MSG_DEV_VDC_RES_CHN_CAP_START  视频通道启动采集响应,MediaServer->VDC库*/
typedef struct
{
	/* Mcs头部*/
	ST_DataHdr stDataHdr;        /* 数据头*/
	ST_MsgRespRet stMsgRet;      /* 响应结果*/
	ST_VdcChnCapStartRes stRes;  /* 数据部*/
}ST_MsgVdcChnCapStartRes;

/*MSG_DEV_VDC_REQ_CHN_CAP_STOP  视频通道停止采集请求,VDC库->MediaServer*/
typedef struct
{
	/* Mcs头部*/
	ST_DataHdr stDataHdr;        /* 数据头*/
	ST_VdcChnCapStopReq stReq;   /* 数据部*/
}ST_MsgVdcChnCapStopReq;

/*MSG_DEV_VDC_REQ_CHN_CAP_STOP  视频通道停止采集响应,MediaServer->VDC库*/
typedef struct
{
	/* Mcs头部*/
	ST_DataHdr stDataHdr;        /* 数据头*/
	ST_MsgRespRet stMsgRet;      /* 响应结果*/
}ST_MsgVdcChnCapStopRes;

/*MSG_DEV_VDC_REQ_CHN_CAPPARAM_SET 视频通道设置采集参数请求,VDC库->MediaServer*/
typedef struct
{
	/* Mcs头部*/
	ST_DataHdr stDataHdr;        /* 数据头*/
	ST_VdcChnCapParamConfigReq stReq; /*数据部*/
}ST_MsgVdcChnCapParamConfigReq;

/*MSG_DEV_VDC_RES_CHN_CAPPARAM_SET 视频通道设置采集参数响应,MediaServer->VDC库*/
typedef struct
{
	/* Mcs头部*/
	ST_DataHdr stDataHdr;        /* 数据头*/
	ST_MsgRespRet stMsgRet;      /* 响应结果*/
	ST_VdcChnCapParam stParam;   /*实际启动的参数，如果是成功，需要解析该内容*/
}ST_MsgVdcChnCapParamConfigRes;

/*MSG_DEV_VDC_REQ_KEEPALIVE 视频系统保活请求,VDC库->MediaServer*/
typedef struct
{
	/* Mcs头部*/
	ST_DataHdr stDataHdr;        /* 数据头*/
	ST_VdcKeepAliveReq stReq;    /* 数据部*/
}ST_MsgVdcKeepAliveReq;

/*MSG_DEV_VDC_RES_KEEPALIVE 视频系统保活响应,MediaServer->VDC库*/
typedef struct
{
	/* Mcs头部*/
	ST_DataHdr stDataHdr;        /* 数据头*/
	ST_MsgRespRet stMsgRet;      /* 响应结果*/
}ST_MsgVdcKeepAliveRes;

/*MSG_DEV_VDC_NFY_CHN_FRAMEADDR 视频通道帧物理地址通知,MediaServer->VDC库*/
typedef struct
{
	/* Mcs头部*/
	ST_CapFrameAddrNfy  stNfy;   /*物理地址通知数据部*/
}ST_MsgVdcFrameAddrNfy;

/*MSG_DEV_VDC_NFY_CHN_RELEASEFRAME 视频通道帧资源释放通知,VDC库->MediaServer*/
typedef struct
{
	/* Mcs头部*/
	ST_CapReleaseFrameNfy  stNfy;   /*物理地址通知数据部*/
}ST_MsgVdcReleaseFrameNfy;
/*ADD_E_by_chenlg 手电筒上视频接口库与MediaServer新增消息类型定义*/
/***************************************************************************************/
/*ADD_S_by_chenlg HDMI新增消息类型定义*/
/*IP地址配置*/
typedef struct
{
	int lAddrProto;                 /* 网卡地址类型:见枚举NIC_ADD_PROTO_XXX*/
	char szStaticIp[20];            /*静态IP地址*/
	char szStaticMask[20];          /*静态掩码*/
	char szStaticGwIp[20];          /*静态网关地址*/
	char szStaticDns[20];           /*静态dns*/
}ST_NicAddrCfg;
/*ADD_S by jiangc 2017/04/18*/
/*GPS定位配置信息 保存扩展定位信息的保留位中*/
typedef struct {
	unsigned int lWorkMode;              /*工作模式 0代表无效位 1 连续定位   2 单次定位  3 定时定位  4 关闭定位*/
	unsigned int lTimeOutHour;           /*定时定位的小时数   (单位 时)*/
	unsigned int lTimeOutMinute;         /*定时定位的分钟数   (单位 分)*/
}ST_GpsConfig;
/*ADD_E by jiangc*/
/*扩展的定位信息结构体（GPS信息）*/
typedef struct
{
	int lGpsSwitchStatus;           /*GPS开关状态 1：打开，0 关闭*/
	int lUtcTimeValidity;           /*utc时间有效性*/
	int lPositionValidity;          /*定位数据有效性*/
	ST_PositionData stPositionData; /*老的定位数据信息*/
	ST_GpsConfig stGpsConfigInFo;   /*GPS配置信息*/
	char world[20];                 /*保留字节*/
}ST_PositionDataEx;


/*流配置*/
typedef struct
{
	short nSubID;          /*子设备编号(采集实时流编号StreamID=SubID)*/
	short nSubType;        /*子设备类型(预留)*/
	short nStreamType;     /*采集或输出流类型:见枚举STREAM_TYPE_XXX*/
	short nReserved;       /*预留参数,兼顾对齐*/
	int lUsdfor;            /*用途,见枚举EN_VIDEO_USDFOR*/
	union 
	{
		struct
		{
			int lVideoChgMask;      /*修改类型的掩码，见枚举VIDEO_PARAM_CHG_XXX*/
			ST_VideoParam stVideoParam;
			char reserved[128];
		}stVideoChg;
		struct
		{
			int lAudioChgMask;      /*修改类型的掩码，见枚举AUDIO_PARAM_CHG_XXX*/
			ST_AudioParam stAudioParam;
			char reserved[128];
		}stAudioChg;
		struct
		{
			int lGpChgMask;         /*修改类型的掩码*/
			ST_GPDParam stGPDParam;
			char reserved[128];
		}stGpdChg;
	};
}ST_StreamCfgSet;
/*采集系统控制结构体*/
typedef struct 
{
	int lCapSystemType;         /*需要重新初始化的采集系统类型,见枚举CAP_SYS_TYPE_XXX*/
	int lCtrlCmdType;           /*控制类别, 见枚举 CMD_CTRL_CAP_SYS_XXX*/    
	char reserverd[128];        /*预留*/
}ST_CapSysCtrl;
/*子设备流信息*/
typedef struct 
{
	short nSubID;          /*子设备编号(采集实时流编号StreamID=SubID)*/
	short nSubType;        /*子设备类型(预留)*/
	short nStreamType;     /*采集或输出流类型:见枚举STREAM_TYPE_XXX*/
	short nReserved;       /*预留参数,兼顾对齐*/
	union 
	{
		ST_VideoParam stVideoParam; /*视频参数*/
		ST_AudioParam stAudioParam; /*音频参数*/
		ST_GPDParam stGPDParam;     /*通用类型参数*/
	};
}ST_SubDevStreamInfo;
/*上传流单元信息*/
typedef struct
{
	int lUploadType;                        /*0:未上传,1:主动上传,2:被要求上传*/
	ST_SubDevStreamInfo stSubDevStreamInfo; /*子设备类型流信息*/
	char reserved[8];                       /* 保留位*/
}ST_UploadSubDevStream;
/*上传流状态数据部*/
typedef struct
{
	int lUploadSwitchStatue;                        /*总上传流开关*/
	int lUploadStreamCount;                         /*所有允许上传的流数目*/
	ST_UploadSubDevStream stUploadSubDevStream[0];  /*每个支持上传的流成员信息*/
}ST_UploadStreamStatus;
/*录像流单元信息*/
typedef struct
{
	ST_SubDevStreamInfo stSubDevStreamInfo; /*子设备类型流信息*/
	char reserved[8];                       /* 保留位*/
}ST_RecordSubDevStreamStatus;
/*全局录像信息*/
typedef struct
{
	int lRecordSwitchStatus;                    /*录像总开关状态*/
	int lStartRecordStatus;                     /*是否已经处于已经可以录像状态*/
	int lRecordPathValidity;                    /*录像路径有效性 0：无效 ，1：有效*/
	int lMaxRecordTime;                         /*录像时间，单位为秒*/
	char szRecordPath[PATHNAME_MAX_LENGTH];     /*本地录像文件目录*/
	char reserved[256];                         /*预留*/  
}ST_DevRecordInfo;
/*录像流状态数据部*/
typedef struct
{
	ST_DevRecordInfo    stDevRecordInfo;                /*设备录像信息*/
	int lRecordStreamCount;                             /*目前配置为录像的流数目*/
	ST_RecordSubDevStreamStatus stRecordSubDevStream[0];/*目前支持录像的流的信息*/
}ST_DevRecordStatus;
/*有线网络连接信息*/
typedef struct
{
	int lInUse;                             /* 启用标志*/
	int lUpFlag;                            /* 连接标志*/
	int lStatus;                            /* 状态，未定义*/
	char szIfName[MAX_LEN_IFNAME_STR];      /* 接口名称*/
}ST_DevWiredNetStatus;

/******************************设备基本的状态结构体 add by louzf 2015.03.14*****************/

/*ADD_S by louzf*/
/*设备配置查询/设置请求响应*/



/*设备配置查询请求MSG_DEV_REQ_INTERNAL_PARAM_QUERY*/
typedef struct
{
	/* Mcs头部*/
	ST_DataHdr stDataHdr;           /* 数据头*/
	ST_DevConfigQuery stQuery;      /* 数据部*/
}ST_DevInternalConfigQueryReq;

/*设备配置查询响应MSG_DEV_RES_INTERNAL_PARAM_QUERY*/
typedef struct
{
	/* Mcs头部*/
	ST_DataHdr stDataHdr;           /* 数据头*/
	ST_MsgRespRet stMsgRet;         /* 响应结果*/
	char content[0];                /*TLV形式，后面跟随一项或多项TLV结构的配置*/
}ST_DevInternalConfigQueryRes;


/*设备配置设置请求MSG_DEV_REQ_INTERNAL_PARAM_SET*/
typedef struct
{
	/* Mcs头部*/
	ST_DataHdr stDataHdr;           /* 数据头*/
	ST_DevConfigSet stSet;          /* 数据部*/
}ST_DevInternalConfigSetReq;

/*设备配置设置响应MSG_DEV_RES_INTERNAL_PARAM_SET*/
typedef struct
{
	/* Mcs头部*/
	ST_DataHdr stDataHdr;           /* 数据头*/
	ST_MsgRespRet stMsgRet;         /* 响应结果*/
}ST_DevInternalConfigSetRes;


/*ADD_E by louzf*/



/*设备基本状态信息结构体  ADD by louzf 2015.03.14*/
/*设备电池电量扩展结构体*/
typedef struct
{
	int lBattery;   /* 电池电量百分比*/
	int lVotage;    /*电压值，经过*100换算，如301表示3.01伏*/
	int lStatus;    /*0:未充电 1:充电  2:电满*/
	char reserved[20];
}ST_DevBatteryInfoEx;
typedef struct
{
	ST_DevBatteryInfoEx stDevBatteryInfoEx;  /*电池信息*/
	int lCpuTemperature;            /* cpu温度*/
	int lCpuOccupy;                 /* cpu使用率*/
	int lMemTotal;			    		/*内存大小*/
	int lMemFree;              /* 内存剩余，单位MB*/
	int lSysStoryAll;		    		/*系统磁盘总量*/
	int lSysStoryFree;       			/*剩余磁盘空间 MB*/
	int lRecordStoryAll;               /*录像磁盘总量*/
	int lRecordStoryFree;	   /*录像磁盘剩余空间*/
	unsigned int lRunTime;	   /* 运行时长,  x64 兼容, long -> int.MOD by zhengsw. */
	ST_PositionDataEx stPositionDataEx;  /*GPS扩展信息*/
	int lWorkMode;                  /*当前工作模式 0:正常工作模式   1:本地模式  2:*休眠模式 3:静默模式*/
	char reserved[124];
}ST_DevBasicStatusInfo;

/*录像基本状态*/
typedef struct
{
	int lRecordSwitchStatus;                    /*录像总开关状态*/
	int lStartRecordStatus;                     /*是否已经处于已经可以录像状态*/
	int lRecordStatus;				/*录像目前状态0:未录像 1.正在录像*/
	int lRecordPathValidity;                    /*录像路径有效性 0：无效 ，1：有效*/
	char reserved[32];
}ST_DevRecordBasicStatus;

/*MSG_STS_NFY_DEV_BASIC_STATUS  设备基本状态信息通知 状态监控->设备管理*/
typedef struct
{
	/* Mcs头部 */
	ST_DevBasicStatusInfo stDevBasicStatusInfo;     /*设备基本信息*/
}ST_DevBasicStatusNfy;

/*MSG_SMP_NFY_RECORD_BASIC_STATUS 录像基本状态信息通知 流媒体->设备管理*/
typedef struct
{
	/* Mcs头部 */
	ST_DevRecordBasicStatus stRecordBasicStatus;     /*设备基本信息*/
}ST_DevRecordBasicStatusNfy;
/*ADD_E by louzf 2015.03.14*/


/*MSG_COM_REQ_WIRED_NET_CFG 有线网络配置修改请求*/
typedef struct
{
	/*mcs消息头*/
	ST_DataHdr stDataHdr;            /* 数据头   */
	ST_NicAddrCfg stAddrCfg;         /*IP地址配置*/
	char reserved[64];                /* 保留位*/
}ST_DevWiredNetCfgReq;
/*MSG_COM_RES_WIRED_NET_CFG 有线网络配置修改响应*/
typedef struct
{
	/*mcs消息头*/
	ST_DataHdr stDataHdr;            /* 数据头   */
	ST_MsgRespRet stMsgRet;          /* 响应结果 */
	char reserved[8];                /* 保留位*/
}ST_DevWiredNetCfgRes;
/*MSG_COM_REQ_WIFI_NET_CFG  wifi配置修改请求*/
typedef struct
{
	/*mcs消息头*/
	ST_DataHdr stDataHdr;            /* 数据头   */
	int lOperateType;                /*操作类型,本期只需支持0,以后可扩展0：set, 1：add, 2：del*/
	char szSSID[32];                /* 网络参数1:WIFI的SSID */
	char szAuthType[32];            /* 网络参数2:认证类型：见宏定义 STR_WIFI_AUTHTYPE_XXX*/
	char szPassword[32];            /* 网络参数3:WIFI的密码 */
	ST_NicAddrCfg stAddrCfg;         /*IP地址配置*/
	char reserved[64];                /* 保留位*/
}ST_DevWifiNetCfgReq;
/*MSG_COM_RES_WIFI_NET_CFG  wifi配置修改响应*/
typedef struct
{
	/*mcs消息头*/
	ST_DataHdr stDataHdr;            /* 数据头   */
	ST_MsgRespRet stMsgRet;          /* 响应结果 */
	char reserved[8];                /* 保留位*/
}ST_DevWifiNetCfgRes;
/*MSG_COM_REQ_GPS_SET GPS控制请求*/
typedef struct
{
	/*mcs消息头*/
	ST_DataHdr stDataHdr;            /* 数据头 */
	int lOperateType;                /* 1：打开， 0 关闭*/
	char reserved[32];                /* 保留位*/
}ST_DevGpsSetReq;
/*MSG_COM_RES_GPS_SET GPS控制响应*/
typedef struct
{
	/*mcs消息头*/
	ST_DataHdr stDataHdr;            /* 数据头   */
	ST_MsgRespRet stMsgRet;          /* 响应结果 */
	char reserved[8];                /* 保留位*/
}ST_DevGpsSetRes;
/*MSG_EXP_NFY_POSITIONDATA_EX扩展的GPS状态信息通知*/
typedef struct
{
	/* Mcs头部 */
	ST_PositionDataEx stPositionData;     /*定位信息*/
}ST_PositionDataExNfy;


/*MSG_COM_REQ_STREAM_CFG_SET子设备流参数配置请求*/
typedef struct
{
	/* Mcs头部 */
	ST_DataHdr stDataHdr;               /* 数据头   */
	ST_StreamCfgSet stCfg;              /* 配置数据部*/

}ST_DevSubCfgSetReq;

/*MSG_COM_RES_STREAM_CFG_SET子设备流参数配置响应*/
typedef struct
{
	/*mcs消息头*/
	ST_DataHdr stDataHdr;            /* 数据头   */
	ST_MsgRespRet stMsgRet;          /* 响应结果 */
	char reserved[8];                /* 保留位*/
}ST_DevSubCfgSetRes;

/*MSG_COM_REQ_CAP_SYS_CTRL采集系统控制请求*/
typedef struct
{
	/* Mcs头部 */
	ST_DataHdr stDataHdr;               /* 数据头   */
	ST_CapSysCtrl stCtrl;               /* 配置数据部*/
}ST_CapSysCtrlReq;
/*MSG_COM_RES_CAP_SYS_CTRL采集系统控制响应*/
typedef struct
{
	/*mcs消息头*/
	ST_DataHdr stDataHdr;            /* 数据头   */
	ST_MsgRespRet stMsgRet;          /* 响应结果 */
	char reserved[8];                /* 保留位*/
}ST_CapSysCtrlRes;

/*MSG_COM_REQ_STREAM_UPLOAD_SET流传输开关控制响应*/
typedef struct
{
	/*mcs消息头*/
	ST_DataHdr stDataHdr;            /* 数据头   */
	int lOperateType;                /* 1：打开， 0 关闭*/
	/*目前此处以下的内容都不需要解析，默认为全局开关控制*/
	int lOperateFlag;                /*控制标志：预留，以后可扩展为 0：全局控制，1：指定子设备控制*/
	short nSubID;                   /*子设备编号(采集实时流编号StreamID=SubID)*/
	short nSubType;                 /*子设备类型(预留)*/
	short nStreamType;              /*采集或输出流类型:见枚举STREAM_TYPE_XXX*/
	short nReserved;                /*预留参数,兼顾对齐*/
	char reserved[64];              /* 保留位*/
}ST_DevUploadSetReq;
/*MSG_COM_RES_STREAM_UPLOAD_SET流传输开关控制响应*/
typedef struct
{
	/*mcs消息头*/
	ST_DataHdr stDataHdr;            /* 数据头   */
	ST_MsgRespRet stMsgRet;          /* 响应结果 */
	char reserved[8];                /* 保留位*/
}ST_DevUploadSetRes;


/*MSG_COM_REQ_STREAM_RECORD_SET流录像开关控制请求*/
typedef struct
{
	/*mcs消息头*/
	ST_DataHdr stDataHdr;            /* 数据头   */
	int lOperateType;                /* 1：打开， 0 关闭*/
	/*目前此处以下的内容都不需要解析，默认为全局开关控制*/
	int lOperateFlag;                /*控制标志：预留，以后可扩展为 0：全局控制，1：指定子设备控制*/
	short nSubID;                   /*子设备编号(采集实时流编号StreamID=SubID)*/
	short nSubType;                 /*子设备类型(预留)*/
	short nStreamType;              /*采集或输出流类型:见枚举STREAM_TYPE_XXX*/
	short nReserved;                /*预留参数,兼顾对齐*/
	char reserved[64];              /* 保留位*/
}ST_DevRecordSetReq;
/*MSG_COM_RES_STREAM_RECORD_SET流录像开关控制响应*/
typedef struct
{
	/*mcs消息头*/
	ST_DataHdr stDataHdr;            /* 数据头   */
	ST_MsgRespRet stMsgRet;          /* 响应结果 */
	char reserved[8];                /* 保留位*/
}ST_DevRecordSetRes;
/*MSG_SMP_NFY_UPLOAD_STREAM_STATUS流传输状态通知*/
typedef struct
{
	/* Mcs头部 */
	ST_UploadStreamStatus stUploadStreamStatus; /*传输流信息*/
}ST_UploadStreamStatusNfy;
/*MSG_SMP_NFY_RECORD_STREAM_STATUS流录像状态通知*/
typedef struct
{
	/* Mcs头部 */
	ST_DevRecordStatus stRecordStatus;      /*录像流信息*/
}ST_DevRecordStatusNfy;

/*MSG_COM_REQ_AUDIO_INPUT_SRC音频输入类型切换请求*/
typedef struct
{
	/*mcs消息头*/
	ST_DataHdr stDataHdr;            /* 数据头   */
	int lMode;                       /*音频出入源，AUDIO_INPUT_SRC_xxx*/                 
	char reserved[4];                /* 保留位*/
}ST_DevAudioInputSrcReq;

/*MSG_COM_RES_AUDIO_INPUT_SRC音频输入类型切换响应*/
typedef struct
{
	/*mcs消息头*/
	ST_DataHdr stDataHdr;            /* 数据头   */
	ST_MsgRespRet stMsgRet;          /* 响应结果 */
	char reserved[8];                /* 保留位*/
}ST_DevAudioInputSrcRes;


/*MSG_COM_REQ_VIDEO_INPUT_SRC视频输入类型切换请求*/
typedef struct
{
	/*mcs消息头*/
	ST_DataHdr stDataHdr;            /* 数据头   */
	int lMode;                       /*视频输入源，VIDEO_INPUT_SRC_xxx*/                 
	char reserved[4];                /* 保留位*/
}ST_DevVideoInputSrcReq;

/*MSG_COM_RES_VIDEO_INPUT_SRC视频输入类型切换响应*/
typedef struct
{
	/*mcs消息头*/
	ST_DataHdr stDataHdr;            /* 数据头   */
	ST_MsgRespRet stMsgRet;          /* 响应结果 */
	char reserved[8];                /* 保留位*/
}ST_DevVideoInputSrcRes;

/*MSG_COM_NFY_DEV_SERVER_SWITCH 服务器切换通知*/
typedef struct
{
	/*mcs消息头*/
	ST_ServerIPPort stServer;        /*服务器IP端口*/
	int lSaveFlag;                   /*是否需要保存*/                 
	int reserved[4];                 /* 保留位*/
}ST_DevServerSwitchNfy;


/*MSG_COM_NFY_NET_SWITCH网络模式切换控制通知*/
typedef struct
{
	int lLinkMode;                              /*切换至的模式，有线/wifi/3G,见枚举值EN_LINKMODE_TYPE_T*/
	int lSwitchFlag;                            /*非3G模式，切换的标志，0：无 , 1:强制重新启用 3G模式，表示是启用哪张卡 0x1:卡1 0x10:卡2 0x11:双卡 见枚举EN_SIMCARD_OPENMARK_TYPE_T:*/
	char reserved[256];                         /*预留*/
}ST_DevNetSwitchNfy;

/*MSG_COM_NFY_NET_INIT_LINKMODE 网络模式初始连接模式通知*/
typedef struct
{
	int lInitLinkMode;                          /*初始化连接的模式，有线/wifi/3G,见枚举值EN_LINKMODE_TYPE_T*/
	int lUndefineFlag;									/*未定义的标志*/
	char reserved[32];                         /*预留*/
}ST_DevNetInitLinkModeNfy;
/*MSG_NMG_NFY_LINK_STATUS_EX网络连接状态通知(扩展)*/
typedef struct
{
	ST_DevLinkStatusInfo stDevLinkStatusInfo;       /*旧的只有wifi+3G的链路状态信息*/
	ST_DevWiredNetStatus stDevWiredNetStatus;    /*有线网络链路状态信息*/
	char reserved[20];                          /*预留*/  
}ST_DevLinkStatusNfyEx;
/*MSG_NMG_NFY_UPLINK_INFO_EX在线网络链路通知(扩展)*/
typedef struct
{
	ST_UpLinksInfo stUpLinkInfo;                /*旧的只有wifi+3G的在线信息*/
	ST_DevWiredNetStatus stDevWiredNetStatus;    /*有线网络连接信息*/
	char reserved[20];                          /*预留*/
}ST_UpLinksInfoNfyEx;
/*ADD_E_by_chenlg HDMI新增消息类型定义*/

/*wifi配置结构体  ADD by louzf 2015.04.08*/
typedef struct
{
	int lOperateType;                /*操作类型,本期只需支持0,以后可扩展0：set, 1：add, 2：del*/
	char szSSID[32];                /* 网络参数1:WIFI的SSID */
	char szAuthType[32];            /* 网络参数2:认证类型：见宏定义 STR_WIFI_AUTHTYPE_XXX*/
	char szPassword[32];            /* 网络参数3:WIFI的密码 */
	ST_NicAddrCfg stAddrCfg;         /*IP地址配置*/
	int llinkMode;		     /*连接方式: 0：dhcp 1: static*/
	char reserved[32];                /* 保留位*/
} ST_DevWifiNetCfg;

/*查询加密配置信息请求 MSG_COM_REQ_ENCYP_INFO*/
typedef struct
{
	ST_DataHdr stDataHdr;        /* 数据头*/
	int lEncypMode;					/*加密方式 1:数据加密 2:文件加密*/
} ST_MsgEncypInfoReq;

/*查询加密配置信息响应 MSG_COM_RES_ENCYP_INFO*/
typedef struct
{
	ST_DataHdr stDataHdr;        /* 数据头*/
	ST_MsgRespRet stMsgRet;      /* 响应结果*/
	int lEncypAth;					/*加密算法*/
	int lKeyLength;           	/* 密钥长度*/
	unsigned char byKey[128];   	/* 密钥*/
	char revert[32];					/*预留*/
} ST_MsgEncypInfoRes;
/*设备录像秘钥更新请求MSG_COM_REQ_DEV_RECORD_KEY_UPDATE*/
typedef struct
{
	ST_DataHdr stDataHdr;        /* 数据头*/
	char reserved[256];			 /*预留*/
}ST_MsgRecordKeyUpdateReq;
/*设备录像秘钥更新响应MSG_COM_RES_DEV_RECORD_KEY_UPDATE*/
typedef struct
{
	ST_DataHdr stDataHdr;        /* 数据头*/
	ST_MsgRespRet stMsgRet;      /* 响应结果*/
	char reserved[256];			 /*预留*/
}ST_MsgRecordKeyUpdateRes;

/*MSG_DEV_REQ_DEVENCRY_ABILITY_CFG 查询设备加密能力请求  设备管理模块->流媒体模块*/
/*设备加密能力响应*/
typedef struct
{
	ST_DataHdr stDataHdr;        /* 数据头*/
	char reserved[256];          /*预留*/
} ST_DevEnctyAbilityCfgReq;

/*MSG_DEV_RES_DEVENCRY_ABILITY_CFG 设备加密能力查询响应 流媒体模块->设备管理模块*/
/*设备加密能力响应*/
typedef struct
{
	ST_DataHdr stDataHdr;        /* 数据头*/
	ST_MsgRespRet stMsgRet;      /* 响应结果*/
	int lEncryAbility;        /*加密能力集*/
	char reserved[256];          /*预留*/
} ST_DevEnctyAbilityCfgRes;


/***************************************************************************************/

/***************************************************************************************/
/*音视频采集模块消息类型定义*/
/*TODO*/

/***************************************************************************************/
/*音视频解码模块消息类型定义*/
/*TODO*/

/***************************************************************************************/

/***************************************************************************************/
/*透传模式结构体定义*/
/*TODO*/
/*透传模式SIM卡结构体信息*/
typedef struct SST_SerialnetSimInfo
{
	int           IsSimCardExit;
	char          szIpAddr[MAX_LEN_IP_STR];
	char          szImsi[MAX_LEN_PHONENUM_STR];         /*卡特有的IMSI信息*/
	char          byreserved[16];
}ST_SerialnetSimInfo;

/*透传模式配置结构体*/
typedef struct SST_SerialNetModeCfgInfo
{
	int lCtrlMode; /*0:关闭 1:开启*/
	char LocalIp[20]; /*本地IP*/
	char ExternDevIp[20]; /*外接设备IP*/
	char byreserved[16];
}ST_SerialNetModeCfgInfo;

/*MSG_COM_REQ_SERIALNET_MODE_CFG 透传模式控制配置请求*/
typedef struct SST_DevSerialNetModeCfgReq
{
	ST_DataHdr stDataHdr;           /* 数据头   */
	ST_SerialNetModeCfgInfo stCtrlInfo;    /*服务器IP端口*/
}ST_DevSerialNetModeCfgReq;

/*MSG_COM_REQ_SERIALNET_MODE_CFG 透传模式控制配置响应*/
typedef struct SST_DevSerialNetModeCfgRes
{
	ST_DataHdr stDataHdr;           /* 数据头   */
	ST_MsgRespRet stMsgRet;         /* 响应结果 */
}ST_DevSerialNetModeCfgRes;

/*MSG_COM_REQ_SERIALNET_MODE_SIMCFG 透传模式查询配置请求*/
typedef struct SST_DevGetSerialNetModeSimCfgReq
{
	ST_DataHdr stDataHdr;           /* 数据头   */
	char byreserved[16];
}ST_DevGetSerialNetModeSimCfgReq;

/*MSG_COM_RES_SERIALNET_MODE_SIMCFG 透传模式查询配置响应*/
typedef struct SST_DevGetSerialNetModeSimCfgRes
{
	ST_DataHdr stDataHdr;           /* 数据头   */
	ST_MsgRespRet stMsgRet;         /* 响应结果 */
	ST_SerialnetSimInfo stSerialnetSimInfo;
}ST_DevGetSerialNetModeSimCfgRes;

/*MSG_COM_NFY_SERIALNET_SIMCFG 透传模式配置通知*/
typedef struct SST_DevSerialNetSimCfgNfy
{
	ST_SerialnetSimInfo stSerialnetSimInfo;
}ST_DevSerialNetSimCfgNfy;


/*报警标志*/
typedef struct SST_AlarmFlag
{
	int bBattery;               /*电池是否报警*/
	int bTemperature;           /*设备温度是否报警*/
	int bSystem;                /*系统剩余是否报警*/
	int bRecord;                /*录像剩余是否报警*/
	int lTempAlarmLevel;        /*温度报警级别*/
	char revert[128];
}ST_AlarmFlag;

/*MSG_STS_NFY_ALARM ALARM 内部报警通知*/
typedef struct SSTDevInterAlarmNfy
{
	ST_AlarmFlag stAlarmFlag;
}ST_DevInterAlarmNfy;


/*MSG_COM_REQ_DEVSTATUS_INFO 查询状态信息请求   内部模块->状态监控*/
typedef struct SSTDevStatusInfoReq
{
	ST_DataHdr stDataHdr;            /* 数据头   */
} ST_DevStatusInfoReq;

/*MSG_COM_RES_DEVSTATUS_INFO 查询状态信息响应  状态监控->内部模块*/
typedef struct SSTDevStatusInfoRes
{

	ST_DataHdr stDataHdr;            /* 数据头   */
	ST_MsgRespRet stMsgRes;          /* 响应结果 */
	ST_DevStatusInfo stDevStatusInfo;/*设备状态信息*/
} ST_DevStatusInfoRes;

/*MSG_COM_REQ_DEVTEMP_LEVEL_INFO 查询设备温度等级请求   内部模块->状态监控*/
typedef struct SSTDevTemptureLevelReq
{

	ST_DataHdr stDataHdr;            /* 数据头   */
} ST_DevTemptureLevelReq;

/*MSG_COM_RES_DEVTEMP_LEVEL_INFO 查询设备温度等级响应  状态监控->内部模块*/
typedef struct SSTDevTemptureLevelRes
{

	ST_DataHdr stDataHdr;            /* 数据头   */
	ST_MsgRespRet stMsgRes;          /* 响应结果 */
	int lTempLevel;
} ST_DevTemptureLevelRes;





/***************************************************************************************/

#endif
