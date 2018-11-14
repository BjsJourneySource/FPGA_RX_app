#ifndef _NALU_H_
#define _NALU_H_

#include <stdbool.h>

struct h264_frame_info {
    bool is_i_frame;
    int header_offset;
    int header_length;
    int data_offset;
    int data_length;
};

//unsigned char* FindNextH264StartCode(unsigned char *pBuf, unsigned int Buf_len);
unsigned char* FindNextH264StartCode(unsigned char *pBuf, unsigned char *pBuf_end);

bool h264_decode_seq_parameter_set(unsigned char *buf, unsigned int nLen, unsigned int *Width, unsigned int *Height);

bool h264_frame_parse(unsigned char *buf, int offset, int length, struct h264_frame_info *info);

#if 0
//! NAL unit structure
typedef struct nalu_sps
{
    unsigned char profile_idc;
    unsigned constraint_set0_flag:1;
    unsigned constraint_set1_flag:1;
    unsigned constraint_set2_flag:1;
    unsigned constraint_set3_flag:1;
    unsigned reserved_zero_4bits:4;
    unsigned char level_idc;
    unsigned seq_parameter_set_id:1;
    unsigned log2_max_frame_num_minus4:5;
} NALU_SPS;
#endif

#endif // _NALU_H_
