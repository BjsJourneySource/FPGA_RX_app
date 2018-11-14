/****************************************************************************************
 * 文 件 名 :   yvcommon_def.h                                                          *
 * 项目名称 :   YVGA1207001A                                                            *
 * 模 块 名 ：  通用宏定义文件                                                          *
 * 功    能 ：  公共头文件及宏定义                                                      *
 * 操作系统 :   LINUX                                                                   *
 * 版    本 ：  Rev 0.1.1                                                               *
 *--------------------------------------------------------------------------------------*
 * 设    计 ：  liangy      '2012-8-21                                                  *
 * 编    码 ：  liangy      '2012-8-21                                                  *
 * 修    改 ：  liangy      '2012-8-31                                                  *
 ****************************************************************************************
 *- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *
 * 公司介绍及版权说明                                                                   *
 *                                                                                      *
 *               (C)Copyright 2012 YView    Corporation All Rights Reserved.            *
 *- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *
 ***************************************************************************************/
#ifndef _YVCOMMON_DEF_H_
#define _YVCOMMON_DEF_H_ 
#include <stdint.h>    /*为了采用系统定义的uint64_t*/

#ifndef TRUE
#define TRUE    1
#endif
#ifndef FALSE
#define FALSE   0
#endif

#define YV_NODATA       1
#define YV_SUCCESS      0
#define YV_FAIL         -1
#define YV_EXCEPTION    -3

/*目标类型*/
#define OBJ_TYPE_USER        0
#define OBJ_TYPE_DEVICE      1
#define OBJ_TYPE_SERVER      2
#define OBJ_TYPE_GROUP       3

/*操作类型*/
#define OPERATE_TYPE_ADD      1 /*增*/
#define OPERATE_TYPE_DEL      2 /*删*/
#define OPERATE_TYPE_UPD      3 /*改*/
#define OPERATE_TYPE_QRY      4 /*查*/

/*读配置文件时宏定义*/
#define MAX_PATH_LEN 256               /* 路径长度最大值 */
#define MAX_PATH_FILENAME_LEN 328      /* 文件路径加文件名最大长度 */

/*默认会话ID，表明发起登陆认证*/
/*用于第一次发认证请求时填充到业务头*/
/*业务头的源和目的四个ID在认证过程全为零，并在认证成功响应消息中被替换为正确的有效值*/
#define DEFAULT_SESSION_ID   0xffffffff

/*用户管理组宏定义*/
#define GROUP_ID_ADMIN       0

/*用户角色权限宏定义*/
#define SUPER_USER_ROLE_ID   0
#define ADMIN_USER_ROLE_ID   1
#define DECODER_USER_ROLE_ID 5

#define MAX_ROLE_ID_VALUE    32
#define MAX_PRIV_ID_VALUE    255

/*消息类别宏定义*/
#define MSG_REQ_TYPE_VALUE   0
#define MSG_RES_TYPE_VALUE   1
#define MSG_NFY_TYPE_VALUE   2
#define MSG_SRM_TYPE_VALUE   3

/*形如"2012-11-14 11:33:33"字符串时间长度宏定义*/
#define MAX_LEN_TIME_STR     32

/*超时时间宏定义*/
#define MILLIN_PER_SECOND           (1000000LLU)

#define MAX_TIMEOUT_MODULE_INIT     (10 * MILLIN_PER_SECOND) /*模块初始化超时时间*/
#define MAX_TIMEOUT_FSM             (10 * MILLIN_PER_SECOND) /*状态机超时时间*/
#define MAX_TIMEOUT_SESSION         (30 * MILLIN_PER_SECOND) /*会话超时时间*/
#define MAX_TIMEOUT_TRANSACTION     (10 * MILLIN_PER_SECOND)  /*事务超时时间*/
#define MAX_TIMEOUT_BUSINESS        (40 * MILLIN_PER_SECOND) /*业务超时时间*/

#define INTERVAL_KEEP_ALIVE            (10 * MILLIN_PER_SECOND)  /*通用心跳发送时间间隔*/
#define INTERVAL_KEEP_ALIVE_CLIENT     (10 * MILLIN_PER_SECOND)  /*客户端心跳发送时间间隔*/
#define INTERVAL_KEEP_ALIVE_DEVICE     (5 * MILLIN_PER_SECOND)   /*设备心跳发送时间间隔*/

#define INTERVAL_DETECT_MODULE_INIT (5 * MILLIN_PER_SECOND)   /*初始化检测时间间隔*/
#define INTERVAL_DETECT_FSM         (5 * MILLIN_PER_SECOND)   /*状态机检测时间间隔*/
#define INTERVAL_DETECT_SESSION     (15 * MILLIN_PER_SECOND)  /*会话检测时间间隔*/
#define INTERVAL_DETECT_TRANSACTION (5 * MILLIN_PER_SECOND)   /*事务检测时间间隔*/
#define INTERVAL_DETECT_BUSINESS    (20 * MILLIN_PER_SECOND)  /*业务检测时间间隔*/

#define MAX_LEN_NAME            31
#define MAX_LEN_ALIAS           64
#define MAX_LEN_MEMO            128
#define MD5_LENGTH              16
#define AES_KEY_LENGTH          MD5_LENGTH
#define AES_KEY_BITS            (8 * AES_KEY_LENGTH)
#define DATE_TIME_LENGTH        32
#define MAX_LEN_IP_ADDR         24
#define MAX_LEN_DETAILS         100
#define MAX_LEN_SIMNO           16                    /*SIM卡号码最大长度*/
#define MAX_DH_KEY_LENGTH       128
#define MAX_DH_KEY_BITS         (8 * MAX_DH_KEY_LENGTH)
#define MAX_GROUP_NAME_LEN      (64)


#define ALARM_INFO_LENGTH       256
#define SERVERMAC_LENGTH        20
#define DATA_PACKET_LENGTH      1400
#define ITEM_MAX_LEN            128       /*读取文件时文件内容中项目名字长度*/
#define CHILDSRV_MAX_LEN        128       /*级联时子服务器长度             */
#define FRAG_UNIT_SIZE          1228        /*文件分片大小*/
#define FILENAME_MAX_LENGTH     64
#define RECORD_NAME_MAX_LENGTH  96
#define PATHNAME_MAX_LENGTH     256
#define FTP_NAME_MAX_LEN        32
#define FTP_PASSWORD_MAX_LEN    32
#define MAX_AVP_CHANNELS        32
#define MAX_VOICE_GROUPS        32
#define MAX_SUB_DEVICE          16      /*设备上最大子设备个数*/
#define MAX_DECODER_STREAM      16      /*解码器客户端最大预览窗口数(同时预览流个数)*/
#if 0
#define MAX_LEN_TYPE            31          /*设备型号*/
#endif
/*ADD_S by shenjj@2016-11-2*/
#define MAX_LEN_JSON_RANGE      512     /*设备能力可修改的范围 json表达式*/
/*ADD_E by shenjj@2016-11-2*/
#define MAX_LEN_LOG             1024
/*ADD_S by youj@2017-08-2*/
#define MAX_AUDIO_TEXT_LEN      256     /*语言转文字内容长度*/
/*ADD_E by youj@2017-08-2*/

#define PASSWD_ANSWER_LENGTH    24
#define MEMO_LENGTH             48
#define TLV_USERINFO_TYPE       0
#define TLV_DEVICEINFO_TYPE     1
#define TLV_SERVERINFO_TYPE     2

/*表示时间阈值*/
#define DEFAULT_TIME_DIFF       0xFFFF

/*设备能力标志*/
#define DEVICE_ENCRYPT_FLG             1
#define DEVICE_CAN_SPEAK_FLG           (1<<1)
#define DEVICE_AUTO_CONNECT_FLG        (1<<2)
#define DEVICE_AUTO_IP_FLG             (1<<3)
#define DEVICE_SMS_FLG                 (1<<4)

/* 录像文件类型 */
#define RECORD_FILE_TYPE_UNKNOWN 0
#define RECORD_FILE_TYPE_YMS 1
#define RECORD_FILE_TYPE_GPD 2
#define RECORD_FILE_TYPE_EXD 3
#define RECORD_FILE_TYPE_MP4 4
#define RECORD_FILE_TYPE_YSS 5

/*录像文件查找类型*/
#define RECORD_SEARCH_TYPE_TIME        1
#define RECORD_SEARCH_TYPE_SIZE        (1<<1)
#define RECORD_SEARCH_TYPE_FILE_TYPE   (1<<2)
#define RECORD_SEARCH_TYPE_CHANNEL     (1<<3)

/*录像文件内容标志 */
#define DATA_FLG_GPS                  1
#define DATA_FLG_VIDEO                (1<<1)
#define DATA_FLG_AUDIO                (1<<2)
#define DATA_FLG_VITAL_SIGNS          (1<<3)
#define DATA_FLG_ALARM                (1<<4)
#define DATA_FLG_THUMB                (1<<5)
#define DATA_FLG_EX                   (1<<6)

/*VPN数据包标志*/
#define VPN_PACKET_ENCRYPT_FLG        1

/*A0设备网络参数长度*/
#define MAX_LEN_NET_PARAM 32

/*加密类型宏定义*/
#define HARD_ENCRYPT 1      /*硬加密*/
#define SOFT_ENCRYPT 2      /*软加密*/

/*加密算法宏定义*/
#define CRYPTO_SM1    1
#define CRYPTO_SM2    (1 << 1)
#define CRYPTO_SM3    (1 << 2)
#define CRYPTO_SM4    (1 << 3)
#define CRYPTO_AES    (1 << 4)
#define CRYPTO_RSA    (1 << 5)

/*加密算法名字长度*/
#define MAX_LEN_CRYPTO_NAME    32
#define MAX_CPYTO_CONUT 16       /*加密算法最大支持个数*/

#define SM4_KEY_BITS 128
#define SM4_KEY_LEN  ((SM4_KEY_BITS + 7) / 8)

/*注册流的模式 :明文、密文、兼容*/
typedef enum
{
	REGISTER_PLAIN_STREAM_MODE = 0,
	REGISTER_CIPHER_STREAM_MODE ,
	REGISTER_COM_STREAM_MODE,
}EN_REGISTER_STREAM_MODE_T;

/*消息出现的区域定义*/
typedef enum
{
	SRV_CLT = 0,
	SRV_SRV,
	SRV_DEV,
}EN_MSG_AREA_T;

/*消息分类*/
typedef enum
{
	MSG_REQ = MSG_REQ_TYPE_VALUE,
	MSG_RES = MSG_RES_TYPE_VALUE,
	MSG_NFY = MSG_NFY_TYPE_VALUE,
	MSG_SRM = MSG_SRM_TYPE_VALUE,
}EN_MSG_SORT_T;

/*消息大分类定义,按业务区别*/
typedef enum
{
	TYPE_SYS_CONFIG     = 0x40,     /*系统配置*/
	TYPE_USR_CONFIG     = 0x41,     /*用户配置*/
	TYPE_DEV_CONFIG     = 0x42,     /*设备配置*/
	TYPE_SRV_CONFIG     = 0x43,     /*服务器配置*/
	TYPE_GRP_CONFIG     = 0x44,     /*组配置*/
	TYPE_USR            = 0x45,     /*用户管理*/
	TYPE_DEV            = 0x46,     /*设备管理*/
	TYPE_SRV            = 0x47,     /*服务器管理*/
	TYPE_DEC            = 0x48,     /*设备控制*/
	TYPE_AVP            = 0x49,     /*音视频预览*/
	TYPE_VIC_TALK       = 0x4A,     /*语音对讲*/
	TYPE_VIC_GROUP      = 0x4B,     /*语音集群*/
	TYPE_STS            = 0x4C,     /*状态监控*/
	TYPE_ALM            = 0x4D,     /*报警信息*/
	TYPE_DRP            = 0x4E,     /*回放下载*/
	TYPE_SMP            = 0x4F,     /*流媒体*/
	TYPE_SAC            = 0x50,     /*安全认证*/
	TYPE_DRP_SRM        = 0x51,     /*回放下载流*/
	TYPE_COMMON         = 0x52,     /*通用消息分类*/
	TYPE_DBM            = 0x53,     /*直接数据访问消息*/
	TYPE_SYN            = 0x54,     /*同步上传*/
	TYPE_SYN_SRM        = 0x55,     /*同步上传流*/
	TYPE_VPN            = 0x56,     /*VPN业务消息分类*/
	TYPE_VPN_SRM        = 0x57,     /*VPN数据流*/
}EN_MSG_MAJORTYPE_T;


/*定义消息类型值*/
#define DEF_MSG_TYPE(AREA_T, TYPE_T, MAJOR_T, MINOR_T) \
	((AREA_T << 27) | (TYPE_T << 24) | (MAJOR_T <<16 ) | (MINOR_T))

/*文件类型枚举*/
typedef enum EFileType
{
	FILE_TYPE_YVMM = 0,
	FILE_TYPE_YMS = 1,
	FILE_TYPE_GPD,
	FILE_TYPE_EXD,
	FILE_TYPE_MP4,
	FILE_TYPE_YSS,

	FILE_TYPE_JPG = 10,
	FILE_TYPE_PNG,

	FILE_TYPE_MAX = 0xff,
} EN_FileType;

/*****以下为流数据相关的一些定义*****/
/*扩展信息类型定义*/
typedef enum 
{ 
	EXPINFOTYPE_NONE= 0, 
	/*视频编解码信息*/
	EXPINFOTYPE_VIDEOCODEC,
	/*音频编解码信息*/
	EXPINFOTYPE_AUDIOCODEC,
	/*通用数据扩展信息*/
	EXPINFOTYPE_GPDATA,
	/*业务数据扩展信息*/
	EXPINFOTYPE_EPDATA,
	EXPINFOTYPE_MAX = 0x7fff,
}EN_EXPINFOTYPE_T;

/*音视频流控值，其实不知道什么意思.. !-!*/
typedef enum
{
	RATECONTROL_CBR = 1,
	RATECONTROL_VBR = 2,
	RATECONTROL_ABR = 3,
}EN_RateControl;


/* 视频编码格式值 */
typedef enum
{
	VIDEOCODEC_H264 = 1,
	VIDEOCODEC_MPEG4 = 2,
}EN_VideoCodec;

/* 视频分辨率 */
typedef enum EResolution
{
	UNKnow_DPII = 0,
	QCIF = 1,
	CIF,
	HalfD1,
	D1,
	VGA = 5,
	HD720,
	HD1080,

	/*以上为我们目前已经使用的，以下为预留的*/

	NTSC,
	PAL,
	QNTSC = 10,
	QPAL,
	SNTSC,
	SPAL,
	FILM,
	NTSC_FILM = 15,
	SQCIF,
	CIF16,
	QQVGA,
	QVGA,
	SVGA = 20,
	XGA,
	UXGA,
	QXGA,
	SXGA,
	QSXGA = 25,
	HSXGA,
	WVGA,
	WXGA,
	WSXGA,
	WUXGA = 30,
	WOXGA,
	WQSXGA,
	WQUXGA,
	WHSXGA,
	WHUXGA = 35,
	CGA,
	EGA,
	HD480,

	DC_2K,/* Digital Cinema System Specification */
	DC_2KFLAT = 40,
	DC_2KSCOPE,
	DC_4K,
	DC_4KFLAT,
	DC_4KSCOPE,
}EN_Resolution;

/* 图片质量 */
typedef enum
{
	PICQUALITY_LOW = 1,
	PICQUALITY_MIDDLE = 3,
	PICQUALITY_HIGH = 5,
}EN_PicQuality;

/*音频编码制式*/
typedef enum
{
	ACODEC_UNKnow_A = 0,
	ACODEC_SPEEK = 1,
	ACODEC_ILBC,
	ACODEC_G723,
	ACODEC_MP3,
	ACODEC_ADPCM,
	ACODEC_AMR_NB = 6,
	ACODEC_AAC,
	ACODEC_AC3,
	ACODEC_ALAC,
	ACODEC_EAC3,
	ACODEC_FLAC = 11,
	ACODEC_MP2,
	ACODEC_OPUS,
	ACODEC_VORBIS,
	ACODEC_WAVPACK,
	ACODEC_G711A = 16,
	ACODEC_G711U = 17,

}EN_AudioCodec;

/* 音频输入方式 */
typedef enum EAudioInputStd
{
	AUDIOINPUT_MIC = 0,
	AUDIOINPUT_LINE
}EN_AudioInputStd;

/*音频采样频率*/
typedef enum 
{
	ASAMPLE_8000HZ = 1,
	ASAMPLE_11025HZ, 
	ASAMPLE_22050HZ, 
	ASAMPLE_32000HZ, 
	ASAMPLE_44100HZ, 
	ASAMPLE_47250HZ = 6, 
	ASAMPLE_48000HZ, 
	ASAMPLE_50000HZ, 
	ASAMPLE_50400HZ, 
	ASAMPLE_96000HZ, 
	ASAMPLE_192000HZ = 11, 
	ASAMPLE_2822400HZ,
	/*chenlg 新增的枚举类型*/
	ASAMPLE_24000HZ,
	ASAMPLE_4000HZ,
	ASAMPLE_6000HZ,
	ASAMPLE_12000HZ = 16,
	ASAMPLE_16000HZ
}EN_AudioSample;

/*ADD_S by liangy*/
/*更新的配置类型*/
typedef enum EConfigType
{
	EN_CONFIG_TYPE_SYSCONFIG = 0,    /*系统配置更新*/
	EN_CONFIG_TYPE_GROUP_DEVICE,     /*组设备配置更新*/
	EN_CONFIG_TYPE_ROLE_PRIVILEGE,   /*角色权限配置更新*/
	EN_CONFIG_TYPE_SERVER,           /*服务器配置更新*/
	EN_CONFIG_TYPE_SUB_DEVICE_INFO,  /*子设备信息更新*/

	EN_CONFIG_TYPE_UPSERVER_INFO = 6,       /*级联服务器信息更新*/
	EN_CONFIG_TYPE_UPSERVER_CFG,            /*级联策略更新*/

	EN_CONFIG_TYPE_LIST_INFO_ALL = 20,      /*所有信息列表更新*/
	EN_CONFIG_TYPE_LIST_INFO_SERVER,        /*(预留)服务器信息列表更新*/
	EN_CONFIG_TYPE_LIST_INFO_USER,          /*(预留)用户信息列表更新*/
	EN_CONFIG_TYPE_LIST_INFO_DEVICE,        /*(预留)设备列表信息更新*/
	EN_CONFIG_TYPE_LIST_INFO_GROUP,         /*(预留)组信息列表更新*/
	EN_CONFIG_TYPE_LIST_INFO_ACK = 29,      /*列表信息更新确认*/

	EN_CONFIG_TYPE_LIST_STATUS_ALL = 30,    /*(预留)所有状态列表更新*/
	EN_CONFIG_TYPE_LIST_STATUS_SERVER,      /*(预留)服务器状态列表更新*/
	EN_CONFIG_TYPE_LIST_STATUS_USER,        /*(预留)用户状态列表更新*/
	EN_CONFIG_TYPE_LIST_STATUS_DEVICE,      /*(预留)设备状态列表更新*/
	EN_CONFIG_TYPE_LIST_STATUS_ACK = 39,    /*(预留)列表状态更新确认*/


	EN_CONFIG_TYPE_MAX = 0x7FFFFFFF
} EN_ConfigType;
/*ADD_E by liangy*/

/*设备类型定义*/
typedef enum EDeviceType
{
	EN_DEVICE_TYPE_NONE = 0,
	EN_DEVICE_TYPE_INNER_START = 1,
	EN_DEVICE_TYPE_G6,          /*无线音视频设备,包括:
								  G6,
								  G4-PCIe,
								  眼镜Glass,
								  手表,
								  手电筒Y0-Torch,
								  */

	EN_DEVICE_TYPE_G12,         /*有线音视频设备,包括:
								  G12,
								  G4-EAV
								  */

	EN_DEVICE_TYPE_G0B,         /*无线救援设备*/

	EN_DEVICE_TYPE_A0,          /*低功耗音频设备,包括:
								  A0,
								  WA0,
								  信标,
								  Y3,
								  Y3W,
								  有线单音频I0,
								  */
	EN_DEVICE_TYPE_Z0_R,
	EN_DEVICE_TYPE_G16,         /*多通道采集传输设备*/
	EN_DEVICE_TYPE_HDMI,        /*高清单兵系列设备,包括:
								  HDMI=Y6,
								  */

	EN_DEVICE_TYPE_G4F,         /*全功能音视频设备,包括:
								  G4F=Y4,
								  */
	EN_DEVICE_TYPE_G4,          /*无线音视频单兵设备*/

	EN_DEVICE_TYPE_INNER_END = 0xffff,
	EN_DEVICE_TYPE_MAX = 0x7fffffff,
}EN_DeviceType;

/* 云台类型定义*/
typedef enum EPTZType
{
	EN_PTZ_TYPE_NONE = 0,
	EN_PTZ_TYPE_MAX = 0x7fffffff,
}EN_PTZType;            

/*检查是否包含FLAGT能力标志*/
#define CHECK_FLG(VAR_FLAG, FLAG_T) \
	((VAR_FLAG & FLAG_T) == FLAG_T ? YV_SUCCESS : YV_FAIL)

/*检查是否包含FLAG位标志, 简化上面的宏*/
#define BOOL_CHECK_FLG(VAR_FLAG, FLAG_T) \
	(((VAR_FLAG) & (FLAG_T)) == (FLAG_T))

/*用于流媒体数据中流数据类型的标志*/

/* 流数据类型 */
typedef enum
{
	STREAM_TYPE_VIDEO = 1,
	STREAM_TYPE_AUDIO = 2,
	STREAM_TYPE_GPDATA = 3,
	STREAM_TYPE_EPDATA = 4
}EN_StreamType;

/* 通用数据具体类型编码定义 */
typedef enum
{
	GP_DATA_TYPE_UNKNOWN = 0,
	GP_DATA_TYPE_HEALTH = 3,        /*生命体征*/
	GP_DATA_TYPE_ENVIRONMENT = 4,   /*环境参数*/
	GP_DATA_TYPE_MAX = 0x7fffffff
}GP_DATA_TYPE_T;

/*数据类型枚举*/
typedef enum
{
	DATA_TYPE_UNKNOWN = 0,
	DATA_TYPE_VIDEO = 1,    /*视频*/
	DATA_TYPE_AUDIO =2,     /*音频*/
	/*各种通用数据类型轨道*/
	DATA_TYPE_GPS,          /*GPS*/
	DATA_TYPE_GAS,          /*瓦斯浓度*/
	DATA_TYPE_ENVIRONMENT,  /*环境参数*/
	DATA_TYPE_HEALTH,       /*生命体征*/
	DATA_TYPE_PTZ,          /*485串口*/
	DATA_TYPE_SERIAL232,    /*232串口*/
	DATA_TYPE_ALARM,        /*报警*/
	DATA_TYPE_SENSOR,       /*传感器*/

	DATA_TYPE_MAX = 0x7f,
} EN_DATA_TYPE_T;


/*音频输入类型*/
typedef enum
{
	AUDIO_INPUT_SRC_HDMI = 0,  /*通过HDMI采集进来的伴音(HDMI)*/
	AUDIO_INPUT_SRC_MAIN = 1,  /*板载mic的声音(PTT)*/
	AUDIO_INPUT_SRC_HP   = 2,  /*外置耳麦线上mic的声音(CVBS)*/
	/*chenlg 添加CVBS音频源输入，其实理论上是和hp是同一个，但是由于可以使用合适的各自参数，所以也分为一类*/
	AUDIO_INPUT_SRC_CVBS   = 3,/*外接CVBS的声音(CVBS)*/
} EN_AudioInputSrc;

/*音频输入类型*/
typedef enum
{
	VIDEO_INPUT_SRC_HDMI = 0,  /*(HDMI)*/
	VIDEO_INPUT_SRC_CVBS = 1,  /*(CVBS)*/
	VIDEO_INPUT_SRC_HD_SDI   = 2,  /*(HD_SDI)*/
} EN_VideoInputSrc;

/*回放类型*/
typedef enum
{
	PLAYBACK_TYPE_DEFAULT = 0,/*默认回放，即回放文件中所有的内容*/
	PLAYBACK_TYPE_VIDEO = 1, /*视频*/
	PLAYBACK_TYPE_AUDIO = 2, /*音频*/
	PLAYBACK_TYPE_AV    = 3, /*视音频*/
} EN_PlaybackType;

/*VPN相关 加解密线程任务类型*/
typedef enum
{
	E_TYPE_ENC = 0, /*加密*/
	E_TYPE_DEC,     /*解密*/
} EN_EncEventType;

/*设备状态信息类型枚举*/
typedef enum
{
	TYPE_UNKNOWN = 0,          /*未知类型*/
	TYPE_DEVICE_BASESTATUS,    /*设备基本状态信息(兼容就版本设备状态查询消息CLT_REQ_DEV_STATUS_INFO)*/
	TYPE_DEVICE_CHANNELSTATUS, /*设备通道状态信息*/
} EN_DevStatusInfoType;

/*集中侦控平台案件对象类型枚举*/
typedef enum ECaseObjectType
{
	EN_CASE_OBJECT_UNKNOWN = 0,         /*未指定*/
	EN_CASE_OBJECT_SCOUT = 1,           /*侦查员*/
	EN_CASE_OBJECT_SUSPECT = 2,         /*嫌疑人*/
	EN_CASE_OBJECT_CAR = 3,             /*车辆*/
	EN_CASE_OBJECT_BUILDING = 4,        /*建筑物*/
	EN_CASE_OBJECT_ROOM = 5,            /*大楼房间*/
	EN_CASE_OBJECT_OTHER = 6,          /*其它*/

	EN_CASE_OBJECT_TYPE_MAX = 0x7fffffff
} EN_CaseObjectType;

/*集中侦控平台案件对象证件编号类型枚举*/
typedef enum ECaseObjectIDType
{
	EN_CASE_OBJECT_ID_UNKNOWN = 0,          /*未指定*/
	EN_CASE_OBJECT_ID_CARD = 1,             /*身份证*/
	EN_CASE_OBJECT_ID_DRIVER_LICENSE = 2,   /*驾驶证*/
	EN_CASE_OBJECT_ID_CAR_NO = 3,           /*车牌号*/
	EN_CASE_OBJECT_ID_ROOM_NO = 4,          /*房间号*/
	EN_CASE_OBJECT_ID_BUILDING = 5,         /*建筑物编号*/
	EN_CASE_OBJECT_ID_OTHER = 6,           /*其它*/

	EN_CASE_OBJECT_ID_TYPE_MAX = 0x7fffffff
} EN_CaseObjectIDType;

/*工作模式枚举*/
typedef enum
{
	EN_WORKMODE_ALLWORKS = 0,  /*全功能*/
	EN_WORKMODE_UNLINK = 1,    /*本地*/
	EN_WORKMODE_SLEEP =2,     /*休眠*/
	EN_WORKMODE_SILENT = 3,      /*静默*/

	EN_WORKMODE_MAX = 0x7f
} EN_WORKMODE_TYPE_T;

/*设备访问模块宏定义*/
#define MAX_RECORD_NUM       1024     /*录相最大个数*/
#define MAX_RECORD_NAME_LEN  128      /*录相名字最大长度*/
#define MAX_PTZ_CMD_LEN      20       /*PTZ命令最大长度*/

/*采集相关宏定义*/
/*采集模块支持的最大摄像头数目*/
#define AVC_MAX_CAMERA_NUM 8
/*支持的分辨率的最大种类*/
#define CAMERA_MAX_RESOLUTION_NUM 40

/*报警级别*/
#define     YV_EMERG     0            /*致命情况，设备无法使用*/
#define     YV_ALERT     1            /*警戒情况，必须立即采取措施*/
#define     YV_ERROR     2            /*错误情况，需要告知用户*/
#define     YV_CRITIAL   3            /*临界情况，需要记录在数据库但不用通知用户*/
#define     YV_NORMAL    0xff         /*正常情况，无报警*/

/*语音对讲集群类型*/
#define MSPVIC_GROUP_TYPE_VOICE     0    /*普通语音集群*/
#define MSPVIC_GROUP_TYPE_SOS       1    /*SOS集群*/

/*下载回放业务类型*/
#define MSPDRP_DOWNLOAD        0 /*下载业务*/
#define MSPDRP_SRV_UPLOAD      1 /*服务器上传业务*/
#define MSPDRP_DEV_UPLOAD      2 /*设备上传业务*/

/*同步方式*/
#define SYNC_TYPE_ALL          0 /*全部同步(预留,暂不支持)*/
#define SYNC_TYPE_OPT          1 /*把符合条件的文件同步(指定大小,通道和时间段等,不超过24小时)*/
#define SYNC_TYPE_LIST         2 /*指定文件列表进行同步(最大不超过50个)*/

#if 0
/*客户端查询音视频文件列表最大返回个数*/
#define MSPDRP_MAX_RECORD_NUM  25
#define FTP_MAX_USER 128
#endif
#define MAX_DRP_QUERY_FILE_COUNT    500    /*回放下载查询结果最大文件数*/
#define MAX_DRP_PAGE_FILE_COUNT     50     /*回放下载查询业务每一分页最大文件数*/

#define MAX_LIST_PAGE_INFO_COUNT    (50)     /*列表数据每一分页最大个数(通用大小)*/

/********************************************************************************/
#define MAX_LEN_GPS_STR     20          /*GPS成员信息的最大长度*/
#define MAX_LEN_PHONENUM_STR 16 /*电话号码字符串最大长度*/
#define MAX_LEN_IP_STR 20               /*IP地址字符串最大长度*/
#define MAX_LEN_DEVALIAS_STR 64 /*采集设备别名字符串最大长度*/
#define MAX_LEN_DEVFILENAME_STR 128 /*设备文件字符串最大长度*/
#define MAX_LEN_IFNAME_STR 8 /*PPP拨号设备名最大长度*/
#define MAX_LEN_LIBNAME_STR 64 /*动态库名称最大长度*/
#define MAX_LEN_EXPANSIONDEVNAME_LEN 64 /*扩展设备名称最大长度*/

#define MAX_SUPPORT_3G_DEV_COUNT 8      /*最多支持的3G模块个数*/
#define MAX_SUPPORT_EXPANSIONDEV_COUNT 16 /*扩展外设管理支持的最多的扩展外设个数*/
#define MAX_SUPPORT_SMS_PHONE_COUNT 16 /*电话控制最多支持的电话号码个数*/
#define MAX_SUPPORT_STREAM_COUNT 24 /*最多支持的流个数*/
#define SETTING_INUSE               1       /*配置为启用*/
#define SETTING_NOTINUSE        0       /*配置为非启用*/

#define EXPANSION_DEVTYPE_UNKNOWN   0   /*扩展外设不明类型*/
#define EXPANSION_DEVTYPE_POSITION  1   /*扩展外设定位类型*/
#define EXPANSION_DEVTYPE_STREAM    2   /*扩展外设流类型*/
#define EXPANSION_DEVTYPE_COMMON    3   /*扩展外设通用类型*/

#define INTERVAL_SOS_ALARM     (5 * MILLIN_PER_SECOND)    /*SOS报警间隔为5秒*/
/**********************************************************************************/
/*VPN业务相关红定义*/
#define VPN_DEVICE_MAX_NUM        5000  /* 最大VPN设备管理个数*/
#define VPN_DEVICE_ONLINE_MAX_NUM 500   /* 最大VPN设备在线数*/
#define ENCRYPT_THREAD_MAX_NUM    128   /* 加解密线程启动的最大个数*/
#define NETWORK_SEGMENT_LENGTH    24    /* 网段的长度*/
#define SYSTEM_CMD_LENGTH         128   /* 命令长度*/
#define NIC_NAME_MAX_LENGTH       16    /* 网卡名称长度*/
#define NET_STANDARD_NAME_LENGTH  16    /* 3G模块制式名称长度*/
#define OEM_STRINE_MAX_LENGTH     64    /* 设备标识信息长度*/
#define KEY_MAX_LENGTH            128   /* 加解密密钥最大长度*/
#define VPN_MAX_MSG_LENGTH        2048  /* 最大消息长度定义*/
#define VPN_SET_CMD_LENGTH        32    /* 配置命令最大长度*/

/***********************************************************************************/
/*同步上传宏定义*/
/*宏定义*/
#define MAX_UPLOAD_BLOCK_NUM 10240 /*上传最大分块个数*/
#define DEFAULT_TRANS_RATE 1000    /*1000KBps*/

#define SYN_BLOCKINFO_SAME   0  /*分块信息相同*/
#define SYN_BLOCKINFO_DIFF   -1 /*分块信息不相同*/

/*同步归档响应消息，成功状态码定义*/
#define SYN_SYNC_DUPLICATE   0   /*已同步，无需同步*/
#define SYN_SYNC_SUCCESS     1   /*可以同步*/

/*文件上传响应消息 成功状态码定义*/
#define SYN_UPLOAD_FILE_EXIST   0 /*文件已存在无需上传*/
#define SYN_UPLOAD_FILE_SUCCESS 1 /*响应成功，可以上传*/

/*分块上传响应消息 成功状态码定义*/
#define SYN_PREBLOCK_RECV_SUCCESS 0 /*上一分块上传成功*/
#define SYN_FILE_UPLOAD_SUCCESS   1 /*文件传输成功*/

/*HDMI开发新增的枚举值定义*/

/*网卡地址协议类型*/
#define NIC_ADD_PROTO_DHCP      0
#define NIC_ADD_PROTO_STATIC    1

/*wifi认证模式字符串定义*/
#define STR_WIFI_AUTHTYPE_WPA_PSK "WPA-PSK"
#define STR_WIFI_AUTHTYPE_WEP "WEP"
#define STR_WIFI_AUTHTYPE_NONE "NONE"

/*视频参数修改掩码定义*/
#define VIDEO_PARAM_CHG_CODEC           (1)
#define VIDEO_PARAM_CHG_DPI             (1<<1)
#define VIDEO_PARAM_CHG_FRAMERATE       (1<<2)
#define VIDEO_PARAM_CHG_BITERATE        (1<<3)
#define VIDEO_PARAM_CHG_RATECONTROL     (1<<4)
#define VIDEO_PARAM_CHG_GOP             (1<<5)
#define VIDEO_PARAM_CHG_AUTOBITERATE    (1<<6)
#define VIDEO_PARAM_CHG_AUTOFRAMERATE   (1<<7)
/*音频参数修改掩码定义*/
#define AUDIO_PARAM_CHG_CODEC           (1)
#define AUDIO_PARAM_CHG_INPUT           (1<<1)
#define AUDIO_PARAM_CHG_SAMPLE          (1<<2)
#define AUDIO_PARAM_CHG_CHANNELNUM      (1<<3)
#define AUDIO_PARAM_CHG_BITWIDTH        (1<<4)
#define AUDIO_PARAM_CHG_BITRATE         (1<<5)
#define AUDIO_PARAM_CHG_VOLUME          (1<<6)

/*通信模块策略控制掩码定义*/
#define STM_POLICY_MATCH_BY_SESSION     (1)
#define STM_POLICY_MATCH_BY_PEERID      (1 << 1)
#define STM_POLICY_SAVE                 (1 << 2)

/***********************************数据访问模块错误码***********************************/
#define ERR_DBM_UNKNOW               12001    /*其它错误*/
#define ERR_DBM_EXCUTE               12002    /*数据库语句执行出错*/
#define ERR_DBM_FIELD_NULL           12003    /*数据不存在或者字段为空*/
#define ERR_DBM_FIELD_EXISTED        12004    /*数据字段已存在*/
#define ERR_DBM_DEL_GROUP_ADMIN      12005    /*不允许删除管理组(默认配置)*/
#define ERR_DBM_SUB_DEV_NULL         12006    /*设备子设备信息记录不存在*/
#define ERR_DBM_LOCAL_SERVER         12007    /*禁止修改本地服务器信息*/
#define ERR_DBM_SUB_DEV_NUM          12008    /*设备子设备信息数异常*/
#define ERR_DBM_UNKNOW_MSG_PROC      12009    /*数据库未找到该请求的处理*/
#define ERR_DBM_UNKNOW_OPERATE_TYPE  12010    /*不支持的操作类型*/
#define ERR_DBM_UNKNOW_OBJ_TYPE      12011    /*不支持的操作对象类型*/
#define ERR_DBM_GROUPNAME_EXISTED    12012    /*要添加的分组名称已存在*/
#define ERR_DBM_GROUP_CASE_INUSE     12013    /*该分组下案件正在使用，禁止删除该分组*/
#define ERR_DBM_GROUP_ITEM_INUSE     12014    /*该分组下[用户/设备/服务器]正在使用，禁止删除该分组*/
#define ERR_DBM_DEL_SUB_SERVER       12015    /*不支持删除级联组信息*/
#define ERR_DBM_MOD_SUB_SERVER       12016    /*不支持修改级联组信息*/
#define ERR_DBM_MOD_SUB_SERVER_DEL   12017    /*不支持删除级联关系（有关联用户，先移除）*/
/*ADD_S by shenjj @2016-11-16*/
#define ERR_DBM_UNLINK				 12018	  /*数据库未连接*/
/*ADD_E by shenjj @2016-11-16*/
/* ADD_S by yangn @2017-1-6 */
#define ERR_DBM_DEVICE_GROUP_EXISTED 12019    /*该设备已分配到其他组下*/
#define ERR_DBM_DEVICE_CASE_EXISTED  12020    /*该设备已分配到其他案件下*/
#define ERR_DBM_DEVICE_INFO_EXISTED  12021    /*设备已存在*/
#define ERR_DBM_USER_INFO_EXISTED    12022    /*用户已存在*/
#define ERR_DBM_SERVER_INFO_EXISTED  12023    /*服务器已存在*/
/* ADD_E by yangn @2017-1-6 */

/***********************************音视频预览错误码***********************************/
#define ERR_AVP_UNKNOWN              13001    /*未知错误*/
#define ERR_AVP_FORWARD              13002    /*转发错误*/
#define ERR_AVP_RECORD_CLOSE         13003    /*服务器录像功能关闭*/
#define ERR_AVP_RECORD_UNKNOWN       13004    /*预览录像未查到*/

/*************管理组错误码(包括用户管理、设备管理、服务器管理及安全认证模块)*************/
#define ERR_LOGIN_UNKNOWN            21001    /*登录未知错误*/
#define ERR_LOGIN_ACCOUNT            21002    /*账号不存在*/
#define ERR_LOGIN_PASSWD             21003    /*口令错误*/
#define ERR_LOGIN_TIMEOUT            21004    /*认证超时*/
#define ERR_LOGIN_RELOGIN            21005    /*重复登陆*/
#define ERR_LOGIN_LIMIT              21006    /*超过在线上限*/

#define ERR_TRANSACTION_UNKNOWN      21007    /*业务未知错误*/
#define ERR_TRANSACTION_PRIVILEGE    21008    /*没有业务权限*/
#define ERR_TRANSACTION_ADMIN        21009    /*需要管理员权限*/
#define ERR_TRANSACTION_TIMEOUT      21010    /*业务超时错误*/
#define ERR_TRANSACTION_DUPLICATE    21011    /*业务重复请求*/
#define ERR_TRANSACTION_LIMIT        21012    /*超过业务处理上限*/
#define ERR_TRANSACTION_DEV_GROUP    21013    /*不属于本组设备*/
#define ERR_TRANSACTION_UNKNOWN_DOBJ    21014    /*目标不可达*/

#define ERR_LOGIN_SERVER_LOOPBACK    21030    /*级联服务器回环*/

#define ERR_DH_UNKNOWN               21100    /*DH密钥交换未知错误*/
#define ERR_DH_TIMEOUT               21101    /*DH密钥交换超时*/
#define ERR_DH_DUPLICATE             21102    /*DH密钥交换已存在,稍后再试*/
#define ERR_DH_KEY_LENGTH            21103    /*不支持的密钥长度*/
#define ERR_DH_KEY_NULL              21104    /*密钥为空,尚未生成*/
#define ERR_DH_COMPUTE               21105    /*DH密钥计算失败*/

#define ERR_CRYPTO_TYPE              21106      /*不支持的加密算法类型*/

/*加密算法名字长度*/
#define MAX_LEN_CRYPTO_NAME    32
/***********************************设备控制模块错误码***********************************/
#define ERR_DEC_UNKNOWN              22001    /*设备未知错误*/
#define ERR_DEC_POWER_ON_TIMEOUT     22002    /*设备启动请求超时*/
#define ERR_DEC_POWER_OFF_TIMEOUT    22003    /*设备关闭请求超时*/
#define ERR_DEC_SLEEP_TIMEOUT        22004    /*设备休眠请求超时*/
#define ERR_DEC_VIDEO_MODE_TIMEOUT   22005    /*视频模式请求超时*/
#define ERR_DEC_AUDIO_MODE_TIMEOUT   22006    /*音频模式请求超时 */
#define ERR_DEC_VIDAUD_MODE_TIMEOUT  22007    /*音视频模式请求超时*/
#define ERR_DEC_VIDEO_TIMEOUT        22008    /*视频控制请求超时*/
#define ERR_DEC_AUDIO_TIMEOUT        22009    /*音频控制请求超时*/
#define ERR_DEC_LOCK_PTZ_TIMEOUT     22010    /*锁定云台请求超时*/
#define ERR_DEC_UNLOC_PTZ_TIMEOUT    22011    /*解锁云台请求超时*/
#define ERR_DEC_CTRL_PTZ_TIMEOUT     22012    /*控制云台请求超时*/
#define ERR_DEC_BITRATE_LIMIT        22013    /*视频控制码率值超出控制范围*/
#define ERR_DEC_AUDIO_ERR            22014    /*音频控制参数出错*/
#define ERR_DEC_PTZ_LOCK_BY_OTHER    22015    /*设备已被其他用户锁定*/
#define ERR_DEC_PTZ_LOCK_BY_YOURSELF 22016    /*设备已被自身用户锁定*/
#define ERR_DEC_REBOOT_TIMEOUT       22017    /*设备重启请求超时*/
#define ERR_DEC_REBOOT_SOS           22018    /*设备处于SOS状态拒绝重启*/
#define ERR_DEC_PTZ_PROTOCOLS        22019    /*读取云台协议出错*/
#define ERR_DEC_CONFIG_INVALID       22020    /*设备配置无效*/
#define ERR_DEC_POWER_OFF_SOS        22021    /*设备处于SOS状态拒绝关机*/
#define ERR_DEC_DBM_UNKNOWN          22022    /*设备数据库操作出错*/
#define ERR_DEC_PHONE_LIMIT          22023    /*设备激活手机号超过上限*/
#define ERR_DEC_PHONE_NULL           22024    /*要操作的设备手机号不存在*/
#define ERR_DEC_DEVICEID_UNMATCH     22025    /*设备ID不匹配*/
#define ERR_DEC_PARAM_INVALID        22026    /*设置的参数无效*/
#define ERR_DEC_NET_PARAM            22027    /*网络参数异常*/
#define ERR_DEC_START                22028    /*在正常模式下收到设备开机消息*/
#define ERR_DEC_STOP                 22029    /*在关机模式下收到设备关机消息*/
#define ERR_DEC_ADD_PHONE_EXIST      22030    /*要添加的手机号已存在*/
#define ERR_DEC_MIC_AGC_VALUE        22031    /*麦克增益值错误*/
#define ERR_DEC_MIC_NO               22032    /*麦克编号错误*/
#define ERR_DEC_NET_KEY              22033    /*网络加密方式错误*/
#define ERR_DEC_NET_OPERATE          22034    /*wifi操作类型错误*/
#define ERR_DEC_AUDIO_SRC            22035    /*音频切换源参数错误*/
#define ERR_DEC_PHONE_EXIST          22036    /*控制号码已经存在，且配置相同*/

/***********************************下载回放模块错误码***********************************/
#if 0
#define ERR_DRP_QUERY_TIMEOUT        23001    /*查询音视频列表超时*/
#define ERR_DRP_GENERATE_FTPUSER     23002    /*生成ftp用户名和密码出错*/
#define ERR_DRP_INSERT_FTPUSER       23003    /*添加ftp用户出错*/
#define ERR_DRP_OPEN_RECDIR          23004    /*打开录像文件目录失败*/
#define ERR_DRP_SEARCH_MSLIST        23005    /*查询音视频文件列表出错*/
#define ERR_DRP_CONNECT_SERVER       23006    /*ftp连接服务出错*/
#define ERR_DRP_LOGIN_SERVER         23007    /*ftp登录失败*/
#define ERR_DRP_FILE_TRANSFER        23008    /*ftp文件传输出错*/
#define ERR_DRP_GETFILEINFO_TIMEOUT  23009    /*查询录像文件信息超时*/
#define ERR_DRP_GETFILEINFO_FAIL     23010    /*查询录像文件信息失败*/
#define ERR_DRP_VIDEODIR_NOTEXIST    23011    /*服务器录像文件目录不存在*/
#endif

#define ERR_DRP_FILENAME_UNKNOWN     23000    /*文件名格式无法识别*/
#define ERR_DRP_FILE_TYPE_UNKNOWN    23001    /*文件类型无法识别*/

#define ERR_DRP_QUERY_UNKNOWN        23101    /*查询未知错误*/
#define ERR_DRP_QUERY_TIMEOUT        23102    /*查询超时*/
#define ERR_DRP_QUERY_LIMIT          23103    /*超过查询业务上限*/
#define ERR_DRP_QUERY_DUPLICATE      23104    /*查询业务重复请求*/
#define ERR_DRP_QUERY_SESSION_NULL   23105    /*查询会话不存在*/
#define ERR_DRP_QUERY_PAGE_OUT       23106    /*请求分页超出范围*/
#define ERR_DRP_QUERY_FILELIST_OUT   23107    /*文件列表超过500条无法显示，请缩小查询范围*/
#define ERR_DRP_QUERY_SEARCH_TIME    23108    /*查询条件时间错误*/
#define ERR_DRP_QUERY_SEARCH_SIZE    23109    /*查询条件大小错误*/

#define ERR_DRP_QUERY_LIMIT_24HOURS  23120    /*查询时间超过24小时范围*/

#define ERR_DRP_DOWNLOAD_UNKNOWN           23201 /*下载未知错误*/
#define ERR_DRP_DOWNLOAD_TIMEOUT           23202 /*下载会话超时*/
#define ERR_DRP_DOWNLOAD_LIMIT             23203 /*超过下载上限*/
#define ERR_DRP_DOWNLOAD_DUPLICATE         23204 /*下载业务重复请求*/
#define ERR_DRP_DOWNLOAD_SESSION_NULL      23205 /*下载会话不存在*/
#define ERR_DRP_DOWNLOAD_FILE_MULTI        23206 /*一个请求没处理完又来另一个请求*/
#define ERR_DRP_DOWNLOAD_FILE_NULL         23207 /*没有下载文件却请求分块数据*/
#define ERR_DRP_DOWNLOAD_BLOCK_NULL        23208 /*当前文件下载没有对应的分块数据*/
#define ERR_DRP_DOWNLOAD_FILE_NOT_EXIST    23209 /*当前下载文件不存在*/
#define ERR_DRP_DOWNLOAD_FILE_TIMEOUT      23210 /*下载文件超时*/
#define ERR_DRP_DOWNLOAD_FILEBLOCK_TIMEOUT 23211 /*下载文件分块超时*/
#define ERR_DRP_DOWNLOAD_BLOCK_TRANSMIT    23212 /*分块传输失败*/
#define ERR_DRP_DOWNLOAD_MALLOC            23213 /*内存空间分配失败*/
#define ERR_DRP_DOWNLOAD_ADD_TRANSACTION   23214 /*添加事物节点失败*/
#define ERR_DRP_DOWNLOAD_SEND_MSG          23215 /*消息发送失败*/
#define ERR_DRP_DOWNLOAD_FILE_PROCSTATE    23216 /*文件处理状态错误*/
#define ERR_DRP_DOWNLOAD_FILE_DECRYPT      23217 /*文件解密失败*/

#define ERR_DRP_PLAYBACK_UNKNOWN        23301    /*回放业务未知错误*/
#define ERR_DRP_PLAYBACK_LIMIT          23302    /*超过回放业务上限*/
#define ERR_DRP_PLAYBACK_DUPLICATE      23303    /*回放业务重复请求*/
#define ERR_DRP_PLAYBACK_CREAT_FAIL     23304    /*回放业务创建失败*/
#define ERR_DRP_PLAYBACK_SESSION_NULL   23305    /*回放会话不存在*/
#define ERR_DRP_PLAYBACK_FILENAME_ERROR 23306    /*文件名错误*/
#define ERR_DRP_PLAYBACK_VIDEO_FILEPATH 23307    /*录像文件路径获取失败*/
#define ERR_DRP_PLAYBACK_NO_SUPPORT     23308    /*回放类型不支持*/
#define ERR_DRP_PLAYBACK_FILE_READFINI  23309    /*文件读取完毕*/
#define ERR_DRP_PLAYBACK_FILE_NOTEXIST  23310    /*录像文件不存在*/
#define ERR_DRP_PLAYBACK_TIME_ILLEGAL   23311    /*回放时间不合法*/
#define ERR_DRP_PLAYBACK_RECORD_ENCRYPT 23312    /*无法在线回放加密录像*/

#define ERR_DRP_DEL_UNKNOW              23400    /*删除文件未知错误*/
#define ERR_DRP_DEL_TIMEOUT             23401    /*删除文件超时*/
#define ERR_DRP_DEL_FILE_DB_INDEX       23407    /*删除文件数据库索引失败*/
#define ERR_DRP_DEL_FILE_DISK_DATA      23408    /*删除文件磁盘数据失败*/

/***************************************通用错误码***************************************/
#define ERR_MSG_UNCOMPLETED          24001    /*消息不完整错误*/
#define ERR_COMMON_UP_SERVER_NULL    24002    /*未配置上级服务器*/
#define ERR_COMMON_SERVER_UPSUPPORT  24003    /*服务器不支持*/
#define ERR_COMMON_CLIENT_UPSUPPORT  24004    /*客户端不支持*/
#define ERR_COMMON_DEVICE_UPSUPPORT  24005    /*设备不支持*/

#define ERR_COMMON_UNKNOW_MSG_PROC   24010    /*未找到该消息的处理*/
#define ERR_COMMON_MSG_LENGTH        24011    /*消息长度检查失败*/

/*ADD_S by yangn @2016-10-24*/
#define ERR_COMMON_USR_NUM_LIMIT     24012    /*超过最大管理用户上限*/
#define ERR_COMMON_DEV_NUM_LIMIT     24013    /*超过最大管理设备上限*/
#define ERR_COMMON_SER_NUM_LIMIT     24014    /*超过最大管理服务器上限*/
/*ADD_E by yangn @2016-10-24*/

/***********************************语音对讲错误码***************************************/
#define ERR_VIC_UNKNOWN              25001    /*未知错误*/
#define ERR_VIC_REFUSE               25002    /*拒绝加入集群*/
#define ERR_VIC_QUIT                 25003    /*用户主动退出*/
#define ERR_VIC_TIMEOUT              25004    /*等待超时*/
#define ERR_VIC_ADMIN                25005    /*必须是集群管理员才能执行*/
#define ERR_VIC_GROUP_NULL           25006    /*集群不存在*/
#define ERR_VIC_ADMIN_REMOVE         25007    /*管理员移除*/
#define ERR_VIC_ADMIN_END            25008    /*管理员结束集群*/
#define ERR_VIC_OFFLINE              25009    /*集群成员下线*/
#define ERR_VIC_RELOCK               25010    /*不允许重复锁定*/
#define ERR_VIC_RELEASE              25011    /*用户主动释放锁定*/
#define ERR_VIC_SMP_FAIL             25012    /*流媒体响应失败*/
#define ERR_VIC_LIMIT_GROUP          25013    /*集群数目超过系统允许最大限制*/
#define ERR_VIC_LIMIT_MEMBER         25014    /*成员数目超过系统允许最大限制*/
#define ERR_VIC_RECREATE             25015    /*重复创建集群*/
#define ERR_VIC_DOWN_SERVER          25016    /*下级成员必须属于同一个下级服务器*/
#define ERR_VIC_SOS_MEMBERS          25017    /*SOS集群加入同组在线成员失败*/
#define ERR_VIC_MUTEX_GROUP          25018    /*从旧的集群中退出*/
#define ERR_VIC_GROUP_OBJ_NULL       25019    /*集群成员不存在*/
#define ERR_VIC_GROUP_IDLE           25020    /*集群空闲*/
#define ERR_VIC_GEN_GROUP_ID         25021    /*生成集群ID失败*/
#define ERR_VIC_DEVICE_LIMIT         25022    /*设备没有权限*/
#define ERR_VIC_TALK_TO_SELF         25023    /*不允许和自己对讲*/
#define ERR_VIC_TIMEOUT_INVITE       25024    /*邀请成员对讲超时*/
#define ERR_VIC_TIMEOUT_KEEP_ALIVE   25025    /*成员语音心跳超时*/

/*设备端错误码定义*/
#define ERR_DEV_STREAM_NOT_EXIST    26001   /*流不存在*/
#define ERR_DEV_STREAM_CLOSE        26002   /*设备流主动关闭*/
#define ERR_DEV_STREAM_TIMEOUT      26003   /*设备流超时关闭*/

#define ERR_DEV_DISK_LOCK           26011   /*磁盘操作锁定,请稍后重试*/
#define ERR_DEV_RECORD_PATH_NULL    26012   /*设备录像目录不存在*/
#define ERR_DEV_PLAYBACK_LIMIT      26013   /*超出设备同时支持回放的客户端上限*/

/*设备配置通用错误码定义*/
#define ERR_DEV_DB_READ_FAILED      26100   /*数据库读取错误*/
#define ERR_DEV_DB_WRITE_FAILED     26101   /*数据库写入错误*/
#define ERR_DEV_DB_APPLY_FAILED     26102   /*参数应用错误*/

/*设备视频源切换错误码*/
#define ERR_DEV_VIDEO_SWITCH_TRY_AGAIN          26110   /*忙*/
#define ERR_DEV_VIDEO_SWITCH_NOT_SUPPORT        26111   /*不支持的切换类型*/
#define ERR_DEV_VIDEO_SWITCH_CAMERA_EXCEPTION   26112   /*摄像头异常*/
#define ERR_DEV_VIDEO_SWITCH_AVC_EXCEPTION      26113   /*采集系统异常*/
#define ERR_DEV_VIDEO_SWITCH_AVC_NORESPONSE     26114   /*采集系统无响应*/

/***********************************VPN模块错误码***************************************/
#define ERR_GET_VPN_DEVINFO          27001  /*VPN设备信息获取失败*/
#define ERR_BUILD_REPLY_INTERCOMMHDR 27002  /*响应消息业务头创建失败*/
#define ERR_REQ_TIMEOUT              27003  /*请求处理超时错误*/

#define ERR_VPNDEVICE_NUM_LIMIT      27005  /*超过支持设备数上限*/
#define ERR_LOAD_VPNDEVICE_INFO      27006  /*vpn设备信息加载失败*/
#define ERR_REPEAT_ADD_VPNDEVICE     27007  /*vpn设备重复添加*/
#define ERR_DEL_ROUTENODE_INFO       27008  /*路由节点信息删除失败*/
#define ERR_VPN_DEVINFO_NOTEXIST     27009  /*要保存的VPN设备信息不存在*/
#define ERR_QUERY_DEV_KEY            27010  /*设备密钥查询失败*/

#define ERR_VPN_ADD_NEFILTERRULE      27004  /*添加NetFilter规则失败*/
#define ERR_KEY_LENGTH_ILLEGAL        27011  /*密钥长度不合法*/
#define ERR_VPN_GET_SRV_VPNCONFIG     27012  /*获取服务器VPN配置失败*/
#define ERR_VPN_DEVICE_OFFLINE        27013  /*VPN设备不在线*/
#define ERR_VPN_EXCISE_DEVNODE        27014  /*摘除VPN设备信息节点失败*/
#define ERR_VPN_ADD_DEVNODE           27015  /*添加VPN设备信息节点失败*/
#define ERR_VPN_SEARCH_ROUTENODE      27016  /*未找到对应设备的路由*/
#define ERR_VPN_IP_TRANSFORM          27017  /*IP地址转换失败*/
#define ERR_VPN_EXCISE_ROUTENODE      27018  /*摘除路由节点失败*/
#define ERR_VPN_UPDATE_DEV_VPNCONFIG  27019  /*通知设备端VPN配置更新失败，需重启设备*/
#define ERR_VPN_UPDATE_SRV_VPNCONFIG  27020  /*更新服务器VPN配置失败*/
#define ERR_VPN_ADD_ROUTENODE         27021  /*添加路由节点失败*/

/************************************网络管理错误码***************************************/
#define ERR_GET_3GLINKINFO            28001  /*3G模块信息获取失败*/
#define ERR_DISCONNECT_3GLINK         28002  /*断开3G链路失败*/
#define ERR_TURNON_3GPOWER            28003  /*3G模块加电失败*/
#define ERR_CONNECT_3GLINK            28004  /*3G模块加电失败*/
#define ERR_SAVE_DEVCHANNEL_CONFIG    28005  /*保存设备通道配置失败*/

/*************************************同步模块错误码***************************************/
#define ERR_SYN_UPLOAD_LIMIT          29001  /*同步上传业务达到上限*/
#define ERR_SYN_UPLOAD_CREAT_FAIL     29002  /*创建同步上传业务失败*/
#define ERR_SYN_MALLOC_FAIL           29003  /*内存空间非配失败*/
#define ERR_SYN_REQ_TIMEOUT           29004  /*请求超时错误*/
#define ERR_SYN_ADD_TRANSACTION       29005  /*添加事务节点错误*/
#define ERR_SYN_FILERECORD_NOTEXIST   29006  /*文件记录不存在处理出错*/
#define ERR_SYN_MSG_ILLEGAL           29007  /*非法消息请求*/
#define ERR_SYN_PREBLOCK_RECV_FAIL    29008  /*上一块接收失败*/
#define ERR_SYN_FILE_UPLOAD_FAIL      29009  /*文件上传失败*/
#define ERR_SYN_SESSION_NOTEXIST      29010  /*业务不存在*/
#define ERR_SYN_UPLOAD_DUPLICATE      29011  /*同步上传业务重复申请*/
#define ERR_SYN_FILE_PROCSTATE_INIT   29012  /*文件处理状态初始化失败*/
#define ERR_SYN_PROC_STATE            29013  /*处理状态初始化失败*/
#define ERR_SYN_SRV_DISK_FULL         29014  /*服务器磁盘空间不足*/
#define ERR_SYN_DEV_SYN_NOW           29015  /*设备正在同步*/
#define ERR_SYN_BLOCKID_ILLEGAL       29016  /*要求上传的分块号不合法*/
#define ERR_SYN_TOTALBLOCK_ILLEGAL    29017  /*文件分块数超出最大范围*/
#define ERR_SYN_ANALYZE_FILENAME      29018  /*解析文件名出错*/
#define ERR_SYN_PREFILE_HANDLING         29019  /*文件正在处理*/

#define ERR_SYN_TYPE_RESERVED         29020  /*暂不支持该方式同步*/
#define ERR_SYN_LIMIT_24HOURS         29021  /*同步方式时间超过24小时范围*/
#define ERR_SYN_LIMIT_FILELIST        29022  /*指定文件列表同步超过上限(50)*/
#define ERR_SYN_FILELIST_EMPTY        29023  /*同步条件上传文件列表为空,同步直接结束*/

#define ERR_RSM_GET_FILE_PATH         29060  /*获取文件路径名失败*/
#define ERR_RSM_GET_FILE_INFO         29061  /*获取文件信息失败*/
#define ERR_RSM_SEND_UPLOADFILE_REQ   29062  /*发送上传文件请求失败*/
#define ERR_RSM_GET_NEEDSEND_BLOCK    29063  /*获取需要上传的分块出错*/
#define ERR_RSM_FILE_ALREADY_UPLOAD   29064  /*文件已上传完毕 无需上传*/
#define ERR_RSM_OPEN_RECORD_FILE      29065  /*文件打开失败*/
#define ERR_RSM_SEND_UPLOADBLOCK_REQ  29066  /*发送上传分块请求失败*/
#define ERR_RSM_GET_FILE_SIZE         29067  /*获取文件大小失败*/
#define ERR_RSM_GET_BLOCK_SIZE        29068  /*获取文件分块大小失败*/
#define ERR_RSM_GET_MSG_BUFF          29069  /*获取数据块缓冲区失败*/
#define ERR_RSM_SEND_BLOCK_DATA       29070  /*数据块分片发送失败*/
#define ERR_RSM_UNKNOWN               29071  /*未知错误*/
#define ERR_RSM_UPLOAD_BLOCK          29072  /*分块上传失败3次*/

/***********************************AVC模块错误码***************************************/

#define ERR_AVC_NO_SUCH_DEV           30001  /*无此子设备*/
#define ERR_AVC_INVALID_VIDEO_PARAM   30002  /*无效的视频参数*/
#define ERR_AVC_PARAM_UPDATE_FAILED   30003  /*参数数据库更新*/
#define ERR_AVC_VIDEO_SYS_BUSY        30004  /*视频采集系统忙*/

/*通信模块策略查询与设置错误码定义*/
#define ERRNO_STM_POLICY_NOT_EXIST               31001  /*未找到对应策略*/
#define ERRNO_STM_POLICY_UNKNOWN                 31002  /*未知错误*/
#define ERRNO_STM_POLICY_IMCOMPATIABLE_UNSUPPORT 31003  /*对端不支持该策略*/
#define ERRNO_STM_POLICY_UNSUPPORT               31004  /*不支持*/
#define ERRNO_STM_POLICY_PEER_UNSUPPORT          31005  /*对端不支持该策略*/
#define ERRNO_STM_POLICY_SET_TIMEOUT             31006  /*超时*/
#define ERRNO_STM_POLILY_PARAM_INVALID           31007  /*参数无效*/
#define ERRNO_STM_POLILY_SAVE_FAILED             31008  /*保存失败*/

/***********************************SED模块错误码***************************************/

#define ERR_SED_PARASE_FILE_NAME      32001  /*解析文件名失败*/
#define ERR_SED_GET_FILE_SIZE         32002  /*获取文件大小*/
#define ERR_SED_GET_GROUP_COUNT       32003  /*获取分组数*/
#define ERR_SED_ENCRYPT_SESSION_INIT  32004  /*加密会话初始化失败*/
#define ERR_SED_GENERATE_RANDOM       32005  /*生成随即序列失败*/
#define ERR_SED_SET_SESSIONKEY        32006  /*设置加密会话密钥失败*/
#define ERR_SED_OPEN_RECORDFILE       32007  /*打开文件失败*/
#define ERR_SED_CREATE_RECORDFILE     32008  /*创建文件失败*/
#define ERR_SED_READ_RECORDFILE       32009  /*读取文件失败*/
#define ERR_SED_DATA_ENCRYPT          32010  /*数据加密失败*/
#define ERR_SED_MALLOC                32011  /*内存空间分配失败*/
#define ERR_SED_KEYNODE_INIt          32012  /*节点初始化失败*/
#define ERR_SED_INSERT_NODE           32013  /*添加节点失败*/
#define ERR_SED_GET_ALIGNLENGTH       32014  /*获取对齐长度失败*/
#define ERR_SED_GET_PREALLOC          32015  /*预分配空间获取失败*/
#define ERR_SED_ENCDATA_LENGTH        32016  /*密文数据长度与预期长度不符*/
#define ERR_SED_PUCDATA_LENGTH        32017  /*明文数据长度与预期长度不符*/
#define ERR_SED_ADD_TRANSACTION       32018  /*添加事物节点失败*/
#define ERR_SED_TRANSACTION_TIMEOUT   32019  /*事物超时错误*/

#endif /*_YVCOMMON_DEF_H_*/

