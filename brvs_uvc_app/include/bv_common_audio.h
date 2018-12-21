#ifndef __BV_COMM_AUDIO_H__
#define __BV_COMM_AUDIO_H__

#include "bv_type.h"
#include "bv_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

typedef struct bvAUDIO_STREAM_S 
{ 
    BV_U32 u32Codec; 
    BV_U8 *pVirAddr;     
    BV_U32 u32PhyAddr; 
    BV_U32 u32Len;    
    BV_U64 u64TimeStamp;
    BV_U32 u32Seq;     
} AUDIO_STREAM_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __BV_COMM_AUDIO_H__ */ 
