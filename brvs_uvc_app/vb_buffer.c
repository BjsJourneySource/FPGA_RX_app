#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <stddef.h>

#include "list.h"
#include "bits.h"
#include "vb_buffer.h"

#define DYNAMIC_BUFFER_MALLOC 1

static int vb_packet_pool_init(struct vb_packet_pool *pool)
{
    if(pool){
        INIT_LIST_HEAD(&pool->head);
        lock_init(&pool->lock);

        return 0;
    }

    return -EINVAL;
}

static int vb_packet_pool_release(struct vb_packet_pool *pool)
{
    if(pool){
        lock_destroy(&pool->lock);

        return 0;
    }

    return -EINVAL;
}

static int vb_packet_pool_get_packet_from_pool(struct vb_packet_pool *pool, struct vb_packet **packet, u32 size)
{
    if(pool && packet){
        unsigned int scan_bits, base_bits;
#if !DYNAMIC_BUFFER_MALLOC
        int i, bits;
#endif

        *packet = NULL;
        size = ALIGN_PAGE(size);
        lock(&pool->lock);
        scan_bits = 0;
        base_bits = 0;
#if !DYNAMIC_BUFFER_MALLOC
        bits = LENS_TO_BITS(size);
        /*scan continue mask*/
        for(i = 0; i < pool->bits; i++){
            if(!test_bit(i, pool->mask)){
                scan_bits++;
                if(scan_bits == bits){
                    break;
                }
            }else{
                scan_bits = 0;
                base_bits = i;
            }
        }

        if(scan_bits != bits){
            unlock(&pool->lock);
            return -ENOMEM;
        }
#else
        scan_bits = 1;
#endif
        if((!list_empty(&pool->head)) && (scan_bits)){
            *packet = list_first_entry(&pool->head, struct vb_packet, head);
            list_del_init(&((*packet)->head));

            (*packet)->packet_size = size;
            (*packet)->base_bits = base_bits;
            (*packet)->bits = scan_bits;
            (*packet)->payload_len = 0;
            (*packet)->consumer_len = 0;
#if DYNAMIC_BUFFER_MALLOC
            (*packet)->data = malloc(size);
            if((*packet)->data == NULL){
                list_add_tail(&(*packet)->head, &pool->head);
                unlock(&pool->lock);

                return -ENOMEM;
            }
#else
            (*packet)->data = pool->data + (base_bits << ALIGN_BITS);
            for(i = base_bits; i <= (base_bits + scan_bits); i++){
                set_bit(i, pool->mask);
            }
#endif
        }
        unlock(&pool->lock);

        return 0;
    }

    return -EINVAL;
}

static int vb_packet_pool_put_packet_to_pool(struct vb_packet_pool *pool, struct vb_packet *packet)
{
    if(pool && packet){
#if !DYNAMIC_BUFFER_MALLOC
        unsigned int i;
#endif

        lock(&pool->lock);
#if DYNAMIC_BUFFER_MALLOC
        if(packet->data){
            free(packet->data);
            packet->data = NULL;
        }
#else
        for(i = packet->base_bits; i <= (packet->base_bits + packet->bits); i++){
            clear_bit(i, pool->mask);
        }
#endif
        packet->payload_len = 0;
        packet->consumer_len = 0;

        list_add_tail(&packet->head, &pool->head);
        unlock(&pool->lock);

        return 0;
    }

    return -EINVAL;
}

static int vb_packet_pool_get_entry_number(struct vb_packet_pool *pool)
{
    if(pool){
        int entry_number = 0;
        struct list_head *list;

        lock(&pool->lock);
        if(!list_empty(&(pool->head))){
            list_for_each(list, &(pool->head)){
                entry_number++;
            }
        }
        unlock(&pool->lock);

        return entry_number;
    }

    return -EINVAL;
}

static struct vb_packet_pool_operation vb_packet_pool_op = {
    .init = vb_packet_pool_init,
    .release = vb_packet_pool_release,

    .get_packet_from_pool = vb_packet_pool_get_packet_from_pool,
    .put_packet_to_pool = vb_packet_pool_put_packet_to_pool,
    .get_entry_number = vb_packet_pool_get_entry_number,
};


static int vb_packet_queue_init(struct vb_packet_queue *queue)
{
    if(queue){
        INIT_LIST_HEAD(&queue->head);
        queue->curr_producer = NULL;
        queue->curr_consumer = NULL;
        lock_init(&queue->lock);
        wait_init(&queue->wait);

        return 0;
    }

    return -EINVAL;
}

static int vb_packet_queue_release(struct vb_packet_queue *queue)
{
    if(queue){
        lock_destroy(&queue->lock);

        return 0;
    }

    return -EINVAL;
}

static int vb_packet_queue_get(struct vb_packet_queue *queue, struct vb_packet **packet)
{
    if(queue && packet){
        *packet = NULL;
        lock(&queue->lock);
        if(!list_empty(&queue->head)){
            *packet = list_first_entry(&queue->head, struct vb_packet, head);
            list_del_init(&((*packet)->head));
        }
        unlock(&queue->lock);

        return 0;
    }

    return -EINVAL;
}

static int vb_packet_queue_put(struct vb_packet_queue *queue, struct vb_packet *packet)
{
    if(queue && packet){
        lock(&queue->lock);
        list_add_tail(&packet->head, &queue->head);
        unlock(&queue->lock);
        wait_wakeup(&queue->wait);

        return 0;
    }

    return -EINVAL;
}

static int vb_packet_queue_get_curr_consumer(struct vb_packet_queue *queue)
{
    if(queue){
        if(queue->curr_consumer){
            return 0;
        }
        lock(&queue->lock);
        queue->curr_consumer = NULL;
        if(!list_empty(&queue->head)){
            queue->curr_consumer = list_first_entry(&queue->head, struct vb_packet, head);
            list_del_init(&(queue->curr_consumer->head));
        }
        unlock(&queue->lock);

        return 0;
    }

    return -EINVAL;
}

static int vb_packet_queue_release_curr_consumer(struct vb_packet_queue *queue, struct vb_packet_pool *pool)
{
    if(queue && pool && queue->curr_consumer){
        lock(&queue->lock);
        pool->op->put_packet_to_pool(pool, queue->curr_consumer);
        queue->curr_consumer = NULL;
        unlock(&queue->lock);

        return 0;
    }

    return -EINVAL;
}

static int vb_packet_queue_get_curr_producer(struct vb_packet_queue *queue, struct vb_packet_pool *pool, u32 len)
{
    if(queue && (queue->curr_producer == NULL)){
        lock(&queue->lock);
        pool->op->get_packet_from_pool(pool, &queue->curr_producer, len);
        unlock(&queue->lock);

        return 0;
    }

    return -EINVAL;
}

static int vb_packet_queue_put_curr_producer(struct vb_packet_queue *queue)
{
    if(queue && queue->curr_producer){
        lock(&queue->lock);
        list_add_tail(&(queue->curr_producer->head), &queue->head);
        queue->curr_producer = NULL;
        unlock(&queue->lock);
        wait_wakeup(&queue->wait);

        return 0;
    }

    return -EINVAL;
}

static int vb_packet_queue_release_curr_producer(struct vb_packet_queue *queue, struct vb_packet_pool *pool)
{
    if(queue && pool && queue->curr_producer){
        lock(&queue->lock);
        pool->op->put_packet_to_pool(pool, queue->curr_producer);
        queue->curr_producer = NULL;
        unlock(&queue->lock);

        return 0;
    }

    return -EINVAL;
}

static int vb_packet_queue_get_entry_number(struct vb_packet_queue *queue)
{
    if(queue){
        int entry_number = 0;
        struct list_head *list;

        lock(&queue->lock);
        if(!list_empty(&queue->head)){
            list_for_each(list, &queue->head){
                entry_number++;
            }
        }
        unlock(&queue->lock);
        return entry_number;
    }

    return -EINVAL;
}

static int vb_packet_queue_wait(struct vb_packet_queue *queue)
{
    if(queue){
        wait_complete(&queue->wait);

        return 0;
    }

    return -EINVAL;
}

static struct vb_packet_queue_operation vb_packet_queue_op = {
    .init = vb_packet_queue_init,
    .release = vb_packet_queue_release,

    .get = vb_packet_queue_get,
    .put = vb_packet_queue_put,

    .get_curr_consumer = vb_packet_queue_get_curr_consumer,
    .release_curr_consumer = vb_packet_queue_release_curr_consumer,
    .get_curr_producer = vb_packet_queue_get_curr_producer,
    .put_curr_producer = vb_packet_queue_put_curr_producer,
    .release_curr_producer = vb_packet_queue_release_curr_producer,
    .get_entry_number = vb_packet_queue_get_entry_number,
    .wait = vb_packet_queue_wait,
};


int init_packet_queue(struct vb_packet_queue *queue)
{
    if(queue){
        queue->op = &vb_packet_queue_op;
        return queue->op->init(queue);
    }

    return -EINVAL;
}

int remove_packet_queue(struct vb_packet_queue *queue)
{
    if(queue){
         if(queue->op){
            return queue->op->release(queue);
        }
    }

    return -EINVAL;
}

struct vb_packet_pool *create_vb_packet_pool(u32 entry, u32 size)
{
    if(entry <= MAX_CACHE_PACKET_ENTRY){
        struct vb_packet_pool *pool;
        struct vb_packet *packet;
        unsigned int i;

        pool = (struct vb_packet_pool *)malloc(sizeof(struct vb_packet_pool));
        if(!pool){
            printf("can't malloc buffer for pool\n");
            return NULL;
        }
        memset(pool, 0, sizeof(struct vb_packet_pool));
        pool->op = &vb_packet_pool_op;
        pool->op->init(pool);
        pool->entry_number = entry;
        pool->packet = (struct vb_packet *)malloc(sizeof(struct vb_packet) * entry);
        if(!pool->packet){
            printf("can't malloc buffer for packet\n");
            goto error;
        }
        memset(pool->packet, 0, sizeof(sizeof(struct vb_packet) * entry));
        
        pool->data_size = ALIGN_PAGE(size); //align to 1K
        pool->bits = pool->data_size >> 10;
        pool->mask = (unsigned long *)malloc(BITS_TO_LONGS(pool->bits) * sizeof(long));
        if(!pool->mask){
            printf("can't malloc buffer for data mask\n");
            goto error;
        }
        bitmap_zero(pool->mask, pool->bits);

        for(i = 0; i < entry; i++){
            packet = &pool->packet[i];
            packet->packet_id = i;
            packet->base_bits = 0;
            packet->bits = 0;
            INIT_LIST_HEAD(&packet->head);
            packet->data = NULL;
            packet->payload_len = 0;
            packet->packet_size = 0;
            pool->op->put_packet_to_pool(pool, packet);
        }
#if !DYNAMIC_BUFFER_MALLOC
        pool->data = (u8 *)malloc(pool->data_size);
        if(!pool->data){
            printf("can't malloc buffer for data pool\n");
            goto error;
        }
#endif


        return pool;
error:
        if(pool){
            if(pool->packet){
                free(pool->packet);
                pool->packet = NULL;
            }
#if !DYNAMIC_BUFFER_MALLOC
            if(pool->data){
                free(pool->data);
                pool->data = NULL;
            }
#endif
            if(pool->mask){
                free(pool->mask);
                pool->mask = NULL;
            }
            free(pool);
        }

        return NULL;
    }

    return NULL;
}


void destroy_vb_packet_pool(struct vb_packet_pool * packet_pool)
{
    if(packet_pool){
        if(packet_pool->packet){
            free(packet_pool->packet);
            packet_pool->packet = NULL;

        }
#if !DYNAMIC_BUFFER_MALLOC
        if(packet_pool->data){
            free(packet_pool->data);
            packet_pool->data = NULL;
        }
#endif
        if(packet_pool->mask){
            free(packet_pool->mask);
            packet_pool->mask = NULL;
        }
        free(packet_pool);
    }
}
