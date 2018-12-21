#ifndef __USER_TYPE_H__
#define __USER_TYPE_H__
#include <sys/time.h>
#include <asm/byteorder.h>

#include <bv_type.h>
#include <bv_common.h>
#include <bv_errno.h>

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;
typedef unsigned long long u64;

#define BV_AP_ERR(level, err) BV_DEF_ERR(BV_APP_ID_LINUX_AP, 0, level, err)

#define __used __attribute__((__used__))

#define __user

#undef offsetof
#ifdef __compiler_offsetof
#define offsetof(TYPE,MEMBER) __compiler_offsetof(TYPE,MEMBER)
#else
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

#define container_of(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) );})

#ifndef MAX
#define MAX(a, b) ((a) >= (b) ? (a) : (b))
#endif //MAX

#ifndef MIN
#define MIN(a, b) ((a) >= (b) ? (b) : (a))
#endif //MIN

/*make little endian default*/
#if defined(__LITTLE_ENDIAN_BITFIELD)
#elif defined(__BIG_ENDIAN_BITFIELD)
#else
#error	"Please fix <asm/byteorder.h>"
#endif

#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>

static pid_t __used gettid(void)
{
  return syscall(SYS_gettid);
}
#endif //__USER_TYPE_H__
