/********************************************************************************************
 * 文 件 名 : yvcommon_dev_msg_type.h                                            
 * 项目名称 : YVGA1207001A                                                            
 * 模 块 名 :
 * 功    能 : 设备内部交互消息类型的定义                                                          
 * 操作系统 : LINUX                                                                  
 * 修改记录 : 初始版本                                                                
 * 版    本 : Rev 0.1.0                                                               
 *-------------------------------------------------------------------------------------------
 * 设    计 : chenlg      '2013-2-22                                                  
 * 编    码 : chenlg      '2013-2-22                                                  
 * 修    改 :                                                                          
 ********************************************************************************************
 *- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  
 * 公司介绍及版权说明                                                                   
 *                                                                                      
 *               (C)Copyright 2012 YView    Corporation All Rights Reserved.            
 *- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 ********************************************************************************************/

#ifndef _YVCOMMON_DEV_MSG_TYPE_H
#define _YVCOMMON_DEV_MSG_TYPE_H

/*用于网络状态中解析3G工作制式，工作模式的宏定义*/
typedef enum
{
	WORKSTANDARD_UNDEFINED = 0,
	WORKSTANDARD_EVDO,         /*EVDO工作制式---代表电信卡*/
	WORKSTANDARD_WCDMA,        /*WCDMA工作制式--代表联通卡*/
	WORKSTANDARD_TDD,          /*TDD-LTE工作制式*/
	WORKSTANDARD_FDD,           /*FDD-LTE工作制式*/
	WORKSTANDARD_TDSCDMA			/*TDSCDMA工作制式--代表移动卡*/
}EN_WORKSTANDARD_TYPE_T;

typedef enum
{
	TRANSMITQULITY_NONE = 0,    /*无传输*/
	TRANSMITQULITY_LOW,         /*低速传输*/
	TRANSMITQULITY_MIDDLE,      /*中速传输*/
	TRANSMITQULITY_HIGH         /*高速传输*/
}EN_TRANSMITQULITY_TYPE_T;

typedef enum
{
	LINKMODE_UNDEFINED = 0,
	LINKMODE_WIFI,
	LINKMODE_3G,
	LINKMODE_WIRED,
	LINKMODE_3G_AND_WIFI,
}EN_LINKMODE_TYPE_T;


typedef enum
{
	OPEN_NONE = 0,
	SIM1_OPENMARK = 0x1,
	SIM2_OPENMARK = 0x10,
	SIM3_OPENMARK = 0x100,
	SIM4_OPENMARK = 0x1000,
	SIM5_OPENMARK = 0x10000,
	SIM6_OPENMARK = 0x100000,
	SIM7_OPENMARK = 0x1000000,
	SIM8_OPENMARK = 0x10000000,
}EN_SIMCARD_OPENMARK_TYPE_T;




/*SIM卡工作模式定义*/
#define WORKMODE_2G 2    /*2G工作模式*/
#define WORKMODE_3G 3    /*3G工作模式*/
#define WORKMODE_4G 4    /*4G工作模式*/

/*SIM卡状态定义*/
#define SIMSTATE_VALID    1  /*SIM存在且有效*/
#define SIMSTATE_NOTEXSIT 0  /*SIM不存在*/
#define SIMSTATE_ERR      -1 /*SIM存在单异常*/

/*流量控制类别*/
#define DEV_TRAFFIC_CTLTYPE_ALLOWUP         1
#define DEV_TRAFFIC_CTLTYPE_FORCEDOWN   2

#define ERR_DEV_STREAM_CHANGE_FAILED         26101
#define ERR_DEV_STREAM_CHANGE_TIMEOUT    26102
#define ERR_DEV_STREAM_CHANGE_UNWANT    26103




/*ful add*/
#define STREAM_ID_FOR_PTT 3333 /*语音对讲专用流通道，，，，数值未定，，，*/
/*end add*/

/*liul add*/
#define OUT_VISIBLE_FLAG        0x00000001
#define DEFAULT_STREAM_FLAG     0x00000002
#define EXP_AUDIO_STREAM        0x00000004
#define STREAM_SRC_SUBDEV       0x00000008 /*子设备流*/
/*end add*/
/*chenlg add 私有的流ID相关联性定义*/
#define VALUE_AUDIO_AND_RECORD    10     /*音频和相应视频的录像ID之间的ID差值*/
#define VALUE_UPLOAD_MORETHAN_RECORD   100       /*上传流和它对应的本地存储流ID之间的差值（可根据实际情况调整）*/


/*采集系统类别*/
#define CAP_SYS_TYPE_VIDEO_AUDIO    1
#define CAP_SYS_TYPE_VIDEO_ONLY     2
#define CAP_SYS_TYPE_AUDIO_ONLY     3

/*控制命令类别*/
#define CMD_CTRL_CAP_SYS_START      1
#define CMD_CTRL_CAP_SYS_STOP       2
#define CMD_CTRL_CAP_SYS_RESTART    3

typedef enum
{
	/*设备内部所有模块通用消息类型定义*/
	/***************************************************************************************/
	/*通用消息类型定义*/
	MSG_COM_NFY_STREAM_INFO_UPDATE       = 0x2701001, /* 流信息更新通知, 采集压缩模块/扩展设备管理模块->流媒体模块*/
	MSG_COM_NFY_ORDER_STREAM_INFO_UPDATE = 0x2701002, /* 流信息上传通知, 流媒体模块->采集压缩模块/扩展设备管理管理模块*/
	/*设备管理模块<-------->其他模块*/
	MSG_COM_NFY_SOS_STATUS               = 0x2701003, /* SOS报警状态通知 设备管理模块->其他模块*/
	/*其他模块<-------->设备管理模块*/
	MSG_COM_REQ_LOGIN_INFO               = 0x0701004, /* 设备登陆信息查询请求,其他模块->设备管理模块*/
	MSG_COM_RES_LOGIN_INFO               = 0x1701004, /* 返回登陆信息查询结果,设备管理模块->其他模块*/

	MSG_COM_REQ_DEV_QUERY_SUB_INFO       = 0x0701005, /* 查询子设备基本信息请求*/
	MSG_COM_RES_DEV_QUERY_SUB_INFO       = 0x1701005, /* 查询子设备基本信息响应*/

	MSG_COM_REQ_DEV_QUERY_SUB_DETAILS    = 0x0701006, /* 查询子设备详细信息请求*/
	MSG_COM_RES_DEV_QUERY_SUB_DETAILS    = 0x1701006, /* 查询子设备详细信息响应*/

	MSG_COM_REQ_WIRED_NET_CFG            = 0x0701007, /*有线网络配置请求*/                                     
	MSG_COM_RES_WIRED_NET_CFG            = 0x1701007, /*有线网络配置响应*/                                  
	MSG_COM_REQ_WIFI_NET_CFG             = 0x0701008, /*WIFI网络配置请求*/                              
	MSG_COM_RES_WIFI_NET_CFG             = 0x1701008, /*WIFI网络配置响应*/                              
	MSG_COM_REQ_GPS_SET                  = 0x0701009, /*GPS配置请求*/                       
	MSG_COM_RES_GPS_SET                  = 0x1701009, /*GPS配置响应*/                                                       
	MSG_COM_REQ_STREAM_CFG_SET           = 0x070100a, /*流参数配置请求*/                               
	MSG_COM_RES_STREAM_CFG_SET           = 0x170100a, /*流参数配置响应*/                               
	MSG_COM_REQ_CAP_SYS_CTRL             = 0x070100b, /*采集系统控制请求*/                               
	MSG_COM_RES_CAP_SYS_CTRL             = 0x170100b, /*采集系统控制响应*/                               
	MSG_COM_REQ_STREAM_UPLOAD_SET        = 0x070100c, /*流上传设置请求*/                                   
	MSG_COM_RES_STREAM_UPLOAD_SET        = 0x170100c, /*流上传设置响应*/                                   
	MSG_COM_REQ_STREAM_RECORD_SET        = 0x070100d, /*流录像设置请求*/                                   
	MSG_COM_RES_STREAM_RECORD_SET        = 0x170100d, /*流录像设置响应*/                                                                             
	MSG_COM_NFY_NET_SWITCH               = 0x270100e, /*网络模式切换通知*/  
	MSG_COM_REQ_AUDIO_INPUT_SRC          = 0x070100f, /*音频输入源切换请求*/  
	MSG_COM_RES_AUDIO_INPUT_SRC          = 0x170100f, /*音频输入源切换响应*/  
	MSG_COM_REQ_VIDEO_INPUT_SRC          = 0x0701010, /*视频输入源切换请求*/  
	MSG_COM_RES_VIDEO_INPUT_SRC          = 0x1701010, /*视频输入源切换响应*/  
	MSG_COM_NFY_DEV_SERVER_SWITCH        = 0x2701011, /*服务器切换通知*/
	MSG_COM_NFY_NET_INIT_LINKMODE        = 0x1701012, /*网络模式初始连接模式通知*/
	MSG_COM_REQ_ENCYP_INFO               = 0x0701013, /*查询加密信息请求*/
	MSG_COM_RES_ENCYP_INFO               = 0x1701013, /*响应加密信息响应*/
	MSG_COM_REQ_DEV_RECORD_KEY_UPDATE    = 0x0701014, /*录像秘钥更新请求*/
	MSG_COM_RES_DEV_RECORD_KEY_UPDATE    = 0x1701014, /*录像秘钥更新响应*/

	/*其他模块<-------->状态监控模块*/
	MSG_COM_REQ_DEVSTATUS_INFO           = 0x0701050, /*主动查询设备状态信息*/
	MSG_COM_RES_DEVSTATUS_INFO           = 0x1701050, /*查询设备状态信息响应*/
	MSG_COM_REQ_DEVTEMP_LEVEL_INFO       = 0x0701051, /*查询设备温度级别*/
	MSG_COM_RES_DEVTEMP_LEVEL_INFO       = 0x1701051, /*响应设备温度级别*/
	/***************************************************************************************/
	/*扩展设备管理模块消息类型定义*/

	/*扩展设备管理模块<-------->信息采集模块*/
	MSG_EXP_NFY_POSITIONDATA             = 0x2711011, /* 定位信息通知, 扩展设备管理模块->信息采集模块*/
	MSG_EXP_NFY_POSITIONDATA_EX          = 0x2711012,  /*定位信息通知（扩展）,扩展设备管理模块->信息采集模块*/
	/*信息采集模块<-------->扩展设备管理模块*/
	STS_NFY_SWITCH_STREAM                = 0x2711001, /* 线控耳机触发切换通道通知，信息采集模块->扩展设备管理模块*/
	/*信息采集模块<-------->扩展设备管理模块*/
	STS_NFY_SWITCH_VOLUME                = 0x2711002, /* 线控耳机触发切换音量通知，信息采集模块->解码模块*/

	/***************************************************************************************/
	/*设备端流媒体模块消息类型定义*/

	/*流媒体模块<-------->采集压缩模块/扩展设备管理模块*/
	MSG_SMP_REQ_CHANGE_VIDEO_PARAM       = 0x0721021, /* 更改视频采集参数请求, 流媒体模块->采集压缩模块/扩展设备管理模块*/
	MSG_SMP_RES_CHANGE_VIDEO_PARAM       = 0x1721021, /* 更改视频采集参数响应, 采集压缩模块/扩展设备管理模块->流媒体模块*/
	/*流数据*/
	MSG_NFY_AVCAP_AV                    = 0x2721022, /* 流媒体数据通知消息,音视频捕捉/扩展外设管理->流媒体模块  */
	MSG_SMP_NFY_UPLOAD_STREAM_STATUS    = 0x2721023, /* 流上传状态通知消息,流媒体模块->状态指示模块  */                                      
	MSG_SMP_NFY_RECORD_STREAM_STATUS    = 0x2721024, /* 流录像状态通知消息,流媒体模块->状态指示模块  */  
	MSG_EXP_NFY_PLAY_STREAM       = 0x2721001, /* 扩展外设通知流媒体接收扩展流信息，扩展设管理->流媒体模块  */
	/*ADD_S by louzf*/
	MSG_NFY_VOICE_PROMPT_NOTICE  = 0x2721002,   /*提示音数据通知, 状态指示模块->设备解码模块*/
	/*ADD_E by louzf*/

	/*状态通知之流状态通知----->设备管理模块*/
	MSG_SMP_NFY_RECORD_BASIC_STATUS		 = 0x2721010,  /*设备录像基本状态通知*/
	MSG_STS_NFY_BASIC_STATUS				 = 0x2721020,  /*设备基本状态通知消息*/
	MSG_STS_NFY_ALARM                   = 0x2721031,  /*设备内部报警通知消息*/

	/*RTC模块时钟唤醒广播*/
	MSG_RTC_CLOCK_TIMEUP_BRC            = 0x2721041,  /*设备RTC模块时钟唤醒广播消息*/
	/***************************************************************************************/
	/*设备端的设备管理模块消息类型定义*/
	/*ADD_S by liangy*/
	/*A0二期新增消息定义*/
	MSG_DEV_REQ_RSM_FILELIST            = 0x0730001, /*查询设备文件列表请求,设备管理->录像管理*/
	MSG_DEV_RES_RSM_FILELIST            = 0x1730001, /*查询设备文件列表响应,录像管理->设备管理*/

	MSG_DEV_REQ_RSM_SYNC_FILE           = 0x0730002, /*文件同步上传请求,设备管理->录像管理*/
	MSG_DEV_RES_RSM_SYNC_FILE           = 0x1730002, /*文件同步上传响应,录像管理->设备管理*/
	MSG_RSM_NFY_SYNC_FILE_STATUS        = 0x2730002, /*文件同步上传状态进度通知,设备管理->来源模块*/
	/*ADD_E by liangy*/

	/*ADD_S by ful*/
	/*设备管理模块<-------->短信控制模块*/
	MSG_REQ_DEV_START                    = 0x0730003, /*远程开机请求,设备进入正常模式 短信控制->设备管理模块*/
	MSG_RES_DEV_START                    = 0x1730003, /*远程开机响应, 设备管理模块->短信控制*/
	MSG_REQ_DEV_STOP                     = 0x0730004, /*远程关机请求,设备进入低功耗模式 短信控制->设备管理模块*/
	MSG_RES_DEV_STOP                     = 0x1730004, /*远程关机响应, 设备管理模块->短信控制*/
	MSG_REQ_SET_DEV_NET_INFO             = 0x0730005, /*设置网络参数请求, 短信控制->设备管理模块*/
	MSG_RES_SET_DEV_NET_INFO             = 0x1730005, /*设置网络参数响应, 设备管理模块->短信控制*/
	MSG_REQ_DEV_REBOOT                   = 0x0730006, /*设备重启, 短信控制->设备管理模块*/
	MSG_RES_DEV_REBOOT                   = 0x1730006, /*设备重启响应, 设备管理模块->短信控制*/
	/*ADD_E by ful*/

	/*ADD_S by chenlg*/
	MSG_REQ_DEV_VIDEOINPUT_SWITCH        = 0x0730010, /*设备视频源切换请求, 设备管理模块->安卓界面模块*/
	MSG_RES_DEV_VIDEOINPUT_SWITCH        = 0x1730010, /*设备视频源切换响应, 安卓界面模块->设备管理模块*/
	/*ADD_E by chenlg*/

	/*ADD_S by louzf */
	MSG_DEV_REQ_INTERNAL_PARAM_SET		  = 0x0730020, /*设备内部配置控制请求， 内部模块->设备管理模块*/
	MSG_DEV_RES_INTERNAL_PARAM_SET		  = 0x1730020,	/*设备内部配置控制回应,   内部模块->设备管理模块*/

	MSG_DEV_REQ_INTERNAL_PARAM_QUERY	  = 0x0730021, /*设备内部配置查询请求,   内部模块->设备管理模块*/
	MSG_DEV_RES_INTERNAL_PARAM_QUERY	  = 0x1730021, /*设备内部配置查询请求,   内部模块->设备管理模块*/
	/*ADD_E by louzf*/
	/*设备管理模块<-------->所有模块*/
	MSG_DEV_NFY_LOGIN_BRC                = 0x2731031, /* 设备登录广播,设备管理模块->所有模块*/
	MSG_DEV_NFY_LOGOUT_BRC               = 0x2731032, /* 设备注销广播,设备管理模块->所有模块*/
	MSG_DEV_NFY_SYSCONFIG_CHANGE_BRC     = 0x2731033, /* 系统配置更改广播,设备管理模块->所有模块*/
	/*设备管理模块<-------->通信模块*/
	MSG_DEV_NFY_SERVER_IPPORT_INFO       = 0x2731034, /* 服务器信息更新通知,设备管理模块->通信模块*/
	/*设备管理模块<-------->流媒体模块*/
	MSG_DEV_NFY_PTT_CREAT                = 0x2731035, /* PTT建立通知,设备管理模块->流媒体模块*/
	MSG_DEV_NFY_PTT_DESTRUCT             = 0x2731036, /* PTT销毁通知,设备管理模块->流媒体模块*/
	MSG_DEV_NFY_PTT_LOCK                 = 0x2731037, /* PTT锁定通知,设备管理模块->流媒体模块*/
	MSG_DEV_NFY_PTT_UNLOCK               = 0x2731038, /* PTT解锁通知,设备管理模块->流媒体模块*/

	MSG_REQ_DEV_RECORD_CTRL_SET				  = 0x2731039, /*	录像配置请求,设备管理模块->流媒体模块*/
	MSG_RES_DEV_RECORD_CTRL_SET				  = 0x2731040, /* 录像配置回应,流媒体模块->设备管理模块*/

	/*设备管理模块<-------->音视频解码模块*/
	MSG_DEV_NFY_START_PLAY_AUDIO         = 0x2731039, /* 播放音频通知,设备管理模块->音视频解码模块 */
	MSG_DEV_NFY_STOP_PLAY_AUDIO          = 0x2731040, /* 停止播放音频通知,设备管理->音视频解码模块*/
	/*设备管理模块<-------->状态指示模块*/
	MSG_DEV_NFY_PTT_STATUS               = 0x2731041, /* PTT状态指示通知,设备管理模块<-------->状态指示模块*/

	MSG_DEV_NFY_DH_KEY_BRC               = 0x2731042, /* 密钥更新广播, 设备管理模块->所有模块*/

	/*设备管理模块<-------->音视频解码模块*/
	MSG_DEV_CTRL_AUDIO_CAPTURE_START         = 0x2731043, /* 开启音频采集控制,设备管理模块->音视频解码模块 */
	MSG_DEV_CTRL_AUDIO_CAPTURE_STOP          = 0x2731044, /* 关闭音频采集控制,设备管理->音视频解码模块*/

	/*设备管理模块<-------->sdpsmp模块*/
	MSG_DEV_REQ_DEVENCRY_ABILITY_CFG    = 0x2731045,    /*查询设备加密能力请求  设备管理模块->流媒体模块*/
	MSG_DEV_RES_DEVENCRY_ABILITY_CFG    = 0x2731046,    /*设备加密能力查询响应 流媒体模块->设备管理模块*/

	/***************************************************************************************/
	/*设备端网络管理模块消息类型定义*/

	/*网络管理模块<-------->状态指示模块*/
	MSG_NMG_NFY_LINK_STATUS              = 0x2741051, /* 网络连接状态通知,网络管理模块------->状态指示模块、信息采集模块*/
	/*网络管理模块<-------->通信模块*/
	MSG_NMG_NFY_UPLINK_INFO              = 0x2741052, /* 网络通道信息通知,网络管理模块<-------->通信模块*/
	MSG_NMG_NFY_LINK_STATUS_EX           = 0x2741053, /* 网络连接状态通知(扩展),网络管理模块------->状态指示模块、信息采集模块*/                              
	MSG_NMG_NFY_UPLINK_INFO_EX           = 0x2741054, /* 网络通道信息通知(扩展),网络管理模块<-------->通信模块*/  
	/*所有模块<-------->通信模块*/
	MSG_DEV_NFY_DEL_SESSION       = 0x2A38047, /* 删除临时会话通知,所有模块->通信模块       */

	/***************************************************************************************/
	/*设备端下载回放模块消息类型定义*/
	/*TODO*/

	/***************************************************************************************/
	/*设备端状态指示模块消息类型定义*/

	/*通信模块<-------->状态指示模块*/
	MSG_STM_NFY_TRANSMIT_STATUS          = 0x2761071, /*传输状态指示通知,通信模块<-------->状态指示模块*/
	/*其他模块<-------->通信模块*/
	MSG_STM_REQ_POLICY_QUERY             = 0x0761072, /*传输策略查询请求,其他模块<-------->通信模块*/
	MSG_STM_RES_POLICY_QUERY             = 0x1761072, /*传输策略查询响应,通信模块<-------->其他模块*/
	MSG_STM_REQ_POLICY_SWITCH            = 0x0761073, /*传输策略切换请求,其他模块<-------->通信模块*/
	MSG_STM_RES_POLICY_SWITCH            = 0x1761073, /*传输策略切换响应,通信模块<-------->其他模块*/



	/***************************************************************************************/
	/*设备端信息采集监控模块消息类型定义*/

	/*信息采集监控模块<-------->设备管理模块*/
	MSG_STS_NFY_SOS_ALARM                = 0x2771081, /* SOS触发通知,信息采集监控模块->设备管理模块*/
	MSG_STS_NFY_SOSSTOP_ALARM            = 0x2771082, /* SOS停止通知,信息采集监控模块->设备管理模块*/
	MSG_STS_NFY_LOCK_PTT                 = 0x2771083, /* PTT触发通知,信息采集监控模块->设备管理模块*/
	MSG_STS_NFY_UNLOCK_PTT               = 0x2771084, /* PTT停止通知,信息采集监控模块->设备管理模块*/
	/*信息采集监控模块<-------->网络管理模块*/
	MSG_STS_NFY_MODE_WIFI                = 0x2771085, /*WIFI模式通知,信息采集模块<-------->网络连接管理模块*/
	MSG_STS_NFY_MODE_3G                  = 0x2771086, /*3G模式通知,信息采集模块<-------->网络连接管理模块*/
	/*信息采集监控模块<-------->状态指示模块*/
	MSG_STS_NFY_BATTERAY_STATUS          = 0x2771087, /*电池电量状态指示通知,信息采集监控模块<-------->状态指示模块*/
	MSG_STS_NFY_TEMPERATURE_STATUS       = 0x2771088, /*设备温度状态指示通知,信息采集监控模块<-------->状态指示模块*/

	/*通信模块<---------->流媒体模块*/
	MSG_STM_REQ_TRAFFIC_CONTROL          = 0x0771089,  /*流量控制请求 通信模块---------->流媒体模块*/
	MSG_STM_RES_TRAFFIC_CONTROL          = 0x1771089,  /*流量控制响应 流媒体模块---------->通信模块*/
	/*更改通信模块的流量控制调整参数，用于测试*/
	MSG_STM_NFY_TC_PARAM                 = 0x2771090,       /*其他模块->通信模块，通知流量控制参数变更*/

	/*ADD_S_by_Wangly VPN二期新增消息类型定义*/
	MSG_STM_NFY_LINKTRANSIMIT_STATUS     = 0x2771091,   /*链路传输状态通知消息 通信模块---->信息采集模块*/
	MSG_VPN_NFY_NIC_TRANSMIT_STATUS      = 0x2771092,   /*网卡数据传输统计消息 网络管理->信息采集模块*/
	MSG_VPN_NFY_DEV_TRANSMIT_STATUS      = 0x2771093,   /*设备数据传输统计消息 网络管理->信息采集模块*/
	/*ADD_S_by_Wangly VPN二期新增消息类型定义*/
	/***************************************************************************************/
	/*VPN模块消息类型定义*/
	MSG_DEV_VPN_REQ_ONLINE_STATE  = 0x0780002,  /*查询设备在线状态请求, VPN模块->设备管理*/
	MSG_DEV_VPN_RES_ONLINE_STATE  = 0x1780002,  /*查询设备在线状态响应, 设备管理->VPN模块*/
	MSG_DEV_VPN_REQ_AES_KEY       = 0x0780003,  /*查询密钥请求, VPN模块->设备管理*/
	MSG_DEV_VPN_RES_AES_KEY       = 0x1780003,  /*查询密钥响应, 设备管理->VPN模块*/

	/*ADD_S_by_chenlg 手电筒上视频接口库与MediaServer新增消息类型定义*/
	MSG_DEV_VDC_REQ_SYS_REG             = 0x0790002,  /*视频采集系统注册请求,VDC库->MediaServer*/
	MSG_DEV_VDC_RES_SYS_REG             = 0x1790002,  /*视频采集系统注册响应,MediaServer->VDC库*/
	MSG_DEV_VDC_REQ_SYS_UNREG           = 0x0790003,  /*视频采集系统注销请求,VDC库->MediaServer*/
	MSG_DEV_VDC_RES_SYS_UNREG           = 0x1790003,  /*视频采集系统注销响应,MediaServer->VDC库*/
	MSG_DEV_VDC_REQ_CHN_CAP_START       = 0x0790004,  /*视频通道启动采集请求,VDC库->MediaServer*/
	MSG_DEV_VDC_RES_CHN_CAP_START       = 0x1790004,  /*视频通道启动采集响应,MediaServer->VDC库*/
	MSG_DEV_VDC_REQ_CHN_CAP_STOP        = 0x0790005,  /*视频通道停止采集请求,VDC库->MediaServer*/
	MSG_DEV_VDC_RES_CHN_CAP_STOP        = 0x1790005,  /*视频通道停止采集响应,MediaServer->VDC库*/
	MSG_DEV_VDC_REQ_CHN_CAPPARAM_SET    = 0x0790006,  /*视频通道设置采集参数请求,VDC库->MediaServer*/
	MSG_DEV_VDC_RES_CHN_CAPPARAM_SET    = 0x1790006,  /*视频通道设置采集参数响应,MediaServer->VDC库*/
	MSG_DEV_VDC_REQ_KEEPALIVE           = 0x0790007,  /*视频系统保活请求,VDC库->MediaServer*/
	MSG_DEV_VDC_RES_KEEPALIVE           = 0x1790007,  /*视频系统保活响应,MediaServer->VDC库*/
	MSG_DEV_VDC_NFY_CHN_FRAMEADDR       = 0x0790008,  /*视频通道帧物理地址通知,MediaServer->VDC库*/
	MSG_DEV_VDC_NFY_CHN_RELEASEFRAME    = 0x0790009,  /*视频通道帧资源释放通知,VDC库->MediaServer*/
	/*ADD_E_by_chenlg 手电筒上视频接口库与MediaServer新增消息类型定义*/
	/***************************************************************************************/
	/*音视频采集模块消息类型定义*/
	/*TODO*/

	/***************************************************************************************/
	/*音视频解码模块消息类型定义*/
	/*TODO*/

	/***************************************************************************************/
	/***************************************************************************************/
	/*透传模块消息类型定义*/
	/*TODO*/
	MSG_COM_REQ_SERIALNET_MODE_CFG      = 0x0782002, /*路由模式控制请求*/
	MSG_COM_RES_SERIALNET_MODE_CFG      = 0x1782002, /*路由模式控制响应*/

	MSG_COM_REQ_SERIALNET_MODE_SIMCFG  = 0x0782003, /*透传模式查询SIM卡信息请求*/
	MSG_COM_RES_SERIALNET_MODE_SIMCFG  = 0x1782003, /*透传模式查询SIM信息响应*/

	MSG_COM_NFY_SERIALNET_SIMCFG  = 0x2782002, /*透传模式SIM信息通知*/

	/***************************************************************************************/
	/*工作模式通知消息类型定义*/
	/*设备管理模块<-------->内部模块*/
	MSG_DEV_NFY_WORKMODE_WAKEUP         = 0x2783002, /* 唤醒通知 */
	MSG_DEV_NFY_WORKMODE_SLEEP          = 0x2783003, /* 休眠通知*/
	MSG_DEV_NFY_WORKMODE_UNLINK          = 0x2783004, /* 本地模式通知*/

	/*短信模块--->内部模块*/
	MSG_DEV_NFY_GPSMODE_CHANGE         = 0x2783010, /* GPS模式配置变更通知 */


	MCS_DEV_MAX_NUM
}EN_DEV_MCS_MSGTYPE_T;

#endif
