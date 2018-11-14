/***************************************************************************************
 * �� �� �� : yvcommon_data.h
 * ��Ŀ���� : YVGA1207001A                                                            
 * ģ �� �� : ����ͷ�ļ�
 * ��    �� : ���������豸��������Ϣ�ṹ��Ķ���                                                          
 * ����ϵͳ : LINUX                                                                  
 * �޸ļ�¼ : ��ʼ�汾                                                                
 * ��    �� : Rev 0.1.0                                                               
 *--------------------------------------------------------------------------------------
 * ��    �� : liangy      '2012-07-03
 * ��    �� : liangy      '2012-07-03
 * ��    �� :                                                                          
 ***************************************************************************************
 *- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 * ��˾���ܼ���Ȩ˵��                                                                   
 *                                                                                      
 *               (C)Copyright 2012 YView    Corporation All Rights Reserved.            
 *- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 ***************************************************************************************/

#ifndef _YVCOMMON_SRV_DEV_MSG_STRUCT_H_
#define _YVCOMMON_SRV_DEV_MSG_STRUCT_H_

#include "yvcommon.h"
#include "yvcommon_data_struct.h"
#define MAX_VIDEO_COLOR_SCENE_NUM 3
/* �豸��֤���� DEV_REQ_SRV_DEVICE_AUTHEN ���豸->��ȫ��֤ */
/* OLD MSG=DEV_REQ_DEVICE_AUTHEN STRUCT=ST_DeviceAuthenReq */
typedef struct SDevReqSrvDeviceAuthen
{
	yv_InterCommHdr stInterHdr;
	ST_DataHdr stDataHdr;
	ST_Device stDevice;
} ST_DevReqSrvDeviceAuthen;


/* �豸��֤��� DEV_RES_SRV_DEVICE_AUTHEN ����ȫ��֤->�豸 */
/* OLD MSG=DEV_RES_DEVICE_AUTHEN STRUCT=ST_DeviceAuthenRes */
typedef struct SDevResSrvDeviceAuthen
{
	yv_InterCommHdr stInterHdr;
	ST_DataHdr stDataHdr;
	ST_MsgRespRet stMsgRet;
	ST_Device stDevice;
	unsigned char byBigNumBuf[0];
} ST_DevResSrvDeviceAuthen;


/* �豸ע�� DEV_NFY_SRV_DEVICE_LOGOUT ���豸->�豸���� */
/* OLD MSG=DEV_NFY_DEVICE_LOGOUT STRUCT=ST_DeviceLogoutInterCommNfy */
typedef struct SDevNfySrvDeviceLogout
{
	yv_InterCommHdr stInterHdr;
	ST_Device stDevice;
} ST_DevNfySrvDeviceLogout;


/*����������Ϣ DEV_REQ_SRV_KEEP_ALIVE,�û����豸������������->�û����豸��������*/
/* OLD MSG=COM_REQ_KEEP_ALIVE STRUCT=ST_KeepAliveReq */
typedef struct SDevReqSrvKeepAlive
{
	yv_InterCommHdr stInterHdr;
	ST_DataHdr stDataHdr;
} ST_DevReqSrvKeepAlive;


/*������Ӧ��Ϣ DEV_RES_SRV_KEEP_ALIVE,�û����豸��������->�û����豸������������*/
/* OLD MSG=COM_RES_KEEP_ALIVE STRUCT=ST_KeepAliveRes */
typedef struct SDevResSrvKeepAlive
{
	yv_InterCommHdr stInterHdr;
	ST_DataHdr stDataHdr;
	ST_MsgRespRet stMsgRet;
} ST_DevResSrvKeepAlive;

/* SRV_REQ_DEV_STREAM_INFO ��ѯ�豸������Ϣ������Ϣ*/
/* OLD MSG=CLT_REQ_DEV_STREAM_INFO STRUCT=ST_CltDevStreamInfoReq */
typedef struct SSrvReqDevStreamInfo
{
	yv_InterCommHdr stInterHdr;      /* ҵ��ͷ   */
	ST_DataHdr stDataHdr;            /* ����ͷ   */
}ST_SrvReqDevStreamInfo;
/* SRV_RES_DEV_STREAM_INFO ��ѯ�豸������Ϣ��Ӧ��Ϣ*/
/* OLD MSG=CLT_RES_DEV_STREAM_INFO STRUCT=ST_CltDevStreamInfoRes */
typedef struct SSrvResDevStreamInfo
{
	yv_InterCommHdr stInterHdr;      /* ҵ��ͷ   */
	ST_DataHdr stDataHdr;            /* ����ͷ   */
	ST_MsgRespRet stMsgRes;          /* ��Ӧ��� */
	ST_DevStreamInfo stDevStreamInfo;/*�豸����Ϣ*/
}ST_SrvResDevStreamInfo;

/* Ҫ���豸��������֪ͨ SRV_NFY_DEV_START_MEDIA_STREAM */
/* OLD MSG=SMP_NFY_ORDER_DEV_TRANSMIT STRUCT=ST_MsgSMPDevCtrl(ֹͬͣ) */
typedef struct SSrvNfyDevStartMediaStream
{
	yv_InterCommHdr stInterHdr;
	ST_SMPDevCtrl stSMPDevCtrl;
} ST_SrvNfyDevStartMediaStream;


/* Ҫ���豸ֹͣ��������֪ͨ SRV_NFY_DEV_STOP_MEDIA_STREAM */
/* OLD MSG=SMP_NFY_ORDER_DEV_STOPTRANSMIT STRUCT=ST_MsgSMPDevCtrl */
/* ��ý��Ҫ���豸���ͻ�ֹͣ��ָ�����Ϣ */
typedef struct SSrvNfyDevStopMediaStream
{
	yv_InterCommHdr stInterHdr;
	ST_SMPDevCtrl stSMPDevCtrl;
} ST_SrvNfyDevStopMediaStream;

/*��ý�����ݱ��Ľṹ�� DEV_SRM_SRV_MEDIA_STREAM */
/* OLD MSG=MSG_DEV_DAT_STM STRUCT=ST_MsgStreamHdr */
typedef struct SDevSrmSrvMediaStream
{
	yv_InterCommHdr stInterCommHdr; /*ҵ��ͷ��*/
	ST_StreamHdr stStreamHdr;       /*��ý��ͷ��*/
	char Content[0];
}ST_DevSrmSrvMediaStream;

/*DEV_NFY_SRV_STATUS_INFO �豸��Ϣ�ϱ�֪ͨ��Ϣ*/
/* OLD MSG=DEV_NFY_DEV_STATUS_INFO STRUCT=ST_DevStatusInfoUploadNfy */
typedef struct SDevNfySrvStatusInfo
{
	yv_InterCommHdr stInterHdr;      /* ҵ��ͷ   */
	ST_DevStatusInfo stDevStatusInfo;/*�豸״̬��Ϣ*/
} ST_DevNfySrvStatusInfo;

/*SRV_REQ_DEV_CHG_VIDEO_PARAM �����Ӳɼ���������*/
/* OLD MSG=CLT_REQ_CHG_VIDEO_PARAM STRUCT=ST_CltChgVideoParamReq */

typedef struct SSrvReqDevChgVideoParam
{
	yv_InterCommHdr stInterHdr;
	ST_DataHdr stDataHdr;
	ST_CltChgVideoParam stCltChgVideoParam;
}ST_SrvReqDevChgVideoParam;

/*SRV_RES_DEV_CHG_VIDEO_PARAM ������Ƶ�ɼ�������Ӧ*/
/* OLD MSG=CLT_RES_CHG_VIDEO_PARAM STRUCT=ST_CltChgVideoParamRes */
typedef struct SSrvResDevChgVideoParam
{
	yv_InterCommHdr stInterHdr;
	ST_DataHdr stDataHdr;
	ST_MsgRespRet stMsgRet;
}ST_SrvResDevChgVideoParam;

/*�豸���ò�ѯ����SRV_REQ_DEV_PARAM_QUERY*/
typedef struct
{
	yv_InterCommHdr stInterHdr;     /* ҵ��ͷ*/
	ST_DataHdr stDataHdr;           /* ����ͷ*/
	ST_DevConfigQuery stQuery;      /* ���ݲ�*/
}ST_DevConfigQueryReq;

/*�豸���ò�ѯ��ӦSRV_RES_DEV_PARAM_QUERY*/
typedef struct
{
	yv_InterCommHdr stInterHdr;     /* ҵ��ͷ*/
	ST_DataHdr stDataHdr;           /* ����ͷ*/
	ST_MsgRespRet stMsgRet;         /* ��Ӧ���*/
	char content[0];                /*TLV��ʽ���������һ������TLV�ṹ������*/
}ST_DevConfigQueryRes;


/*�豸������������SRV_REQ_DEV_PARAM_SET*/
typedef struct
{
	yv_InterCommHdr stInterHdr;     /* ҵ��ͷ*/
	ST_DataHdr stDataHdr;           /* ����ͷ*/
	ST_DevConfigSet stSet;          /* ���ݲ�*/
}ST_DevConfigSetReq;

/*�豸����������ӦSRV_RES_DEV_PARAM_SET*/
typedef struct
{
	yv_InterCommHdr stInterHdr;     /* ҵ��ͷ*/
	ST_DataHdr stDataHdr;           /* ����ͷ*/
	ST_MsgRespRet stMsgRet;         /* ��Ӧ���*/
}ST_DevConfigSetRes;

/*��ȡ�豸��ͷ��������  SRV_REQ_DEV_GET_CAMERA_CFG*/
typedef struct SSrvReqDevGetCameraCfg
{
	yv_InterCommHdr stInterHdr;     /* ҵ��ͷ*/
	ST_DataHdr stDataHdr;           /* ����ͷ*/
} ST_SrvReqDevGetCameraCfg;


/*��ȡ�豸��ͷ������Ӧ  SRV_RES_DEV_GET_CAMERA_CFG*/
typedef struct SSrvResDevGetCameraCfg
{
	yv_InterCommHdr stInterHdr;     /* ҵ��ͷ*/
	ST_DataHdr stDataHdr;           /* ����ͷ*/
	ST_MsgRespRet stMsgRet;
	ST_CameraCfg stCameraCfg;   /*�豸��ͷ��������*/
} ST_SrvResDevGetCameraCfg;


/*��ȡ�豸ͼ���������� SRV_REQ_DEV_GET_VIDEO_COLOR_ATTR*/
typedef struct SSrvReqDevGetVideoColorAttr
{
	yv_InterCommHdr stInterHdr;     /* ҵ��ͷ*/
	ST_DataHdr stDataHdr;           /* ����ͷ*/
} ST_SrvReqDevGetVideoColorAttr;

/*��ȡ�豸ͼ��������Ӧ  SRV_RES_DEV_GET_VIDEO_COLOR_ATTR*/
typedef struct SSrvResDevGetVideoColorAttr
{
	yv_InterCommHdr stInterHdr;     /* ҵ��ͷ*/
	ST_DataHdr stDataHdr;           /* ����ͷ*/
	ST_MsgRespRet stMsgRet;
	ST_VideoColorAttr stVideoColorAttr;        /*�豸ͼ����������*/
} ST_SrvResDevGetVideoColorAttr;
/*��ȡ�ƶ������������SRV_REQ_DEV_GET_MOTTON_DETECT_ATTR*/
typedef struct SSrvReqDevGetMottonDetectAttr
{
	yv_InterCommHdr stInterHdr;     /* ҵ��ͷ*/
	ST_DataHdr stDataHdr;           /* ����ͷ*/
} ST_SrvReqDevGetMottonDetectAttr;
/*��ȡ�ƶ����������Ӧ SRV_RES_DEV_GET_MOTTON_DETECT_ATTR*/
typedef struct SSrvResDevGetMottonDetectAttr
{
	yv_InterCommHdr stInterHdr;     /* ҵ��ͷ*/
	ST_DataHdr stDataHdr;           /* ����ͷ*/
	ST_MsgRespRet stMsgRet;
	ST_MottonDetectAttr stMotDetAttr;   /*�ƶ��������*/
} ST_SrvResDevGetMottonDetectAttr;

/*�����豸ͼ����������  SRV_REQ_DEV_SET_VIDEO_COLOR_ATTR*/
typedef struct SSrvReqDevSetVideoColorAttr
{
	yv_InterCommHdr stInterHdr;     /* ҵ��ͷ*/
	ST_DataHdr stDataHdr;           /* ����ͷ*/
	ST_VideoColorAttr stVideoColorAttr;   /*�豸ͼ����������*/
} ST_SrvReqDevSetVideoColorAttr;

/*�����豸ͼ��������Ӧ  SRV_RES_DEV_SET_VIDEO_COLOR_ATTR*/
typedef struct SSrvResDevSetVideoColorAttr
{
	yv_InterCommHdr stInterHdr;     /* ҵ��ͷ*/
	ST_DataHdr stDataHdr;           /* ����ͷ*/
	ST_MsgRespRet stMsgRet;
} ST_SrvResDevSetVideoColorAttr;

/*�����豸��ͷ�������� SRV_REQ_DEV_SET_CAMERA_CFG*/
typedef struct SSrvReqDevSetCameraCfg
{
	yv_InterCommHdr stInterHdr;     /* ҵ��ͷ*/
	ST_DataHdr stDataHdr;           /* ����ͷ*/
	ST_CameraCfg stCameraCfg;   /*�豸��ͷ��������*/
} ST_SrvReqDevSetCameraCfg;

/*�����豸��ͷ������Ӧ  SRV_RES_DEV_SET_CAMERA_CFG*/
typedef struct SSrvResDevSetCameraCfg
{
	yv_InterCommHdr stInterHdr;     /* ҵ��ͷ*/
	ST_DataHdr stDataHdr;           /* ����ͷ*/
	ST_MsgRespRet stMsgRet;
} ST_SrvResDevSetCameraCfg;

/*�����ƶ������������SRV_REQ_DEV_SET_MOTTON_DETECT_ATTR*/
typedef struct SSrvReqDevSetMottonDetectAttr
{
	yv_InterCommHdr stInterHdr;     /* ҵ��ͷ*/
	ST_DataHdr stDataHdr;           /* ����ͷ*/
	ST_MottonDetectAttr stMotDetAttr;   /*�ƶ��������*/
} ST_SrvReqDevSetMottonDetectAttr;

/*�����ƶ����������Ӧ  SRV_RES_DEV_SET_MOTTON_DETECT_ATTR*/
typedef struct SSrvResDevSetMottonDetectAttr
{
	yv_InterCommHdr stInterHdr;     /* ҵ��ͷ*/
	ST_DataHdr stDataHdr;           /* ����ͷ*/
	ST_MsgRespRet stMsgRet;
} ST_SrvResDevSetMottonDetectAttr;

/*�豸ͼ�񳡾���ѯ������Ϣ SRV_REQ_DEV_GET_VIDEO_COLOR_SCENE*/
typedef struct SSrvReqDevGetVideoColorScene
{
	yv_InterCommHdr stInterHdr;     /* ҵ��ͷ*/
	ST_DataHdr stDataHdr;           /* ����ͷ*/
} ST_SrvReqDevGetVideoColorScene;

/*�豸ͼ�񳡾���ѯ��Ӧ��Ϣ SRV_RES_DEV_GET_VIDEO_COLOR_SCENE*/
typedef struct SSrvResDevGetVideoColorScene
{
	yv_InterCommHdr stInterHdr;     /* ҵ��ͷ*/
	ST_DataHdr stDataHdr;           /* ����ͷ*/
	ST_MsgRespRet stMsgRet;
	ST_VideoColorScene astVideoColorScene[MAX_VIDEO_COLOR_SCENE_NUM];/*�豸ͼ�񳡾���������*/
} ST_SrvResDevGetVideoColorScene;

/*Ӧ���豸ͼ�񳡾����� SRV_REQ_DEV_USE_VIDEO_COLOR_SCENE*/
typedef struct SSrvReqDevSetVideoColorScene
{
	yv_InterCommHdr stInterHdr;     /* ҵ��ͷ*/
	ST_DataHdr stDataHdr;           /* ����ͷ*/
	int lSceneID;/*���������: 1������һ   2��������  3��������*/
	char resel[8];
} ST_SrvReqDevSetVideoColorScene;

/*Ӧ���豸ͼ�񳡾���Ӧ SRV_RES_DEV_USE_VIDEO_COLOR_SCENE*/
typedef struct SSrvResDevSetVideoColorScene
{
	yv_InterCommHdr stInterHdr;     /* ҵ��ͷ*/
	ST_DataHdr stDataHdr;           /* ����ͷ*/
	ST_MsgRespRet stMsgRet;
} ST_SrvResDevSetVideoColorScene;

/*����ͼ�񳡾�������Ϣ  SRV_REQ_DEV_SET_VIDEO_COLOR_SCENE*/
typedef struct SSrvReqDevSevVideoColorScene
{
	yv_InterCommHdr stInterHdr;     /* ҵ��ͷ*/
	ST_DataHdr stDataHdr;           /* ����ͷ*/
	ST_VideoColorScene stVideoColorScene;   /*�豸ͼ�񳡾���������*/
} ST_SrvReqDevSevVideoColorScene;

/*����ͼ�񳡾���Ӧ��Ϣ  SRV_RES_DEV_SET_VIDEO_COLOR_SCENE*/
typedef struct SSrvResDevSevVideoColorScene
{
	yv_InterCommHdr stInterHdr;     /* ҵ��ͷ*/
	ST_DataHdr stDataHdr;           /* ����ͷ*/
	ST_MsgRespRet stMsgRet;
} ST_SrvResDevSevVideoColorScene;

/*�ָ�ͼ��Ĭ����������SRV_REQ_DEV_RESET_VIDEO_COLOR_ATTR*/
typedef struct SSrvReqDevResetVideoColorAttr
{
	yv_InterCommHdr stInterHdr;     /* ҵ��ͷ*/
	ST_DataHdr stDataHdr;           /* ����ͷ*/
} ST_SrvReqDevResetVideoColorAttr;

/*�ָ��豸ͼ��Ĭ��������ӦSRV_RES_DEV_RESET_VIDEO_COLOR_ATTR*/
typedef struct SSrvResDevResetVideoColorAttr
{
	yv_InterCommHdr stInterHdr;     /* ҵ��ͷ*/
	ST_DataHdr stDataHdr;           /* ����ͷ*/
	ST_MsgRespRet stMsgRet;
} ST_SrvResDevResetVideoColorAttr;
#endif
