#ifndef __BV_TYPE_H__
#define __BV_TYPE_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#ifndef BITS_PER_LONG
#define BITS_PER_LONG 32
#endif 

typedef unsigned char           BV_U8;
typedef unsigned short          BV_U16;
typedef unsigned int            BV_U32;
typedef unsigned long           BV_ULONG;

typedef signed char             BV_S8;
typedef short                   BV_S16;
typedef int                     BV_S32;
typedef long                    BV_SLONG;

#if BITS_PER_LONG == 32
typedef unsigned long long  BV_U64;
typedef long long           BV_S64;
#else
typedef unsigned long long  BV_U64;
typedef long long           BV_S64;
#endif

typedef char                    BV_CHAR;
#define BV_VOID                 void

typedef enum {
    BV_FALSE = 0,
    BV_TRUE  = 1,
} BV_BOOL;

#ifndef NULL
    #define NULL    0L
#endif

#define BV_NULL     0L
#define BV_SUCCESS  0
#define BV_FAILURE  (-1)


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __BV_TYPE_H__ */

