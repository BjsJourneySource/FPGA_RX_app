#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
//#include <linux/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stddef.h>
#include <sys/ioctl.h>
#include <wait.h>

#ifdef __KERNEL__
void wait_init(wait_t *)
{
}

void wait_complete(wait_t *)
{
}

void wait_wakeup(wait_t *)
{
}

void wait_wakeup_all(wait_t *)
{
}

void wait_destroy(wait_t *)
{
}

void kernel_wait_init(kernel_wait_t *wait)
{
}

void kernel_wait(kernel_wait_t *wait, struct timeval *tv)
{
}

void kernel_wait_wakeup(kernel_wait_t *wait)
{
}

void kernel_wait_destroy(kernel_wait_t *wait)
{
}

#else
void wait_init(wait_t *wait)
{
    if(wait){
        pthread_cond_init(&wait->wait, NULL);
        pthread_mutex_init(&wait->lock, NULL);
    }
}

void wait_complete(wait_t *wait)
{
    if(wait){
        pthread_mutex_lock(&wait->lock);
        pthread_cond_wait(&wait->wait, &wait->lock);
        pthread_mutex_unlock(&wait->lock);
    }
}

void wait_wakeup(wait_t *wait)
{
    if(wait){
        pthread_mutex_lock(&wait->lock);
        pthread_cond_signal(&wait->wait);
        pthread_mutex_unlock(&wait->lock);
    }
}

void wait_wakeup_all(wait_t *wait)
{
    if(wait){
        pthread_mutex_lock(&wait->lock);
        pthread_cond_broadcast(&wait->wait);
        pthread_mutex_unlock(&wait->lock);
    }
}

void wait_destroy(wait_t *wait)
{
    if(wait){
        pthread_cond_destroy(&wait->wait);
        pthread_mutex_destroy(&wait->lock);
    }
}

void kernel_wait_init(kernel_wait_t *wait)
{
    if(wait){
        wait->fd = open("/dev/bv_event", O_RDWR, 0655);
    }
}

int kernel_wait(kernel_wait_t *wait, struct timeval *tv)
{
    if(wait && tv && (wait->fd > 0)){
        fd_set fdread;

        FD_ZERO(&fdread);
        FD_SET(wait->fd, &fdread);

        return select(wait->fd + 1, &fdread, NULL, NULL, tv);
    }

    return -1;
}

#define BV_EVENT_IOC_WAKEUP     _IOW('e', 0, unsigned int)

int kernel_wait_wakeup(kernel_wait_t *wait)
{
    if(wait && (wait->fd > 0)){
        return ioctl(wait->fd, BV_EVENT_IOC_WAKEUP, 0);
    }

    return -1;
}

void kernel_wait_destroy(kernel_wait_t *wait)
{
    if(wait && (wait->fd > 0)){
        close(wait->fd);
        wait->fd = -1;
    }
}

#endif
