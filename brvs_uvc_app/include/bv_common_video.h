#ifndef __BV_COMM_VIDEO_H__
#define __BV_COMM_VIDEO_H__

#include "bv_type.h"
#include "bv_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define	H264_FRAME_TYPE_IDR		(0)
#define	H264_FRAME_TYPE_I		(1)
#define	H264_FRAME_TYPE_P		(2)
#define	H264_FRAME_TYPE_B		(3)
#define	H264_FRAME_TYPE_SI		(4)
#define	H264_FRAME_TYPE_SP		(5)

typedef enum bvVIDEO_STANDARD_E
{
    VIDEO_STANDARD_PAL=0,
    VIDEO_STANDARD_NTSC,
    VIDEO_STANDARD_AUTO,
    VIDEO_STANDARD_RESERVED,
} VIDEO_STANDARD_E;

enum bvVIDEO_BUS_TYPE{
    VIDEO_BUS_BT601 = 0,
    VIDEO_BUS_BT656,
    VIDEO_BUS_BT709,
    VIDEO_BUS_BT1120,
    VIDEO_BUS_RESERVED,
};

typedef enum bvPIXEL_FORMAT_E
{   
    PIXEL_FORMAT_RGB_1BPP = 0,
    PIXEL_FORMAT_RGB_2BPP,
    PIXEL_FORMAT_RGB_4BPP,
    PIXEL_FORMAT_RGB_8BPP,
    PIXEL_FORMAT_RGB_444,
    PIXEL_FORMAT_RGB_4444,
    PIXEL_FORMAT_RGB_555,
    PIXEL_FORMAT_RGB_565,
    PIXEL_FORMAT_RGB_1555,

    PIXEL_FORMAT_RGB_888,
    PIXEL_FORMAT_RGB_8888,
    PIXEL_FORMAT_RGB_PLANAR_888,
    PIXEL_FORMAT_RGB_BAYER,

    PIXEL_FORMAT_YUV_A422,
    PIXEL_FORMAT_YUV_A444,

    PIXEL_FORMAT_YUV_PLANAR_422,
    PIXEL_FORMAT_YUV_PLANAR_420,
    PIXEL_FORMAT_YUV_PLANAR_444,

    PIXEL_FORMAT_YUV_SEMIPLANAR_422,
    PIXEL_FORMAT_YUV_SEMIPLANAR_420,
    PIXEL_FORMAT_YUV_SEMIPLANAR_444,

    PIXEL_FORMAT_UYVY_PACKAGE_422,
    PIXEL_FORMAT_YUYV_PACKAGE_422,
    PIXEL_FORMAT_VYUY_PACKAGE_422,
    PIXEL_FORMAT_YCbCr_PLANAR,

    PIXEL_FORMAT_RESERVED,   
} PIXEL_FORMAT_E;

typedef enum bvVIDEO_FIELD_E
{
    VIDEO_FIELD_TOP         = 0x01,
    VIDEO_FIELD_BOTTOM      = 0x02,
    VIDEO_FIELD_INTERLACED  = 0x03,
    VIDEO_FIELD_FRAME       = 0x04,

    VIDEO_FIELD_RESERVED,
} VIDEO_FIELD_E;

typedef enum bvVIDEO_CODEC_E
{
    VIDEO_CODEC_H264E = 0,
    VIDEO_CODEC_H265E,
    VIDEO_CODEC_MJPEGE,

    VIDEO_CODEC_RESERVED,
} VIDEO_CODEC_E;

typedef enum bvSLICE_TYPE_E
{
    SLICE_TYPE_B         = 0,
    SLICE_TYPE_P         = 1,
    SLICE_TYPE_I         = 2,
    SLICE_TYPE_RESERVED,

} SLICE_TYPE_E;

typedef enum bvH264E_NALU_TYPE_E
{
    H264E_NALU_UNKNOWN         = 0,
    H264E_NALU_SLICE           = 1,
    H264E_NALU_SLICE_DPA       = 2,
    H264E_NALU_SLICE_DPB       = 3,
    H264E_NALU_SLICE_DPC       = 4,
    H264E_NALU_SLICE_IDR       = 5,
    H264E_NALU_SEI             = 6,
    H264E_NALU_SPS             = 7,
    H264E_NALU_PPS             = 8,

    H264E_NALU_RESERVED, 
} H264E_NALU_TYPE_E;

typedef enum bvH265E_NALU_TYPE_E
{
    NAL_UNIT_CODED_SLICE_TRAIL_N = 0,   // 0
    NAL_UNIT_CODED_SLICE_TRAIL_R,   // 1

    NAL_UNIT_CODED_SLICE_TSA_N,     // 2
    NAL_UNIT_CODED_SLICE_TLA,       // 3   // Current name in the spec: TSA_R

    NAL_UNIT_CODED_SLICE_STSA_N,    // 4
    NAL_UNIT_CODED_SLICE_STSA_R,    // 5

    NAL_UNIT_CODED_SLICE_RADL_N,    // 6
    NAL_UNIT_CODED_SLICE_DLP,       // 7 // Current name in the spec: RADL_R

    NAL_UNIT_CODED_SLICE_RASL_N,    // 8
    NAL_UNIT_CODED_SLICE_TFD,       // 9 // Current name in the spec: RASL_R

    NAL_UNIT_RESERVED_10,
    NAL_UNIT_RESERVED_11,
    NAL_UNIT_RESERVED_12,
    NAL_UNIT_RESERVED_13,
    NAL_UNIT_RESERVED_14,
    NAL_UNIT_RESERVED_15,

    NAL_UNIT_CODED_SLICE_BLA,       // 16   // Current name in the spec: BLA_W_LP
    NAL_UNIT_CODED_SLICE_BLANT,     // 17   // Current name in the spec: BLA_W_DLP
    NAL_UNIT_CODED_SLICE_BLA_N_LP,  // 18
    NAL_UNIT_CODED_SLICE_IDR,       // 19  // Current name in the spec: IDR_W_DLP
    NAL_UNIT_CODED_SLICE_IDR_N_LP,  // 20
    NAL_UNIT_CODED_SLICE_CRA,       // 21
    NAL_UNIT_RESERVED_22,
    NAL_UNIT_RESERVED_23,

    NAL_UNIT_RESERVED_24,
    NAL_UNIT_RESERVED_25,
    NAL_UNIT_RESERVED_26,
    NAL_UNIT_RESERVED_27,
    NAL_UNIT_RESERVED_28,
    NAL_UNIT_RESERVED_29,
    NAL_UNIT_RESERVED_30,
    NAL_UNIT_RESERVED_31,

    NAL_UNIT_VPS,                   // 32
    NAL_UNIT_SPS,                   // 33
    NAL_UNIT_PPS,                   // 34
    NAL_UNIT_ACCESS_UNIT_DELIMITER, // 35
    NAL_UNIT_EOS,                   // 36
    NAL_UNIT_EOB,                   // 37
    NAL_UNIT_FILLER_DATA,           // 38
    NAL_UNIT_SEI,                   // 39 Prefix SEI
    NAL_UNIT_SEI_SUFFIX,            // 40 Suffix SEI
    NAL_UNIT_RESERVED_41,
    NAL_UNIT_RESERVED_42,
    NAL_UNIT_RESERVED_43,
    NAL_UNIT_RESERVED_44,
    NAL_UNIT_RESERVED_45,
    NAL_UNIT_RESERVED_46,
    NAL_UNIT_RESERVED_47,
    NAL_UNIT_UNSPECIFIED_48,
    NAL_UNIT_UNSPECIFIED_49,
    NAL_UNIT_UNSPECIFIED_50,
    NAL_UNIT_UNSPECIFIED_51,
    NAL_UNIT_UNSPECIFIED_52,
    NAL_UNIT_UNSPECIFIED_53,
    NAL_UNIT_UNSPECIFIED_54,
    NAL_UNIT_UNSPECIFIED_55,
    NAL_UNIT_UNSPECIFIED_56,
    NAL_UNIT_UNSPECIFIED_57,
    NAL_UNIT_UNSPECIFIED_58,
    NAL_UNIT_UNSPECIFIED_59,
    NAL_UNIT_UNSPECIFIED_60,
    NAL_UNIT_UNSPECIFIED_61,
    NAL_UNIT_UNSPECIFIED_62,
    NAL_UNIT_UNSPECIFIED_63,
    NAL_UNIT_INVALID,
} H265E_NALU_TYPE_E;

typedef enum bvJPEGE_PACK_TYPE_E{
    JPEG_PACK_RESERVED, 
}JPEGE_PACK_TYPE_E;
typedef struct bvVIDEO_DATA_TYPE_U
{
    VIDEO_CODEC_E codec;

    union {
        H264E_NALU_TYPE_E    enH264EType;
        H265E_NALU_TYPE_E    enH265EType;
        JPEGE_PACK_TYPE_E    enJPEGEType;
    };
}VIDEO_DATA_TYPE_S;

typedef struct bvVIDEO_FRAME_S
{    
    BV_U32          u32Width;
    BV_U32          u32Height;    
    VIDEO_FIELD_E   u32Field;
    PIXEL_FORMAT_E  enPixelFormat;

    BV_U32          u32PhyAddr[3];
    BV_VOID         *pVirAddr[3];
    BV_U32          u32Stride[3];

    BV_U16          u16OffsetTop;    
    BV_U16          u16OffsetBottom;
    BV_U16          u16OffsetLeft; 
    BV_U16          u16OffsetRight;

    BV_U64          u64pts;
    BV_U32          u32TimeRef;

    BV_SLONG        slPrivateData;
}VIDEO_FRAME_S;

typedef struct bvVIDEO_PACK_S
{
    BV_U32   u32PhyAddr[2]; 
    BV_VOID   *pVirAddr[2];
    BV_U32   u32Len[2];
    
    BV_U64   u64PTS; 
    BV_U32  bFieldEnd;
    BV_U32  bFrameEnd;
    
    VIDEO_DATA_TYPE_S  dataType;

    BV_SLONG        slPrivateData;
}VIDEO_PACK_S;

typedef struct bvVIDEO_STREAM_S
{
    VIDEO_PACK_S         *pstPack;
    BV_U32              u32PackCount;
    BV_U32              u32Seq;    
}VIDEO_STREAM_S;

typedef struct hiBITMAP_S
{
    PIXEL_FORMAT_E enPixelFormat;  

    BV_U32 u32Width;              
    BV_U32 u32Height;            
    BV_VOID *pData;             
} BITMAP_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* _BV_COMM_VIDEO_H_ */ 
