#ifndef __LOCK_H__
#define __LOCK_H__

#ifdef __KERNEL__
#include <linux/spinlock.h>
#else
#include <pthread.h>
#endif

typedef struct{
#ifdef __KERNEL__
    spinlock_t lock; 
#else
    pthread_mutex_t lock;
#endif
}lock_t;

extern void lock_init(lock_t *);
extern void lock(lock_t *);
extern int try_lock(lock_t *);
extern void unlock(lock_t *);
extern void lock_destroy(lock_t *);

#endif //__LOCK_H__
