/***************************************************************************************
 * 文 件 名 : yvcommon_data_struct.h
 * 项目名称 : YVGA1207001A
 * 模 块 名 : 集中侦控平台
 * 功    能 : 消息公共结构体的定义
 * 操作系统 : LINUX
 *--------------------------------------------------------------------------------------
 * 设    计 : liangy      '2015-09-15
 ***************************************************************************************
 *- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * 公司介绍及版权说明
 *
 *               (C)Copyright 2015 YView    Corporation All Rights Reserved.
 *- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 ***************************************************************************************/

#ifndef _YVCOMMON_DATA_STRUCT_H_
#define _YVCOMMON_DATA_STRUCT_H_

#include <time.h>
#include "yvcommon_def.h"
#define MAX_MD_AREA_NUM 4
#define MAX_SCENE_DESCRIBE_COUNT 32
/* 公共 操作用户 结构体 */
typedef struct SUser
{
	unsigned int lUserID;              /* 用户ID*/
	char szUserName[MAX_LEN_NAME + 1]; /* 用户名*/
} ST_User;

/* 公共 操作设备 结构体 */
typedef struct SDevice
{
	unsigned int lDeviceID;              /* 设备ID*/
	char szDeviceName[MAX_LEN_NAME + 1]; /* 设备名*/
} ST_Device;

/* 公共 操作服务器 结构体 */
typedef struct SServer
{
	unsigned int lServerID;              /* 服务器ID*/
	char szServerName[MAX_LEN_NAME + 1]; /* 服务器名*/
} ST_Server;

/*公共 操作组 结构体*/
typedef struct SGroup
{
	unsigned int lGroupID;              /* 组ID*/
	char szGroupName[MAX_LEN_NAME+1];   /* 组名称*/
} ST_Group;

/* 组对象信息 结构体*/
typedef struct SObjInfo
{
	int lObjType;                     /* 对象类型*/
	unsigned int lObjID;              /* 对象编号*/
	char szObjName[MAX_LEN_NAME + 1]; /* 对象名称*/
}ST_ObjInfo;

/* 组信息 结构体 */
typedef struct SGroupInfo
{
	int lObjNum;               /* 组中对象的个数*/
	ST_Group stGroup;          /* 组基本信息*/
	ST_ObjInfo stObjInfo[0];   /* 组对象*/
}ST_GroupInfo;

/* 用户信息 结构体 */
typedef struct SUserInfo
{
	char szUserName[MAX_LEN_NAME + 1]; /* 用户名*/
	unsigned int lUserID;              /* 用户ID*/
	unsigned int lGroupID;             /* 分组ID*/
	unsigned int lRoleID;              /* 角色ID*/
	unsigned int lUpServerID;          /* 所属服务器ID*/
	int lCanSpeakFlg;                  /* 对讲能力*/
	int lAutoConnectFlg;               /* 自动连接选项*/
	int lAutoIPFlg;                    /* 动态IP选项*/
	int lEncryptFlg;                   /* 加密选项*/
	int lAlertFlg;                     /* 报警选项*/
	int lUserStatus;                   /* 用户在线状态*/
} ST_UserInfo;

/*配置用户信息结构体（72字节）*/
/* ST_BaseUserInfo -> ST_TblUserInfo -> ST_AddUserReq(用于网络) */
typedef struct SBaseUserInfo
{
	ST_UserInfo stUserInfo;
	unsigned int lUserCreateTime;           /* 账户创建时间. x64 兼容，long->int. MOD by zhengsw */
	unsigned short nUserValidityTime;        /* 账户有效期(单位：月)*/
	unsigned short nPasswdQ;                 /* 找回密码问题*/
	char szPasswdA[PASSWD_ANSWER_LENGTH];    /* 找回密码答案*/
}ST_BaseUserInfo;

/* 用户信息详细 结构体 */
typedef struct STblUserInfo
{
	ST_BaseUserInfo stBaseUserInfo;
	unsigned char byPasswd[MD5_LENGTH]; /* 密码*/
	unsigned char bySalt[MD5_LENGTH];   /* 用户密码对应的唯一的Key*/
} ST_TblUserInfo;

/* 设备信息 结构体 */
typedef struct SDeviceInfo
{
	char szDeviceName[MAX_LEN_NAME + 1];        /* 设备名称*/
	int lDevModelNo;                            /* 设备类型*/
	unsigned int lDeviceID;                     /* 设备ID*/
	unsigned int lUpServerID;                   /* 所属服务器ID*/
	int lDeviceFlg;                             /* 能力标志*/
	char cDevStatus;                            /* 设备在线状态标志*/
	char byReservedFlg[5];                      /* 预留标志位*/
	short  nCyptoID;                            /*加密算法ID [0--自动，其他---对应服务器算法能力表]*/
	char szPhoneNo[MAX_LEN_SIMNO];              /* SIM卡号码*/
	int lChannelNum;                            /* 默认通道数*/
} ST_DeviceInfo;

/*配置设备信息结构体（84字节）*/
typedef struct SBaseDeviceInfo
{
	ST_DeviceInfo stDeviceInfo;
	int lPTZModelNo;                         /* 云台型号*/
	int lPTZProtocol;                        /* 云台协议*/
	int lPTZBaudRate;                        /* 云台波特率*/
	int lPTZAddr;                            /* 云台波地址*/
}ST_BaseDeviceInfo;

/* 设备详细信息 结构体 */
typedef struct STblDeviceInfo
{
	ST_BaseDeviceInfo stBaseDeviceInfo;
	char szDevManufacturer[MAX_LEN_NAME + 1];   /* 设备生产厂家*/
	char szPTZManufacturer[MAX_LEN_NAME + 1];/* 云台生产厂家*/
	char szUserName[MAX_LEN_NAME + 1];       /* 用户名*/
	char szPasswd[MD5_LENGTH];               /* 密码*/
} ST_TblDeviceInfo;

/*配置服务器信息结构体（62字节）*/
/* 服务器信息 结构体 */
typedef struct SServerInfo
{
	char szServerName[MAX_LEN_NAME + 1]; /* 设备名称*/
	unsigned int lServerID;              /* 服务器ID*/
	unsigned int lUpServerID;            /* 所属服务器ID*/
	int  lServerIP;                      /* 服务器IP地址*/
	char szServerMac[SERVERMAC_LENGTH];  /* 服务器Mac地址*/
	int lServerPort;                     /* 服务器端口*/
	int lServerType;                     /* 服务器类型*/
	int lServerClass;                    /* 服务器级别*/
	int lServerSts;                      /* 服务器在线状态*/
} ST_ServerInfo;

/* 服务器详细信息 结构体 */
typedef struct STblServerInfo
{
	ST_ServerInfo stServerInfo;
	char szModelNo[MAX_LEN_NAME + 1];   /* 型号*/
	char szUserName[MAX_LEN_NAME + 1];  /* 用户名*/
	char szPasswd[MD5_LENGTH];          /* 密码*/
} ST_TblServerInfo;

/* 口令信息 结构体 */
typedef struct SPasswdInfo
{
	unsigned char bySalt[MD5_LENGTH];
	unsigned char byPassword[MD5_LENGTH];
	unsigned char byChallenge[MD5_LENGTH];
	unsigned char byResponse[MD5_LENGTH];
} ST_PasswdInfo;

/*用户会话信息结构体*/
typedef struct SSessionUser
{
	unsigned int lSessionID;
	unsigned int lUserID;
	unsigned int lUpServerID;
	int lEncryptFlg;
} ST_SessionUser;

/* 设备会话信息 部结构体*/
typedef struct SSessionDevice
{
	unsigned int lSessionID;
	unsigned int lDeviceID;
	unsigned int lUpServerID;
	int lEncryptFlg;
} ST_SessionDevice;

/*集群成员列表结构体*/
typedef struct SVoiceMember
{
	unsigned int lUpServerID; /*成员所属服务器*/
	unsigned int lObjID;      /*成员ID*/
	int lConfirmFlg;          /*成员的加入状态，0：未加入；1：已加入*/
} ST_VoiceMember;

/*集群成员信息结构体：ObjID可能是用户，设备或服务器*/
typedef struct SSessionObj
{
	unsigned int lSessionID;
	unsigned int lObjID;
	unsigned int lUpServerID;
	int lEncryptFlg;
} ST_SessionObj;

/*语音集群信息结构体*/
typedef struct SVoiceGroupInfo
{
	int lVoiceGroupType;                   /*集群类型(服务器分配)*/
	unsigned int lServerID;                /*集群所属服务器(服务器分配)*/
	unsigned int lVoiceGroupID;            /*集群ID(服务器分配)*/
	char szVoiceGroupName[MAX_LEN_NAME + 1];/*集群名称(发起对讲的客户端填充)*/
	/*暂定与发起者的名称(用户名或设备名)相同,在整个集群的生命周期中保持不变)*/
	ST_SessionObj stCreator;                /*集群创建者(发起对讲的客户端填充)*/
} ST_VoiceGroupInfo;

/* 对讲通道信息 结构体 */
typedef struct STalkInfo
{
	int lChannelCount;
	ST_VoiceGroupInfo astVoiceGroupInfo[MAX_VOICE_GROUPS];
} ST_TalkInfo;

/* 音视预览通道信息 结构体 */
typedef struct SPreviewInfo
{
	int lChannelNum;                        /* 预览通道数*/
	unsigned int plDevIDs[MAX_AVP_CHANNELS];/* 数据部*/
} ST_PreviewInfo;

/* 会话远端IP/端口信息 结构体 */
typedef struct SRemoteAddr
{
	unsigned int lSessionID;    /* 会话Id*/
	int lRemoteIP;              /* 远端IP*/
	unsigned int nRemotePort;   /* 远端端口*/
} ST_RemoteAddr;

/* 用户状态信息 结构体 */
typedef struct SUserStateInfo
{
	int lRemoteAddrRet;
	int lTalkInfoRet;
	int lPreviewInfoRet;
	ST_RemoteAddr stRemoteAddr;
	ST_TalkInfo stTalkInfo;
	ST_PreviewInfo stPreviewInfo;
} ST_UserStateInfo;

/*ADD_S by louzf 2015.3.25*/
/*设备配置查询请求数据部*/
typedef struct
{
	int lReserved[10];  /*预留*/
	/*以下为 配置类型编号数组,一次可请求多项，由接收长度计算成员数*/
	int alParamType[0];
}ST_DevConfigQuery;

/*设备配置设置请求数据部*/
typedef struct
{
	int ParamOperateFlag;           /*参数配置类型,只应用/只修改配置/既应用又修改配置*/
	int lReserved[4];               /*预留*/
	char content[0];                /*TLV形式后面跟随一项TLV结构的配置*/
}ST_DevConfigSet;

/*ADD_E by louzf 2015.3.25*/

/* 系统配置信息 结构体 */
typedef struct SSysConfig
{
	int lUsrNumMax;                           /* 最大管理用户数*/
	int lUsrOnlineMax;                        /* 最大在线用户数*/
	int lDevNumMax;                           /* 最大管理设备数*/
	int lDevOnlineMax;                        /* 最大在线设备数*/
	int lSrvNumMax;                           /* 最大管理服务器数*/
	int lSrvOnlineMax;                        /* 最大在线服务器数*/
	int lSrvEncryptFlg;                       /* 服务器是否提供加密服务（0:不加密 1:加密）*/
	int lMSMax;                               /* 流媒体服务器最大数*/
	int lMaxAVPSessionNo;                     /* 音视频预览最大连接路数*/
	int lMaxStorageNo;                        /* 最大存储路数*/
	int lMaxInterCommNo;                      /* 语音对讲最大连接路数*/
	int lMaxClusterInterCommNo;               /* 集群对讲最大对像数*/
	int lMaxTotalSaveTime;                    /* 集中存储最大保存时间 最小单位“天”*/
	int lMaxFileSaveTime;                     /* 单个录像文件最大存储时间 单位:秒*/
	int lMaxInBandWidth;                      /* 最大入带宽*/
	int lMaxOutBandWidth;                     /* 最大出带宽*/
	int lAlmLevel;                            /* 报警级别*/
	int lRespTimeout;                         /* 消息响应的超时时间 单位:秒*/
	int lOnLineTimeout;                       /* 对象在线检测的超时时间 单位:秒*/
	unsigned int lServerID;                   /* 服务器ID*/
	char szVideoFilePath[FILENAME_MAX_LENGTH];/* 录像文件保存的路径*/
} ST_SysConfig;

/* 流媒体状态信息 结构体 */
typedef struct
{
	unsigned int lCurSessionNum;          /* 会话个数*/
	unsigned int lCurStorageNum;          /* 存储路数*/
	unsigned int lCurInBandWidth;         /* 下行带宽*/
	unsigned int lCuroutBandWidth;        /* 上行带宽*/
	unsigned int lCurInterCommNum;        /* 对讲数*/
	unsigned int lCurClusterInterCommNum; /* 集群对讲数*/
} ST_SMPStateRpt;

/* 通用TLV 结构体 */
typedef struct STlvHdr
{
	int lType;
	int lLength;
	char content[0];
} ST_TlvHdr;

/* 日志级别信息 结构体 */
typedef struct
{
	int lLoglevel;
}ST_LogLevel;

/*组设备对应关系结构本*/
typedef struct SGroupDevice
{
	unsigned int lGroupID;
	unsigned int lDeviceID;
} ST_GroupDevice;

/*操作记录相关结构体定义*/
/* MOD by zhengsw. [X64] 该结构虽然不会用于网络传输，但是被 bspcom_TimeStr2Uint 使用 */
typedef struct SOperationLog
{
	ST_ObjInfo SObj;
	ST_ObjInfo DObj;
	unsigned int lSessionID;
	unsigned int lTime;         /* x64 兼容，long->int. MOD by zhengsw */
	int lOperationType;
	char szDetails[MAX_LEN_DETAILS];
} ST_OperationLog;

/*内部请求消息头结构体*/
typedef struct SReqMsgHdr
{
	ST_DataHdr stDataHdr;        /* 数据头*/
} ST_ReqMsgHdr;

/*内部响应消息头结构体*/
typedef struct SResMsgHdr
{
	ST_DataHdr stDataHdr;        /* 数据头*/
	ST_MsgRespRet stMsgRet;      /* 响应结果*/
} ST_ResMsgHdr;

/*内部通知消息头结构体*/
typedef struct SNfyMsgHdr
{
} ST_NfyMsgHdr;

/*外部请求消息头结构体*/
typedef struct SInterReqMsgHdr
{
	yv_InterCommHdr stInterHdr;  /* 业务头*/
	ST_DataHdr stDataHdr;        /* 数据头*/
} ST_InterReqMsgHdr;

/*外部响应消息头结构体*/
typedef struct SInterResMsgHdr
{
	yv_InterCommHdr stInterHdr;  /* 业务头*/
	ST_DataHdr stDataHdr;        /* 数据头*/
	ST_MsgRespRet stMsgRet;      /* 响应结果*/
} ST_InterResMsgHdr;

/*外部通知消息头结构体*/
typedef struct SInterNfyMsgHdr
{
	yv_InterCommHdr stInterHdr;  /* 业务头*/
} ST_InterNfyMsgHdr;

/*定义结构体表示uint64_t，实现跨平台传输*/
typedef struct SUInt64
{
	unsigned int lHigh;    /*高32位*/
	unsigned int lLow;     /*低32位*/
} ST_UInt64;

/*录相文件信息结构体定义*/
typedef struct SRecordFile
{
	char szFileName[FILENAME_MAX_LENGTH];   /*文件名64*/
	/*从文件名称可解析出：创建者ID，设备ID，通道号，文件序列号，录相起始时间*/
	ST_UInt64 stFileSize;                   /*文件大小*/
	int lTimeLength;                        /*录相文件时长*/
	int lFileFlg;                           /*录相内容标志，如音频、视频、GPS、通用数据等*/
	char cFileType;                         /*录相文件类型，最大可支持255种类型扩展*/
	char byReservedFlg[7];                  /*保留位*/
} ST_RecordFile;

/*ADD_S by liangy @2015-06-11*/
/*加密录像文件头*/
typedef struct SEncryptFileHeader
{
	/*文件信息*/
	char magic[4];                          /*'y''v''m''f'*/
	char version;                           /*版本,默认0*/
	char reserved1[3];                      /*预留1(结构体对齐)*/
	short header_len;                       /*文件头长度*/
	short data_offset;                      /*实际数据起始的偏移地址*/
	ST_UInt64 src_file_size;                /*加密前文件数据长度*/
	ST_UInt64 enc_file_size;                /*加密后文件数据长度*/
	short crypto;                           /*加密算法*/
	short reserved2;                        /*预留2*/

	/*录像信息(供设备直接遍历获取录像列表)*/
	unsigned int creator_id;                /*创建者ID:服务器或设备*/
	unsigned int device_id;                 /*设备ID*/
	unsigned int channel_id;                /*设备通道ID*/
	unsigned int begin_time;                /*录相起始时间*/
	unsigned int end_time;                  /*录相结束时间*/
	int time_len;                           /*录相文件时长:秒*/
	int file_flg;                           /*录相内容标志，如音频、视频、GPS、通用数据等*/
	char cFileType;                         /*录相文件类型*/
	char reserved3[7];                      /*预留3(结构体对齐)*/

	char content[0];
} ST_EncryptFileHeader;
/*ADD_E by liangy @2015-06-11*/

/* 修改口令消息 数据部结构体*/
typedef struct SUserPasswd
{
	unsigned int lUserID;              /* 用户Id*/
	unsigned char byPasswd[MD5_LENGTH];/* 密码的MD5值*/
	unsigned char bySalt[MD5_LENGTH];  /* 随机值*/
} ST_UserPasswd;

/*角色信息  结构体*/
typedef struct SRoleInfo
{
	unsigned int lRoleID;
	char szRoleName[MAX_LEN_NAME+1];
} ST_RoleInfo;
/*权限信息  结构体*/
typedef struct SPrivilegeInfo
{
	unsigned int lPrivilegeID;
	char szPrivilegeName[MAX_LEN_NAME+1];
} ST_PrivilegeInfo;

/*接收设备报警/服务器报警的数据结构*/
typedef struct SAlarmInfo
{
	unsigned int lObjID;                    /* 对象编号*/
	int lObjType;                           /*（0：用户   1：设备   2：服务器）*/
	int lAlarmLevel;                        /* 报警级别*/
	char szAlarmInfo[ALARM_INFO_LENGTH];    /* 报警信息内容*/
	int	 lAlmTime;			                /* 报警时间. x64 兼容，long->int. MOD by zhengsw */
} ST_AlarmInfo;

typedef struct SStreamCapParam
{
	/*视频参数*/
	int lVideo_picdpi;          /* 格式CIF/D1*/
	int lVideo_picquality;      /* 图片质量*/
	int lVideo_codec;           /* 编码格式 h264...*/
	int lVideo_biterate;        /* 码率 in kbps*/
	int lVideo_gop;             /* GOP*/
	int lVideo_framerate;       /* 帧率*/
	int lVideo_ratecontrol;     /* 编码方式 CBR VBR AVR*/
	int lVideo_VBR_QP;          /* QP*/
	int lVideo_autobiterate;    /* 码率自动调节设置*/
	int lVideo_maxbiterate;     /* 最大码率*/
	int lVideo_minbiterate;     /* 最小码率*/
	int lVideo_autoframerate;   /* 帧率自动调节设置*/
	int lVideo_maxframerate;    /* 最大帧率*/
	int lVideo_minframerate;    /* 最小帧率*/
	/*音频参数*/
	int lAudio_input;           /* 音频流的输入制式mic or linein*/
	int lAudio_codec;           /* 音频流的编码格式*/
	int lAudio_sample;          /* 音频流的采样率*/
	int lAudio_channelnum;      /* 音频流的通道数*/
	int lAudio_bitwidth;        /* 音频流的量化位数*/
	int lAudio_bitrate;         /* 音频流的码率*/
	int lAudio_volume;          /* 音频流的音量*/
}ST_StreamCapParam;

/*流信息单元*/
/*未完整*/
typedef struct SVectorStreamInfo
{
	char szCaptureDevAlias[MAX_LEN_DEVALIAS_STR];   /* 采集设备别名*/
	int lStreamType;                                /* 流类型，音频视频，其他*/
	int lStreamUploadFlag;                          /* 流上传设置*/
	int lStreamRecordFlag;                          /* 流录像设置*/
	int lStreamID;                                  /* 流编号*/
	int lOutVisibleFlag;                            /* 外部是否可见*/
	ST_StreamCapParam stStreamCapParam;             /* 流采集参数*/
}ST_VectorStreamInfo;

typedef struct SDevStreamInfo
{
	int lStreamCount;                           /* 流信息的个数*/
	ST_VectorStreamInfo stVectorStreamInfo[0];  /* 流信息*/
}ST_DevStreamInfo;

/*客户端修改音频参数结构体*/
typedef struct SCltChgAudioParam
{
	int lStreamID;          /* 所要更改的流编号*/
	int lInputStd;          /* 输入制式*/
	int lChannel;           /* 通道数*/
	int lWidth;             /* 表示位数*/
	int lRate;              /* 采样率*/
	int lFmt;               /* 编码格式*/
	int lBitrate;           /* 码率*/
	int lAudio_volume;      /*音频流的音量*/
}ST_CltChgAudioParam;

/*客户端修改视频参数结构体*/
typedef struct SCltChgVideoParam
{
	int lStreamID;          /* 所要更改的流编号*/
	int lBitrate;           /* 码率*/
	int lFramerate;         /* 帧率*/
	int lDpi;               /* 分辨率*/
	int lBrightness;        /* 亮度*/
	int lColor;             /* 色彩*/
	int lContrast;          /* 对比度*/
	int lWidth;             /* 宽度*/
	int lHeight;            /* 高度*/
	int lGop;               /* Gop*/
}ST_CltChgVideoParam;

/*设备电池电量结构体*/
typedef struct SDevBatteryInfo
{
	int lBattery;   /* 电池电量百分比*/
	int lVotage;    /*电压值，经过*100换算，如301表示3.01伏*/
}ST_DevBatteryInfo;
/*设备报警阈值设置*/
typedef struct SDevAlarmThreshold
{
	int lBatteryPercentage;     /* 电池百分比下限*/
	int lCpuTemperature;        /* cpu温度上限*/
	int lCpuUsagePercentage;    /* cpu使用率上限*/
	int lMemRemaining;          /* 内存剩余下限，单位MB*/
}ST_DevAlarmThreshold;

/*设备录像设置*/
typedef struct SDevRecordSetting
{
	int lRecordFlag;                        /* 是否录像开关*/
	char szRecordPath[PATHNAME_MAX_LENGTH]; /* 录像存储路径*/
	int lRecordFileMaxTime;                 /* 每个录像文件存储最长时间,秒*/
	int lRecordFileMaxSize;                 /* 每个录像文件存储最大的文件大小*/
	int lRecordFileOverlagTime;             /* 相邻录像文件重叠时间，秒*/
	int lRecordPermitMinResStorage;         /* 允许系统保留的最小空间 MB*/
	int lTimingRecordFlag;                  /* 是否定时录像开关*/
	int lTimingRecordType;                  /* 定时类型 1每天 2每月*/
	int lTimingRecordStartTime;             /* 定时起始时间*/
	int lTimingRecordStopTime;              /* 定时结束时间*/
}ST_DevRecordSetting;

/*GPS设备串口参数设置*/
typedef struct SGPSDevComSetting
{
	int Baudrate;   /* 波特率*/
	int databits;   /* 数据位*/
	int stopbits;   /* 停止位*/
	int parity;     /* 奇偶校验位*/
}ST_GPSDevComSetting;

/*短信控制相关结构体*/
/*一个控制电话号码的定义*/
typedef struct SSmsControlNum
{
	int lInuseFlag;     /*是否启用该号码的标识*/
	char szPhoneNum[MAX_LEN_PHONENUM_STR];
}ST_SmsControlNum;

/*短信控制结构体*/
typedef struct SSmsControlSetting
{
	/*短信控制相关，由设备管理读取*/
	int lSmsControlFlag; /*短信控制是否启用标识*/
	ST_SmsControlNum stSmsControlNum[MAX_SUPPORT_SMS_PHONE_COUNT];
}ST_SmsControlSetting;

/*流参数设置*/
typedef struct SStreamConfigInfo
{
	int lInuseFlag;                                 /* 是否启用标识*/
	char szCaptureDevAlias[MAX_LEN_DEVALIAS_STR];   /* 采集设备别名*/
	int lStreamType;                                /* 流类型，音频视频，其他*/
	int lStreamUploadFlag;                          /* 流上传设置*/
	int lStreamRecordFlag;                          /* 流录像设置*/
	/*OSD显示配置*/
	int lOsdDisplayFlag;                            /* OSD显示开关*/
	/*流采集参数*/
	ST_StreamCapParam stStreamCapParam;
}ST_StreamConfigInfo;

/*设备配置信息结构体*/
typedef struct SDevConfigInfo
{
	int lAutoLoginFlag;                                                 /* 程序启动自动登陆标识*/
	char szServerIP[MAX_LEN_IP_STR];                                    /* 服务器IP*/
	int lServerPort;                                                    /* 服务器端口*/
	int lDataEncryptFlag;                                               /* 设备加密开关*/
	int lWIFI3GSwitchDefaultSetting;                                    /* 3G/wifi初始设置 0:有硬件开关决定,1:默认WIFI模式,2:默认3G模式，3:默认wifi+3G模式*/
	ST_DevAlarmThreshold stDevAlarmThreshold;                           /* 报警阈值设定*/
	ST_DevRecordSetting stDevRecordSetting;                             /* 录像设置*/
	ST_GPSDevComSetting stGPSDevComSetting;                             /* GPS设备串口设置*/
	ST_SmsControlSetting stSmsControlSetting;                           /* 短信控制设置*/
	ST_StreamConfigInfo stStreamConfigInfo[MAX_SUPPORT_STREAM_COUNT];   /* 流参数设置*/
}ST_DevConfigInfo;

/* 设备基本控制请求 数据部  */
typedef struct SCtrlDev
{
	char szDeviceType[MAX_LEN_NAME+1];     /* 设备类型*/
	unsigned int lDeviceID;                /* 设备编号*/
	unsigned int lServerID;                /* 设备所属服务器编号*/
} ST_CtrlDev;

/* 视频控制请求 数据部  */
typedef struct SCtrlDevVidio
{
	char szDeviceType[MAX_LEN_NAME+1];     /* 设备类型*/
	unsigned int lDeviceID;                /* 设备编号*/
	unsigned int lServerID;                /* 设备所属服务器编号*/
	int lFrameRate;                        /* 帧率:1~25*/
	int lBitRate;                          /* 码率*/
	EN_Resolution enResolution;            /* 分辨率*/
}ST_CtrlVidio;

/* 视频控制请求 */
typedef struct SCtrlAudio
{
	char szDeviceType[MAX_LEN_NAME+1];     /* 设备类型*/
	unsigned int lDeviceID;                /* 设备编号*/
	unsigned int lServerID;                /* 设备所属服务器编号*/
	int lInputType;                        /* 输入类型:0-LINEIN,1-MIC*/
	int lvolume;                           /* 音量:百分比*/
}ST_CtrlAudio;

/* 锁定云台请求 数据部  */
typedef struct SLockPtz
{
	unsigned int lUserID;                  /* 用户ID*/
	unsigned int lDeviceID;                /* 设备编号*/
	unsigned int lServerID;                /* 设备所属服务器编号*/
} ST_LockPtz;

/* 云台控制 数据部  */
typedef struct SCtrlPtz
{
	char szmanufacturer[MAX_LEN_NAME+1];   /* 云台生产厂家*/
	char szPtzType[MAX_LEN_NAME+1];        /* 云台型号*/
	unsigned int lUserID;                  /* 用户ID*/
	unsigned int lDeviceID;                /* 设备编号*/
	unsigned int lServerID;                /* 设备所属服务器编号*/
	unsigned int enprotocol;               /* 协议*/
	int enBaudRate;                        /* 波特率*/
	int lAddressCode;                      /* 地址码*/
	int lFstCmd;                           /* 第一指令:具体执行命令(方位，焦距，光圈)、自动巡航、预置点*/
	int lScdCmd;                           /* 第二指令:具体命令停止、关闭自动巡航、*/
	unsigned int lTime;                   /* 第一、二指令的间隔时间. x64 兼容，long->int. MOD by zhengsw */
} ST_CtrlPtz;

/* 用用户锁定云台后发送心跳包 数据部  */
typedef struct SPtzKeepAlive
{
	unsigned int lServerID;                /* 设备所属服务器编号*/
}ST_PtzKeepAlive;

/* 音视频预览申请请求的 数据部 */
typedef struct SAVPApplyRequest
{
	unsigned int lSSrvID;  /* 流来源服务器ID*/
	unsigned int lSObjID;  /* 流来源的设备ID*/
	unsigned int lDSrvID;  /* 转发目标的服务器ID*/
	unsigned int lDObjID;  /* 转发目标的编号ID*/
	unsigned int lStreamID;/* 流编号*/
} ST_AVPApplyRequest;

/* 音视频预览申请返回结果的 数据部 */
typedef struct SAVPApplyResponse
{
	unsigned int lAvpBusinessID;/* 判断远程视频是否已经传到本地*/
} ST_AVPApplyResponse;

/* 音视频连接/停止连接指令的 数据部 */
typedef struct SAVPConnectCtrl
{
	unsigned int lAvpBusinessID;/* 用于音视频连接的业务ID*/
	int lCmd;                   /* 命令代码*/
} ST_AVPConnectCtrl;

/* 流媒体要求设备发送或停止流指令的 数据部 */
typedef struct SSMPDevCtrl
{
	unsigned int lDSID;    /* 目标服务器编号*/
	unsigned int lDOID;    /* 目标设备编号*/
	int lCmd;              /* 当前控制命令编号*/
	unsigned int lStreamID;/* 流ID编号*/
} ST_SMPDevCtrl;

/* 客户端发送给音视频预览的保活消息的 数据部 */
typedef struct SAvpKeepAlive
{
	unsigned int        lAvpBusinessID;/* 音视频预览业务ID*/
	ST_AVPApplyRequest  stAvpInfo;     /* 音视频预览信息*/
} ST_AvpKeepAlive;

/*语音对讲流数据属性*/
typedef struct SIntercomStreamProperty                              
{                          
	unsigned int lVoiceGroupID;            /*集群ID*/     
	unsigned int lLen;                    /*对讲音频流数据长度*/ 
	short lExpInfoType;     /*扩展信息类型*/ 
	short lExpInfoLen;      /*扩展信息长度*/ 
}ST_IntercomStreamProperty; 

/* CPU信息 结构体 */
/* zhengsw. 该结构体用于存储系统信息，
 * 并不会经过网络发送,long 类型可以保留
 */
typedef struct
{
	char szName[ITEM_MAX_LEN];   /* 名称*/
	unsigned long lUser;         /* 用户使用间隙*/
	unsigned long lNice;         /* 优先级低cpu使用间隙*/
	unsigned long lSystem;       /* 系统使用间隙*/
	unsigned long lIdle;         /* 空闲间隙*/
	unsigned long lIrq;          /* 硬中断使用间隙*/
	unsigned long lSoftirq;      /* 软中断使用间隙*/
	unsigned long lStealstolen;  
	int lCpuUtilizationRate;     /* cpu使用率*/
	int lFirstFlag;              /* 标志位*/
} ST_CpuOccupy;

/* cpu的信息 */
typedef struct
{
	ST_CpuOccupy stCpuOccupy;    /* cpu占有率*/
	int lCpuTemperature;         /* cpu温度*/
} ST_CpuInfo;

/* 内存信息 结构体*/
/* zhengsw. 该结构体用于存储系统信息，
 * 64位系统下可以支持 int 无法存储的内存容量
 * 并不会经过网络发送,long 类型可以保留
 */
typedef struct
{
	char szName[ITEM_MAX_LEN];
	unsigned long lMemTotal;    /* 总的内存容量*/
	unsigned long lMemFree;     /* 空余容量*/
	unsigned long lBuffers;     /* 缓冲区容量*/
	unsigned long lCached;      /* 缓存容量*/
	char szUnits[ITEM_MAX_LEN]; /* 单位*/
	int lMemUtilizationRate;    /* 使用率*/
} ST_MemOccupy;

/* 内存的信息 */
typedef struct
{
	ST_MemOccupy stMemOccupy; /* 内存占有率*/
} ST_MemInfo;

/*网络负载信息 结构体*/
/* zhengsw. [X64] 该结构体用于存储系统信息，
 * 并不会经过网络发送,long 类型可以保留
 */
typedef struct
{
	char szName[ITEM_MAX_LEN];  /* 名称*/
	unsigned long lRecBytes;    /* 接受网络bytes数目*/
	unsigned long lRecPackets;  /* 接受包数*/
	unsigned long lRecErrs;     /* 接受错误包数*/
	unsigned long lRecDrop;     /* 丢弃的包数*/
	unsigned long lRecFifo;     
	unsigned long lRecFrame;    /* 接受的帧数*/
	unsigned long lRecCompressed;
	unsigned long lRecMulticast;/* 接受多播报数*/
	unsigned long lTranBytes;   /* 发送网络接受byte数目*/
	unsigned long lTranPackets; /* 发送包数*/
	unsigned long lTranErrs;    /* 发送错误*/
	unsigned long lTranDrop;    /* 发送丢弃的包数*/
	unsigned long lTranFifo;    
	unsigned long lTranColls;   /* 发送冲突包*/
	unsigned long lTranCarrier;
	unsigned long lTranCompressed;
} ST_NetLoad;

/* 服务器的状态 结构体*/
/* zhengsw. [X64] 该结构会作为网络消息结构体 ST_SrvViewResp 的间接成员 */
typedef struct
{
	unsigned int  lCpuOccupy;             /* cpu占有率*/
	int           lCpuTemperature;        /* cpu温度*/
	unsigned int  lMemOccupy;             /* 内存占有率*/
	unsigned int  lRunTime;               /* 运行时间.     x64 兼容，long->int. MOD by zhengsw */
	unsigned int  lRecBytes;              /* 网络下行流量. x64 兼容，long->int. MOD by zhengsw */
	unsigned int  lTranBytes;             /* 网络上行流量. x64 兼容，long->int. MOD by zhengsw */
	unsigned int  lMediaMountOccupy;      /* 磁盘使用率*/
	char szMountDir[PATHNAME_MAX_LENGTH]; /* 录像文件所在目录*/
} ST_SrvStat;

/* 级联信息暂时不用的 */
typedef struct
{
	unsigned int lChildSrv[CHILDSRV_MAX_LEN]; /* 级联信息 */
	unsigned int lChildSrvLen;                /* 级联长度 */
} ST_SrvCascadeInfo;

/* 服务器的信息合 */
typedef struct
{
	unsigned int lOnlineSrvNum;    /* 在线服务器数量*/
	unsigned int lOnlineUserNum;   /* 在线用户数目*/
	unsigned int lOnlineDevNum;    /* 在线设备数目*/
	unsigned int lAlarmNum;        /* 本服务器产生报警个数*/
	unsigned int lServerID;        /* 服务器ID*/
	ST_SrvStat stSrvStat;          /* 服务器状态信息*/
	ST_SMPStateRpt stSMPStateRpt;  /* 流媒体信息*/
} ST_SrvInfo;

/* 服务器全览请求结构体 */
typedef struct
{
	unsigned int lServerID;            /* 服务器ID*/
} ST_SrvOverviewRequest;

/* 服务器全览应答结构体 */
typedef struct
{
	ST_SrvInfo stSrvInfo;              /* 服务器信息*/
} ST_SrvOverviewResponse;

/* 修改报警级别消息 数据部 结构体 */
typedef struct SUserAlert
{
	unsigned int lUserID;
	int lAlertFlg;
} ST_UserAlert;

/*定位数据信息结构体*/
typedef struct SPositionData
{
	char szUTC[MAX_LEN_GPS_STR];                   /*UTC时间 : yymmddhhmmss年月日时分秒,如130325111111*/
	char szLongitude[MAX_LEN_GPS_STR];     /* 经度：E/Wdddmm.mmmm东经/西经度分,如E00011.1111*/
	char szLatitude[MAX_LEN_GPS_STR];          /* 纬度: N/Sddmm.mmmm 南纬/北纬度分格式，如N0011.1111*/
	char szSpeed[MAX_LEN_GPS_STR];             /* 地面速率（000.0~999.9 节 ，Knot，前导位数不足则补0）*/
	char szOrientation[MAX_LEN_GPS_STR];   /* 地面航向（000.0~359.9 度，以真北为参考基准，前导位数不足则补0）*/

}ST_PositionData;

/*设备SOS状态信息结构*/
typedef struct SSosDevStatus
{
	ST_SessionDevice stSessionDevice; /* 设备会话信息结构*/
	ST_PositionData stPosition;       /* 定位数据*/
}ST_SosDevStatus;

/*设备状态信息结构体*/
/* MOD by zhengsw. [X64] 该结构会作为网络消息结构体 ST_CltDevStatusInfoRes 的成员 */
typedef struct SDevStatusInfo
{
	ST_DevBatteryInfo stDevBatteryInfo; /*电池信息*/
	int lCpuTemperature;            /* cpu温度上限*/
	int lCpuOccupy;                 /* cpu使用率*/
	int lMemRemaining;              /* 内存剩余，单位MB*/
	int lStorageRemaining;          /* 剩余磁盘空间 MB*/
	unsigned int lRunTime;          /* 运行时长. x64 兼容，long->int. MOD by zhengsw */
	ST_PositionData stPositionData; /* 定位数据*/
}ST_DevStatusInfo;

/******ADD_S_by_Wangly VPN二期新增结构定义******/
/*链路数据传输信息结构体*/
typedef struct
{
	int lIfIndex;        /*接口ID*/
	char szIfName[MAX_LEN_IFNAME_STR]; /*PPP接口名称*/
	int lTransmitBytes; /*网卡发送数据量*/
	int lReceiveBytes;  /*网卡接收数据量*/
} ST_VpnNicTransmitInfo;

/*设备传输数据统计信息*/
typedef struct
{
	int lTransmitBytes; /*发送字节数*/
	int lReceiveBytes;  /*接收字节数*/
} ST_VpnDevTransmitInfo;

/*链路传输信息结构体*/
typedef struct
{
	unsigned int lObjID; /*3G链路的目标ID*/
	int lIfIndex;        /*接口ID*/
	char szIfName[MAX_LEN_IFNAME_STR]; /*PPP接口名称*/
	int lBPS;            /*数据发送速率 单位:KB/s*/
	int lPPS;            /*包发送速率 单位:包每秒*/
	int lRTT;            /*往返时延(RTT) 单位:微秒*/
	int lLossPermillage; /*丢包千分比*/
	int lWeight;         /*权重*/
	char szReserve[8];   /*保留位*/
} ST_LinkTransmitInfo;

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

/*设备通道状态信息结构体*/
typedef struct
{
	ST_VpnDevTransmitInfo stVpnDevTransmitInfo; /*设备接收和发送数据统计信息*/
	ST_VpnNicTransmitInfo astVpnNicTransmitInfo[MAX_SUPPORT_3G_DEV_COUNT+1]; /*链路接收和发送数据统计*/
	ST_LinkTransmitInfo astLinkTransmitInfo[MAX_SUPPORT_3G_DEV_COUNT+1]; /*链路传输信息*/
	ST_DevLinkStatusInfo stDevLinkStatusInfo; /*链路链接状态信息*/
} ST_DevChannelStatus;

/*设备状态信息描述通用结构体*/
typedef struct SDevCommonStatusInfo
{
	char cStatusInfoVersion; /*状态信息版本*/
	char cStatusInfoType;    /*状态信息类型*/
	char szReserve[6];       /*保留位*/
	int lStatusInfoLength;   /*状态信息长度*/
	char Content[0];         /*状态信息内容*/
}ST_DevCommonStatusInfo;
/******ADD_E_by_Wangly VPN二期新增结构定义******/

/*设备SOS报警信息，未完成*/
/* MOD by zhengsw. [X64] 该结构会作为网络消息结构体 ST_DevNfySrvSosAlarm 的成员 */
typedef struct SDevSOSInfo
{
	unsigned int lDeviceID;         /* 发出SOS的设备ID*/
	unsigned int lAlarmTime;       /* 报警时间. x64 兼容，long->int. MOD by zhengsw */
	ST_PositionData stPositionData; /* 定位数据*/
}ST_DevSOSInfo;

/*数据描述公用头部结构定义*/
typedef struct
{
	int lDataType;                 /*数据描述类型*/
	int lDataDscpLen;              /*数据描述长度*/
	int lDataVersion;              /*数据版本*/
	int lDataOffset;               /*数据位置偏移量*/
	int lDataLength;               /*数据长度*/
	unsigned int lDataTimeLength;  /*数据时长*/
} ST_DataHdrCommonDesc;

/*数据描述结构定义*/
typedef struct
{
	ST_DataHdrCommonDesc stDataHdrCommonDesc;
	union
	{
		struct /*视频私有数据描述*/
		{
			short lCodec;    /*编码制式*/
			short lDpi;      /*分辨率*/
			int lHeight;     /*高度*/
			int lWidth;      /*宽度*/
			char reserve[4]; /*保留位*/
		}VideoPriv;
		struct /*音频私有数据描述*/
		{
			unsigned char lSample;     /*采样频率*/
			unsigned char lBitwidth;   /*量化位数*/
			unsigned char lChannelNum; /*声道*/
			unsigned char lCodec;      /*编码制式*/
			char reserve[12];          /*保留位*/
		}AudioPriv;
		struct /*通用数据私有数据描述*/
		{
			char reserve[16];
		}CommonPriv;
	};
} ST_DataDesc;

/*回放块信息*/
typedef struct
{
	int lBlockSize;            /*回放块大小*/
	int lDataDscpNum;          /*数据描述个数*/
	int lDataDscpLen;          /*数据描述部分长度*/
	unsigned int lBeginTime;   /*回放块起始时间, 单位:毫秒*/
	unsigned int lTimeLength;  /*回放块的时长, 单位:毫秒*/
	ST_DataDesc astDataDesc[0];
} ST_PlayBackBlockInfo;

typedef struct SChallenRespInfo
{
	unsigned char bySalt[MD5_LENGTH];
	unsigned char byChallenResp[MD5_LENGTH];
} ST_ChallenRespInfo;

/*请求控制云台*/
/* MOD by zhengsw. [X64] 该结构会作为网络消息结构体ST_InterCtrlPtzReq 的成员 */
typedef struct
{
	unsigned int lDeviceID;                     /* 设备编号*/
	int enBaudRate;                             /* 波特率*/
	int lFirstCmdLen;                           /* 第一命令长度*/
	int lSecondCmdLen;                          /* 第一命令长度*/
	unsigned int lInterval;                    /* 第一、二指令的间隔时间. x64 兼容，long->int. MOD by zhengsw */
	unsigned char byFirstCmd[MAX_PTZ_CMD_LEN];  /* 第一指令：具体执行命令(方位倍长，焦距，光圈)、自动巡航、预置点*/
	unsigned char bySecondCmd[MAX_PTZ_CMD_LEN]; /* 第二指令：具体命令停止、关闭自动巡航、*/
}ST_DevCtrlPtz;

///*视频流扩展信息头*/
//typedef struct
//{
//   short  lCodec;    /*编码格式*/
//   short  lDpi;      /*分辨率*/
//   int    lHeight;   /*高度*/
//   int    lWidth;    /*宽度*/
//   char   reserve[4];/*保留位*/
//}ST_VideoStreamExtInfo;

/*各音频制式携带的相关数据，附在音频流扩展信息头之后*/
typedef struct
{
	union 
	{
		/* SPEEX */
		struct
		{
			unsigned int lBiteRate;/*码率*/
			char reserve[8];/*保留位*/ 
		}stSpeexCodecInfo;
		/* MP3 */ 
		struct
		{  
			unsigned int lBiteRate;/*码率*/ 
			/*码率控制类型*/
			unsigned int lRateControlType;
			unsigned short nVoicePower;	/*声音强度*/
			char reserve[2];/*保留位*/ 
		}stMp3CodecInfo;
		/* OPUS 私有结构 */ 
		struct
		{  
			unsigned int lBiteRate;/*码率*/ 
			unsigned short nFragSize;/*分包的大小*/
			unsigned char cFragCount;/*分包数*/
			unsigned char cTsGap;/*分包的时戳间隔，毫秒，注意上限为255ms，所以，打包间隔就要求不能超过该时限，超过该间隔需直接分开发送*/  
			unsigned short nVoicePower; /*声音强度*/
			char reserve[2];/*保留位*/ 
		}stOpusCodecInfo;
		/* ADPCM */
		struct
		{
			unsigned int lBiteRate; /*码率*/
			short PredictedValue;/*帧中预测值*/
			short StepIndex;/*帧中的步长索引*/
			char reserve[4];/*保留位*/
		}stAdpcmCodecInfo;
	};
}ST_AudioPrivateData;
/*音频扩展信息*/
typedef struct 
{ 
	/*pcm信息*/
	unsigned char lSample;    /* 采样频率 */ 
	unsigned char lBitwidth;  /* 量化位数 */ 
	unsigned char lChannelNum;/* 声道     */
	/*编码制式,决定后续内容*/
	unsigned char lCodec;
	char Content[0];
}ST_AudioStreamExtInfo;

/*通用数据结构定义*/
/*通用数据采集间隔结构体*/
typedef  struct
{
	int lCapFreq_Sec;               /*采集时间间隔。秒值*/
	int lCapFreq_USec;           /*采集时间间隔。微秒值*/
}ST_GpCapFreq;

/*通用数据采集参数信息*/
typedef struct
{
	GP_DATA_TYPE_T enGpDataType;/*通用数据具体类型编号，使用4字节*/
	ST_GpCapFreq stGpCapFreq;       /*采集频率，包括秒值与微秒值*/
	char reserved[4];                            /*保留位*/
}ST_GpCapInfo;
/*通用数据的流扩展信息*/
typedef struct 
{
	ST_GpCapInfo stGpCapInfo;/*通用数据采集信息*/
	int    lDataCrc32;                       /*通用数据的CRC32校验码*/
	char    reserved[4];                   /*保留位*/
}ST_GpDataExtInfo;

typedef struct SStreamProperty
{
	union
	{
		//video body
		struct
		{
			unsigned int timeStamp;
			unsigned int seqNo;
			unsigned char keyFrame;
			char reserve[3];
		}video;
		//audio body
		struct
		{
			unsigned int timeStamp;
			unsigned int seqNo;
			char reserve[4];
		}audio;
		//data body
		struct
		{
			unsigned int timeStamp;
			unsigned int seqNo;
			char reserve[4];
		}data;
	};
}ST_StreamProperty;

typedef struct {
	short lCodec;              /*编码格式*/
	short lDpi;                /*分辨率*/
	int lHeight;               /*高度*/
	int lWidth;                /*宽度*/
	char reserve[4];           /*保留位*/
	char Content[0];
}ST_VideoStreamExtInfo;

typedef struct SStreamHdr
{
	short lStreamID;                /* 流ID，用于区分来自同一个设备的不同流*/
	short lType;                    /* 流媒体类型，音频，视频，etc*/
	int lLen;                       /* 报文长度*/
	short lExpInfoType; /*扩展信息类型*/ 
	short lExpInfoLen; /*扩展信息长度*/ 
	ST_StreamProperty stProperty;   /* 属性，具体针对各种流类型的处理*/
	char Content[0];
}ST_StreamHdr;

/*子设备基本信息结构体*/
typedef struct SSubDeviceInfo
{
	char szSubDeviceName[MAX_LEN_DEVALIAS_STR]; /*子设备别名*/
	short nSubID;                               /*子设备编号(采集实时流编号StreamID=SubID)*/
	short nSubType;                             /*子设备类型(预留)*/
	short nStreamType;                          /*采集或输出流类型:0:未知;1:视频;2:音频;3:通用数据;4:业务扩展数据；其他*/
	short nReserved;                            /*预留参数,兼顾对齐*/
	char cIOFlg;                                /*0:采集输入;1:控制输出;2:可同时作为输入和输出*/
	char reserved[7];                           /*保留位*/
} ST_SubDeviceInfo;

/*子设备视频参数*/
typedef struct SVideoParam
{
	int lVideo_picdpi;          /* 格式CIF/D1*/
	int lVideo_picquality;      /* 图片质量*/
	int lVideo_codec;           /* 编码格式 h264...*/
	int lVideo_biterate;        /* 码率 in kbps*/
	int lVideo_gop;             /* GOP*/
	int lVideo_framerate;       /* 帧率*/
	int lVideo_ratecontrol;     /* 编码方式 CBR VBR AVR*/
	int lVideo_VBR_QP;          /* QP*/
	int lVideo_autobiterate;    /* 码率自动调节设置*/
	int lVideo_maxbiterate;     /* 最大码率*/
	int lVideo_minbiterate;     /* 最小码率*/
	int lVideo_autoframerate;   /* 帧率自动调节设置*/
	int lVideo_maxframerate;    /* 最大帧率*/
	int lVideo_minframerate;    /* 最小帧率*/
} ST_VideoParam;

/*子设备音频参数*/
typedef struct SAudioParam
{
	int lAudio_input;           /* 音频流的输入制式mic or linein*/
	int lAudio_codec;           /* 音频流的编码格式*/
	int lAudio_sample;          /* 音频流的采样率*/
	int lAudio_channelnum;      /* 音频流的通道数*/
	int lAudio_bitwidth;        /* 音频流的量化位数*/
	int lAudio_bitrate;         /* 音频流的码率*/
	int lAudio_volume;          /* 音频流的音量*/
} ST_AudioParam;

/*子设备通用数据参数*/
typedef struct SGPDParam
{
} ST_GPDParam;

/*子设备详细配置信息*/
typedef struct SSubDeviceConfig
{
	struct {
		char szSubDeviceName[MAX_LEN_DEVALIAS_STR]; /*子设备别名*/
		short nSubID;          /*子设备编号(采集实时流编号StreamID=SubID)*/
		short nSubType;        /*子设备类型(预留)*/
		short nStreamType;     /*采集或输出流类型:0:未知;1:视频;2:音频;3:通用数据;4:业务扩展数据；其他*/
		short nReserved;       /*预留参数,兼顾对齐*/
		char cIOFlg;           /*0:采集输入;1:控制输出;2:可同时作为输入和输出*/
		char cStreamUploadFlag;/*流上传设置*/
		char cStreamRecordFlag;/*流录像设置*/
		char reserved[5];      /*保留位*/
	} stBasicInfo;
	union {
		struct {
			ST_VideoParam stLocalVideo;
			ST_VideoParam stUploadVideo;
		} stVideoInfo;
		struct {
			ST_AudioParam stLocalAudio;
			ST_AudioParam stUploadAudio;
		} stAudioInfo;
		struct {
			ST_GPDParam stGPDParam;
		} stGPDInfo;
	} unStreamInfo;
} ST_SubDeviceConfig;

/*ADD_S by liangy @2104-07-16*/
/*解码器预览配置信息*/
typedef struct SDecoderInfo
{
	unsigned int lSServerID;        /*(预留)解码用户所属服务器ID*/
	unsigned int lSUserID;          /*解码用户ID*/
	unsigned int lDServerID;        /*设备所属服务器ID*/
	unsigned int lDDeviceID;        /*预览设备ID*/
	unsigned int lDStreamID;        /*(预留)预览设备流ID*/
	int lDStreamType;               /*(预留)预览设备流类型枚举:音频,视频*/
	char cStatus;                   /*(预留)状态标志:0:停用;1:启用;*/
	char reserved[7];
} ST_DecoderInfo;

/*ADD_S by liangy @2014-11-04*/
/*用户状态信息结构体*/
typedef struct SUserStatusInfo
{
	char szUserName[MAX_LEN_NAME+1];   /*用户名*/
	unsigned int lUpServerID;          /*所属服务器ID*/
	unsigned int lUserID;              /*用户ID*/
	int lClientIP;                     /*客户端IP地址*/
	int lClientPort;                   /*客户端端口*/
	char szClientMac[SERVERMAC_LENGTH];/*Mac地址*/
	int lUserType;                     /*用户类型:1:普通用户;2:高级用户;*/
	int lClientType;                   /*客户端类型:100:Android;200:PC;300:iOS;*/
	char cUserLockFlg;                 /*账户是否锁定:0:未锁定(默认);1:锁定;*/
	char cStatus;                      /*用户在线状态*/
	char cDelFlg;                      /*删除标志:0:未删除(默认);1:已删除;*/
	char reserved[5];
} ST_UserStatusInfo;

/*设备状态信息结构体*/
typedef struct SDeviceStatusInfo
{
	char szDeviceName[MAX_LEN_NAME+1]; /*设备名称*/
	unsigned int lUpServerID;          /*所属服务器ID*/
	unsigned int lDeviceID;            /*设备ID*/
	int lDeviceIP;                     /*IP地址*/
	int lDevicePort;                   /*端口*/
	char szDeviceMac[SERVERMAC_LENGTH];/*Mac地址*/
	int lDeviceType;                   /*设备类型*/
	int lDeviceChannel;                /*设备通道号*/
	char cStatus;                      /*设备在线状态*/
	char cDelFlg;                      /*删除标志:0:未删除(默认);1:已删除;*/
	char reserved[6];
} ST_DeviceStatusInfo;

/*服务器状态信息结构体*/
typedef struct SServerStatusInfo
{
	char szServerName[MAX_LEN_NAME+1]; /*服务器名称*/
	unsigned int lUpServerID;          /*所属服务器ID*/
	unsigned int lServerID;            /*服务器ID*/
	int lServerIP;                     /*服务器IP地址*/
	int lServerPort;                   /*服务器端口*/
	char szServerMac[SERVERMAC_LENGTH];/*服务器Mac地址*/
	int lServerType;                   /*服务器类型*/
	int lServerClass;                  /*服务器级别*/
	int lServerKind;                   /*服务器种类:
0:嵌入式服务器;1:普通服务器;2:专业服务器;*/
	char cStatus;                      /*服务器在线状态*/
	char cDelFlg;                      /*删除标志:0:未删除(默认);1:已删除;*/
	char reserved[6];
} ST_ServerStatusInfo;

/*VPN业务消息结构定义*/
/*设备VPN配置信息数据模型*/
typedef struct
{
	char szDevNetAddress[NETWORK_SEGMENT_LENGTH]; /* 网络地址(和掩码组合表示网段)*/
	unsigned int lDevNetMask;                     /* 掩码 (和网络地址组合表示网段)*/
	int lEncryptFlg;                              /* 是否加密标志 0:不加密 1:加密*/
	int lCompressFlg;                             /* 是否压缩标志 0:不压缩 1:压缩*/
	char szDevLanIP[NETWORK_SEGMENT_LENGTH];      /* 设备IP地址(网关地址)*/
	char szDevLanInterface[NIC_NAME_MAX_LENGTH];  /* 设备端网络接口卡配置*/
	int lW1EnableFlg;                             /* 3G模块W1启用标志 0:不启用 1:启用*/
	int lW2EnableFlg;                             /* 3G模块W2启用标志 0:不启用 1:启用*/
	int lW3EnableFlg;                             /* 3G模块W3启用标志 0:不启用 1:启用*/
	int lW4EnableFlg;                             /* 3G模块W4启用标志 0:不启用 1:启用*/
	char szW1Standard[NET_STANDARD_NAME_LENGTH];  /* 3G模块W1的制式*/
	char szW2Standard[NET_STANDARD_NAME_LENGTH];  /* 3G模块W2的制式*/
	char szW3Standard[NET_STANDARD_NAME_LENGTH];  /* 3G模块W3的制式*/
	char szW4Standard[NET_STANDARD_NAME_LENGTH];  /* 3G模块W4的制式*/
	char szOemString[OEM_STRINE_MAX_LENGTH];      /* 设备标识信息*/
	int lCipherType;                              /* 数据加解密算法类型*/
} ST_DevVpnConfigInfo;

/*服务器VPN配置信息数据模型*/
typedef struct
{
	char szSrvNetAddress[NETWORK_SEGMENT_LENGTH];/* 服务器网段地址*/
	unsigned int lSrvNetMask;                    /* 服务器网段掩码*/
	int lEncryptFlg;                             /* 设备默认加密标志*/
	int lCompressFlg;                            /* 设备默认压缩标志*/
	int lCryptThreadNum;                         /* VPN模块中加解密线程创建个数*/
	char szSrvLanIP[NETWORK_SEGMENT_LENGTH];     /* 服务器局域网网口地址*/
	char szSrvLanInterface[NIC_NAME_MAX_LENGTH]; /* 服务器局域网网卡配置*/
	int lWan1EnableFlag;                         /* 服务器广域网1启用标志: 1:启用 0:不启用*/
	char szWan1IP[NETWORK_SEGMENT_LENGTH];       /* 服务器广域网1网口地址*/
	char szWan1Interface[NIC_NAME_MAX_LENGTH];   /* 服务器广域网1网卡配置*/
	int lWan2EnableFlag;                         /* 服务器广域网2启用标志: 1:启用 0:不启用*/
	char szWan2IP[NETWORK_SEGMENT_LENGTH];       /* 服务器广域网2网口地址*/
	char szWan2Interface[NIC_NAME_MAX_LENGTH];   /* 服务器广域网2网卡配置*/
	int lWan3EnableFlag;                         /* 服务器广域网3启用标志: 1:启用 0:不启用*/
	char szWan3IP[NETWORK_SEGMENT_LENGTH];       /* 服务器广域网3网口地址*/
	char szWan3Interface[NIC_NAME_MAX_LENGTH];   /* 服务器广域网3网卡配置*/
	int lWan4EnableFlag;                         /* 服务器广域网4启用标志: 1:启用 0:不启用*/
	char szWan4IP[NETWORK_SEGMENT_LENGTH];       /* 服务器广域网4网口地址*/
	char szWan4Interface[NIC_NAME_MAX_LENGTH];   /* 服务器广域网4网卡配置*/
	int lCipherType;                              /* 数据加解密算法类型*/
} ST_SrvVpnConfigInfo;

/*录像清理条件*/
typedef struct SRecordFilter
{
	char szFilterName[MAX_LEN_NAME+1];/*条件名称*/

	char byRecordNo;             /*按编号范围清理(预留)*/
	char byFileType;             /*按文件类型清理*/
	char byFileName;             /*按匹配文件名清理(预留)*/
	char byCreatorID;            /*按创建者ID清理*/
	char byDeviceID;             /*按设备ID清理*/
	char byChannelID;            /*按通道ID清理*/
	char byRecordTime;           /*按录像时间清理*/
	char byRecordTimeAgo;        /*清理超过一定时间的录像*/

	char byRecordTimeLength;     /*按录相时长清理(单位:秒)*/
	char byFileSize;             /*按文件大小清理*/
	char bySyncState;            /*按同步状态清理(预留)*/
	char byEncryptFlg ;          /*按加密标志清理*/
	char reserved[4];

	unsigned int lMinRecordNo;        /*编号范围*/
	unsigned int lMaxRecordNo;        /*编号范围*/
	int lFileType;                    /*清理该文件类型*/
	char szFileName[FILENAME_MAX_LENGTH];  /*清理匹配文件名*/
	unsigned int lCreatorID;          /*创建者ID*/
	unsigned int lDeviceID ;          /*设备ID*/
	unsigned int lChannelID;          /*音视频通道*/
	unsigned int lCleanBeginTime;     /*起始时间*/
	unsigned int lCleanEndTime;       /*结束时间*/
	unsigned int lTimeAgo;            /*多长时间以前,单位:秒;*/
	int lMinTimeLen;                  /*录相时间长度范围*/
	int lMaxTimeLen;                  /*录相时间长度范围*/
	ST_UInt64 stMinSize;              /*文件大小范围(单位:Byte)*/
	ST_UInt64 stMaxSize;              /*文件大小范围(单位:Byte)*/
	char cSyncState;                  /*同步状态:0-未同步;1-已同步;2-同步中*/
	char cEncryptFlg;                 /*加密标志:0-未加密;1-已加密;*/
	char pading[6];
} ST_RecordFilter;

/*录像清理策略*/
typedef struct SRecordCleanStrategy
{
	char szStrategyName[MAX_LEN_NAME+1];/*名称(标题)*/
	unsigned int lStrategyID;       /*策略在数据库中的索引*/
	int lCleanMode;                 /*录像清理模式:
									  0-默认,不清理;
									  1-自动循环覆盖;
									  2-自动条件清理;
									  3-手动条件清理;*/
	int lTurnOffDiskSize;           /*录像功能关闭磁盘余量阈值(单位:MB)*/
	int lTurnOnDiskSize;            /*录像功能开启磁盘余量阈值(单位:MB)*/
	int lBeginCleanDiskSze;         /*开始清理录像磁盘余量阈值(单位:MB)*/
	int lEndCleanDiskSze;           /*结束清理录像磁盘余量阈值(单位:MB)*/
	char cUsingFlg;                 /*是否启用:0-off;1-on;*/
	char reserved[7];

	/*定时器参数*/
	struct {
		char szTimerName[MAX_LEN_NAME+1];   /*定时器名称*/
		int lTimerType;                /*定时清理类型:
										 0-Timer;
										 1-yearly;
										 2-monthly;
										 3-weekly;
										 4-daily;
										 5-hourly;
										 6-minutely;
										 7-secondly;*/
		int lTimes;                    /*循环次数:
										 <0:永久循环;
										 =0:停止;
										 >0:执行次数;*/
		unsigned int lInterval;        /*定时器间隔,单位:秒;*/
		short nYear;                   /*2015-2099*/
		char cMonth;                   /*1-12*/
		char cWeek;                    /*1-7*/
		char cDay;                     /*1-31*/
		char cHour;                    /*0-23*/
		char Minitue;                  /*0-59*/
		char Second;                   /*0-59*/
		char padding[2];
	} stTimer;

	/*录像清理条件*/
	ST_RecordFilter stFilter;

} ST_RecordCleanStrategy;

/*录像查找条件*/
typedef struct SRecordSearch
{
	char byRecordNo;             /*按编号范围查找(预留)*/
	char byFileType;             /*按文件类型查找*/
	char byFileName;             /*按匹配文件名查找(预留)*/
	char byCreatorID;            /*按创建者ID查找*/
	char byDeviceID;             /*按设备ID查找*/
	char byChannelID;            /*按通道ID查找*/
	char byRecordTime;           /*按录像时间查找*/
	char byRecordTimeAgo;        /*查找超过一定时间的录像*/

	char byRecTimePoint;         /*按录像文件中的时间点来查找*/
	char byRecordTimeLength;     /*按录相时长查找(单位:秒)*/
	char byFileSize;             /*按文件大小查找*/
	char bySyncState;            /*按同步状态查找(预留)*/

	char byEncryptFlg;           /*按加密标志查找*/
	char reserved[3];

	unsigned int lMinRecordNo;            /*编号范围*/
	unsigned int lMaxRecordNo;            /*编号范围*/
	int lFileType;                        /*文件类型*/
	char szFileName[FILENAME_MAX_LENGTH]; /*匹配文件名*/
	unsigned int lCreatorID;              /*创建者ID*/
	unsigned int lDeviceID ;              /*设备ID*/
	unsigned int lChannelID;              /*音视频通道*/
	unsigned int lCleanBeginTime;         /*起始时间*/
	unsigned int lCleanEndTime;           /*结束时间*/
	unsigned int lTimeAgo;                /*多长时间以前,单位:秒;*/
	unsigned int lTimePoint;              /*录像文件中的时间点*/
	int lMinTimeLen;                      /*录相时间长度范围*/
	int lMaxTimeLen;                      /*录相时间长度范围*/
	ST_UInt64 stMinSize;                  /*文件大小范围(单位:Byte)*/
	ST_UInt64 stMaxSize;                  /*文件大小范围(单位:Byte)*/
	char cSyncState;                      /*同步状态:0-未同步;1-已同步;2-同步中*/
	char cEncryptFlg;                     /*加密标志:0-未加密;1-已加密;*/
	char pading[6];
} ST_RecordSearch;

/*ADD_S  BY youj@2016-03-30*/
/*服务器加密算法信息结构体*/
typedef struct SServerCryptoInfo
{
	int lCryptoNo;        /*加密算法ID*/
	char szCryptoName[MAX_LEN_CRYPTO_NAME];/*加密算法的名称；*/
	int lCryptoVal;       /*加密算法值：1-sm1；2-sm2；4-sm3；8-sm4; 16-AES; 32-RSA;*/
	int lKeyLength;       /*加密算法长度;*/
	int lPriority;        /*优先级*/
	char reserved[16];
} ST_ServerCryptoInfo;
/*ADD_E  BY youj@2016-03-30*/

/*ADD_S by shenjj@2016-10-29*/
/*设备类型信息 结构体*/
typedef struct SDeviceTypeInfo
{
	int lDevTypeID;                      /* 设备类型*/
	char szDevTypeName[MAX_LEN_NAME+1];  /* 设备类型名称*/
	char szDevName[MAX_LEN_NAME+1];      /* 设备名称 预留*/
	char szReserve[16];                  /* 保留位*/
} ST_DeviceTypeInfo;

/* 设备能力信息 结构体*/
typedef struct SDevicePrivilegeInfo
{
	int lDevPrivilegeID;					  /* 设备能力*/
	int lDevPrivilegeFlag;					  /* 设备能力标志*/
	char szDevPrivilegeName[MAX_LEN_NAME+1];  /* 设备能力名称*/
	char szModifyRange[MAX_LEN_JSON_RANGE];	  /* 可修改能力的范围 JSON表达式*/
	char szReserve[16];					      /* 保留位*/
}ST_DevicePrivilegeInfo;
/*ADD_E by shenjj@2016-10-29*/
/*设置设备镜头参数请求 数据部*/
typedef struct SCameraCfg
{
	int lRotation;                   /*0-不旋转 1-旋90 2-180 3-270*/
	int lFlip;                       /*0-不翻转 1-左右 2-上下*/
	int lCamerEnable;                /*镜头开关：1-打开 0-关闭*/
	int lCamerColorSw;               /*黑白/彩色切换：0-彩色 1-黑白*/
	int lMirror;                     /*镜像：0-关闭 1-打开*/
	char reserver[12];       /*预留*/
}ST_CameraCfg;

/*设置设备图像属性请求 数据部*/
typedef struct SVideoColorAttr
{
	int lImageScale;                         /*宽高比：0-原始 1-4:3 2-16:9 3-16:10 */
	unsigned char cBrightness;             /* 亮度:0~255        */
	unsigned char cContrast;               /* 对比度:0~255      */
	unsigned char cSaturation;             /* 饱和度:0~255      */
	unsigned char cHue;                    /* 色调:0~255        */
	unsigned char cAcutance;               /* 锐度:0~255        */
	unsigned char cWdr;                    /* wrd:0~255         */
	char  reserver[10];                     /*预留*/
}ST_VideoColorAttr;

/*移动侦测区域*/
typedef struct SRect
{
	int lLeft;               /*左顶点 X坐标值*/
	int lTop;                /*左顶点 y坐标值*/
	int lRight;              /*右顶点 X坐标值*/
	int lBottom;             /*右顶点 y坐标值*/
}ST_Rect;
/*设备移动侦测属性请求 数据部*/
typedef struct SMottonDetectAttr
{
	unsigned int byEnable;              /* 0-关闭 1-开启  */
	unsigned int byLevel;               /* 灵敏度         */
	ST_Rect stRect[MAX_MD_AREA_NUM];    /* 矩形区域    */
	char  reserver[8];                  /* 预留 */
}ST_MottonDetectAttr;

/*设备图像场景请求 数据部*/
typedef struct SVideoColorScene
{
	int  lVideoColorSceneNum;   /*设备图像场景的序号: 1：场景一   2：场景二  3：场景三*/
	char szSceneDescribe[MAX_SCENE_DESCRIBE_COUNT]; /*场景描述信息*/
	ST_VideoColorAttr stVideoColorAttr;   /*设备图像属性描述*/
}ST_VideoColorScene;


/*设备camera参数初始化*/
typedef struct DevCameraParam
{
	int lImageScale;                            /* 宽高比：0-原始 1-4:3 2-16:9 3-16:10 */
	int cBrightness;                           /* 亮度:0~255        */
	int cContrast;                             /* 对比度:0~255      */
	int cSaturation;                           /* 饱和度:0~255      */
	int cHue;                                  /* 色调:0~255        */
	int cAcutance;                             /* 锐度:0~255        */
	int cWdr;                                  /* wrd:0~255         */

	int cBriMax;                               /*亮度最大值*/
	int cBriMin;                               /*亮度最小值*/
	int cConMax;                               /*对比度最大值*/
	int cConMin;                               /*对比度最小值*/
	int cSatMax;                               /*饱和度最大值*/
	int cSatMin;                               /*饱和度最小值*/
	int cHueMax;                               /*色调最大值*/
	int cHueMin;                               /*色调最小值*/
	int cAcuMax;                               /*锐度最大值*/
	int cAcuMin;                               /*锐度最小值*/
	int cWdrMax;                               /*cWdr最大值*/
	int cWdrMin;                               /*cWdr最小值*/
}ST_DevCameraParam;

/*设备图像场景请求 数据部   --------------------新结构体，为了适配客户端那非常不标准的写死的镜头属性范围*/
typedef struct NewVideoColorScene
{
	int  lVideoColorSceneNum;   /*设备图像场景的序号: 1：场景一   2：场景二  3：场景三*/
	char szSceneDescribe[MAX_SCENE_DESCRIBE_COUNT]; /*场景描述信息*/
	ST_DevCameraParam stDevCameraParam;   /*设备图像属性描述*/
}ST_NewVideoColorScene;

#endif /*_YVCOMMON_DATA_STRUCT_H_*/
