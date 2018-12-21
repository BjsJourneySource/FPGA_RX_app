#ifndef __BV_NETMSG_H__
#define __BV_NETMSG_H__

#include <bv_type.h>

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define NET_HEADER_MAGIC   (0xDEADBEEF)

typedef enum bvNET_PARAMETER_ID_E{
    /*encode*/
    NET_PARAMETER_REQUEST_IDR = 0,
    NET_PARAMETER_VIDEO_STANDARD,
    NET_PARAMETER_VIDEO_ANALOG_CONFIG,

    NET_PARAMETER_VIDEO_BASIC,
    NET_PARAMETER_VIDEO_ADVANCE,
    NET_PARAMETER_VIDEO_EXTERNAL,

    /*system*/
    NET_PARAMETER_SYSTEM_INFO,
}NET_PARAMETER_ID_E;

typedef struct bvNET_PARAMETER_HEADER_S{
    NET_PARAMETER_ID_E enParameterId;
    BV_U32 u32ParameterLen;
    BV_U32 u32Channel;
    BV_U8  u8Payload[0];
}NET_PARAMETER_HEADER_S;

typedef struct bvNET_PARAMETER_REQUEST_IDR_S{
}NET_PARAMETER_REQUEST_IDR_S;

typedef struct bvNET_PARAMETER_VIDEO_STANDARD_S{
}NET_PARAMETER_VIDEO_STANDARD_S;

typedef struct bvNET_PARAMETER_VIDEO_ANALOG_CONFIG_S{
    BV_U32 u32AGC;
    BV_U32 u32WhiteBalance;
    BV_U32 u32BrightNess;
    BV_U32 u32Contrast;
    BV_U32 u32Sharpness;
    BV_U32 u32Hue;
}NET_PARAMETER_VIDEO_ANALOG_CONFIG_S;

typedef struct bvNET_PARAMETER_VIDEO_BASIC_S{
    BV_U32 u32Width;
    BV_U32 u32Height;
    BV_U32 u32FPS;
    BV_U32 u32BitRate;
    BV_U32 u32GOP;
    BV_U32 u32Interlace;
    BV_U32 u32RCType;
    BV_U32 u32QP;
    BV_U32 u32RCLevel;
}NET_PARAMETER_VIDEO_BASIC_S;

typedef struct bvNET_PARAMETER_VIDEO_ADVANCE_S{
}NET_PARAMETER_VIDEO_ADVANCE_S;

typedef struct bvNET_PARAMETER_VIDEO_EXTERNAL_S{
    BV_U8 u8Deinterlace;
    BV_U8 u8Skip;
    BV_U8 u8I4x4;
    BV_U8 u8Half;
    BV_U8 u8Quarter;
    BV_U8 u8Reserved[3];
    BV_U32 u32Mbdelay;
}NET_PARAMETER_VIDEO_EXTERNAL_S;

typedef struct bvNET_PARAMETER_SYSTEM_INFO_S{
}NET_PARAMETER_SYSTEM_INFO_S;

typedef struct bvNET_TIME_S{
    BV_U32 u32UtcSec;
    BV_U32 u32Usec;

    BV_U32 s32TimeZone;
}NET_TIME_S;

typedef NET_TIME_S NET_TIME_G;

typedef enum bvNET_MESSAGE_ID_E{
    NET_MESSAGE_DISCOVER = 0,
    NET_MESSAGE_LOGIN,
    NET_MESSAGE_QUIT,

    NET_MESSAGE_PRIVATE = 500,
    /*data message*/
    NET_MESSAGE_VIDEO_FRAME = 1000,
    NET_MESSAGE_AUDIO_FRAME,
    NET_MESSAGE_KEEPLIVE,

    /*control message*/
    NET_MESSAGE_REQUEST_SERVICE = 2000,
    NET_MESSAGE_ACK,
    NET_MESSAGE_PARAMETER_GET,
    NET_MESSAGE_PARAMETER_SET,


    /*parameter message*/
    NET_MESSAGE_DEVICE_IDENTIFY, //device info
	NET_MESSAGE_TIME_GET,
    NET_MESSAGE_TIME_SET,

    NET_MESSAGE_RESERVED,
}NET_MESSAGE_ID_E;

typedef struct bvNET_HEADER_S{
    BV_U32  u32HeaderMagic;
    BV_U32  u32SessionId;
    NET_MESSAGE_ID_E enMsgId;
    BV_U32  u32Ack;
    BV_U32  u32AckCode;
    BV_U32  u32MsgLen;
    BV_CHAR u8Payload[0];
}NET_HEADER_S;

typedef struct bvNET_MESSAGE_DISCOVER_S{
    BV_U32  u32MatchMask;
}NET_MESSAGE_DISCOVER_S;

typedef struct bvNET_MESSAGE_LOGIN_S{
    BV_CHAR u8UserName[16];
    BV_CHAR u8UserPasswd[64];
}NET_MESSAGE_LOGIN_S;

typedef struct bvNET_MESSAGE_QUIT_S{
}NET_MESSAGE_QUIT_S;

enum NET_MESSAGE_REQUEST_SERVICE_TYPE{
    NET_MESSAGE_REQUEST_SERVICE_VIDEO = 0,
    NET_MESSAGE_REQUEST_SERVICE_AUDIO,
};

typedef struct bvNET_MESSAGE_REQUEST_SERVICE_S{
    BV_U32 u32ServiceType;
    BV_U32 u32ServiceChannel;
    BV_U32 u32ServiceEnable;
}NET_MESSAGE_REQUEST_SERVICE_S;

typedef struct bvNET_MESSAGE_DEVICE_IDENTIFY_S{
    BV_CHAR	u8VendorID[16];
    BV_CHAR	u8ProductID[16];
    BV_CHAR	u8HardwareVersion[16];
    BV_CHAR	u8FirmwareVersion[16];
    BV_CHAR u8DeviceId[16];

    BV_CHAR u8DeviceName[64];

    BV_U32  u32MaxChannel;

    BV_CHAR	DeviceIP[64];//support for IPV4/IPV6
    BV_CHAR DeviceMAC[16];
    BV_U32  DevicePort;
}NET_MESSAGE_DEVICE_IDENTIFY_S;

typedef struct bvNET_MESSAGE_VIDEO_FRAME_S{
    BV_U32 u32Type;
    BV_U32 u32Channel;
    BV_U32 u32Width;
    BV_U32 u32Height;
    BV_U64 u64Timestamp;
    BV_U32 u32Codec;
    BV_U32 u32NalType;
    BV_U32 u32FrameLen;
    BV_U8  u8Payload[0];
}NET_MESSAGE_VIDEO_FRAME_S;

typedef struct bvNET_MESSAGE_AUDIO_FRAME_S{
    BV_U32 u32Type;
    BV_U32 u32Channel;
    BV_U64 u64Timestamp;
    BV_U32 u32Codec;
    BV_U32 u32FrameLen;
    BV_U8  u8Payload[0];
}NET_MESSAGE_AUDIO_FRAME_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __BV_NETMSG_H__ */

