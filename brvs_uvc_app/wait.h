#ifndef __WAIT_H__
#define __WAIT_H__

#ifdef __KERNEL__
#include <linux/wait.h>
#else
#include <pthread.h>
#endif

typedef struct {
#ifdef __KERNEL__
    wait_queue_head_t	wait;
#else
    pthread_cond_t      wait;
    pthread_mutex_t     lock;
#endif
}wait_t;

typedef struct{
    int fd;
}kernel_wait_t;

void wait_init(wait_t *);
void wait_complete(wait_t *);
void wait_wakeup(wait_t *);
void wait_wakeup_all(wait_t *);
void wait_destroy(wait_t *);

/*please use eventfd*/
void kernel_wait_init(kernel_wait_t *);
int kernel_wait(kernel_wait_t *, struct timeval *tv);
int kernel_wait_wakeup(kernel_wait_t *);
void kernel_wait_destroy(kernel_wait_t *);

#endif //__WAIT_H__

