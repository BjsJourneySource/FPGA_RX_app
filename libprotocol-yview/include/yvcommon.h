/***************************************************************************************
 * 项目名称: YVGA1207001A                                                               *
 *                                                                                      *
 * 模块名 ： common header		                                                       *
 * 文件名   ： yvcommon.h                                                               *
 * 机能说明：全局变量统一定义，ASSERT机制等通用功能                                     *
 *--------------------------------------------------------------------------------------*
 * Designed   by Zheng Yuhong        '2012-8-19                                         *
 * Coded      by Zheng Yuhong        '2012-8-19   Rev.0.0                               *
 * Revised    by Zheng Yuhong		'2012-8-30	 Rev.0.1							   *
 * 				增加用于网络通信的消息内部业务头部格式定义							   *
 ****************************************************************************************
 *- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *
 *               (C)Copyright 2012 YView Corporation All Rights Reserved.               *
 *- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *
 ***************************************************************************************/
#ifndef	_YVCOMMON_H_
#define	_YVCOMMON_H_

#include "yvcommon_def.h"

# define __ASSERT_VOID_CAST (void)

#define BEGIN_MODULE_GLOBAL_DEFINITION 	\
	struct yv_module_ctrl { 			\
		char 				*name;		\
		MCS_MODULE_ID_T		id;			\
		int					log_level;	

#define END_MODULE_GLOBAL_DEFINITION \
	};

// struct yv_module_ctrl g_yv_module_ctrl;

#define YV_GLOBAL(name) \
	g_yv_module_ctrl.name

#define YV_GLOBAL_DEFINE(module_name, module_id) 	 \
	struct yv_module_ctrl g_yv_module_ctrl =		 \
{												 \
	.name = module_name,						 \
	.id = module_id,							 \
};												 \
char * yv_GetModuleName() {							 \
	return g_yv_module_ctrl.name;					 \
}												 \

#define YV_THIS_MODULE_NAME \
	yv_GetModuleName()

#define YV_THIS_MODULE_ID \
	yv_GetModuleID()


/* 需要网络传输的消息头部格式定义，在系统内部传送时，该头部位于MCS头部之后，在网络上传*
 * 送时，该头部位于通信头部之后														  */
typedef struct ST_InterCommHdrV2
{
	//	unsigned char Version:4; /*版本，目前为 0，下一版本开始赋值*/
	//	unsigned char HdrLen:4;        /*头部长度，表示总共有几个int，目前为0，下一版本开始赋值*/
	//	char reserved[7];    /*保留位，目前未0*/
	unsigned char Reserved[8];    //预留
	unsigned int 	SServerID;	/*源服务器编号*/
	unsigned int 	SObjID;		/*源对象编号*/
	unsigned int 	DServerID;	/*目标服务器编号*/
	unsigned int 	DObjID;		/*目标对象编号*/
	unsigned int 	MsgType;	/*消息类型*/
	unsigned int 	MsgID;		/*消息唯一标识*/
	unsigned int 	SessionID;	/*会话标识*/
	unsigned int 	DataLength;	/*消息中载荷数据长度，即Content部分的实际长度*/
	unsigned char	Content[0];	/*载荷数据*/
} yv_InterCommHdrV2;

/*ADD_S by liangy @2017-04-26*/
/*
 * 由于增加业务系统审计功能,需要记录登录IP信息,
 * 而原有消息结构体中没有该信息,IP和端口信息都隐藏在通信模块内部;
 * 修改通信业务头,复用reserved保留位,增加IP和端口号,继续保持结构体大小不变,
 * 此版本的业务头只在几个登录认证消息中有效;
 * */
typedef struct ST_InterCommHdr
{
	unsigned char Version:4; /*版本，目前为 0，下一版本开始赋值*/
	unsigned char HdrLen:4;  /*头部长度，表示总共有几个int，目前为0，下一版本开始赋值*/
	char reserved[1];    /*保留位，目前未0*/
	unsigned short nPort;
	unsigned int IP;

	unsigned int    SServerID;  /*源服务器编号*/
	unsigned int    SObjID;     /*源对象编号*/
	unsigned int    DServerID;  /*目标服务器编号*/
	unsigned int    DObjID;     /*目标对象编号*/
	unsigned int    MsgType;    /*消息类型*/
	unsigned int    MsgID;      /*消息唯一标识*/
	unsigned int    SessionID;  /*会话标识*/
	unsigned int    DataLength; /*消息中载荷数据长度，即Content部分的实际长度*/
	unsigned char   Content[0]; /*载荷数据*/
} yv_InterCommHdr;
/*ADD_E by liangy @2017-04-26*/

/* 数据头结构体,该头部只在请求消息和响应消息中出现 */
typedef struct SDataHdr
{
	unsigned int lTransactionID; /* 事物ID*/
	unsigned int lBusinessID;    /* 业务ID*/
	int lDataLength;             /* 数据长度*/
	char content[0];
} ST_DataHdr;

/* 响应头结构体,该头部只在响应消息中出现，用于表示响应结果 */
typedef struct SMsgRespRet
{
	int lRet;           /* 0:成功 -1:失败*/
	int lSts;           /* 错误码*/
	char content[0];
} ST_MsgRespRet;

/*服务器初始化结构体*/
typedef struct ServerInfo
{
	char cSip[20];             /*服务器IP*/
	unsigned int lPort;       /*服务器端口*/
	unsigned int lDeviceID;   /*设备ID*/
	int ipLength;              /*IP地址有效长度*/
}ST_SerInfo;

/*音频参数初始化结构体*/
typedef struct AudioInfo
{
	int lSamepleRate;    /*采样率*/
	int lChannelNum;     /*通道数*/
	int lQuantifcal;     /*量化位数*/
	int lBitRate;        /*码率*/
	int lEncodFormat;    /*编码格式*/
}ST_AuInfo;

/*视频参数初始化结构体*/
typedef struct VideoInfo
{
	int lWidth;          /*视频宽*/
	int lHeight;         /*视频高*/
	int lFrameRate;      /*帧率*/
	int lBitRate;        /*码率*/
	int lEncodFormat;    /*编码格式*/
	int gl_ResolutRatio; /*分辨率DPI*/
}ST_ViInfo;


#ifdef __cplusplus
extern "C" {
#endif
	//char * yv_GetModuleName();
	//MCS_MODULE_ID_T yv_GetModuleID();
#ifdef __cplusplus
}
#endif

#ifndef	DEBUG

# define yv_Assert(expr)		(__ASSERT_VOID_CAST (0))

#else /* DEBUG switch on  */

#ifdef __cplusplus
extern "C" {
#endif
	/* This prints an "Assertion failed" message and aborts.  */
	extern void __assert_fail (__const char *__assertion, __const char *__file,
			unsigned int __line, __const char *__function);
#ifdef __cplusplus
}
#endif

# define yv_Assert(expr)							\
	((expr)								\
	 ? __ASSERT_VOID_CAST (0)						\
	 : __assert_fail (__STRING(expr), __FILE__, __LINE__, __func__))

#endif /* DEBUG  */

#endif /* _YVCOMMON_H_ */
