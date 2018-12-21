#include <lock.h>

void lock_init(lock_t *lock)
{
    if(lock){
#ifdef __KERNEL__
        spin_lock_init(&lock->lock);
#else
        pthread_mutex_init(&lock->lock, NULL);
#endif
    }
}

void lock(lock_t *lock)
{
    if(lock){
#ifdef __KERNEL__
        spin_lock(&lock->lock);
#else
        pthread_mutex_lock(&lock->lock);
#endif
    }
}

int try_lock(lock_t *lock)
{
    if(lock){
#ifdef __KERNEL__
        return spin_trylock(&lock->lock);
#else
        return pthread_mutex_trylock(&lock->lock);
#endif
    }

    return -1;
}

void unlock(lock_t *lock)
{
    if(lock){
#ifdef __KERNEL__
        spin_unlock(&lock->lock);
#else
        pthread_mutex_unlock(&lock->lock);
#endif
    }
}

void lock_destroy(lock_t *lock)
{
    if(lock){
#ifdef __KERNEL__
#else
        pthread_mutex_destroy(&lock->lock);
#endif
    }
}
