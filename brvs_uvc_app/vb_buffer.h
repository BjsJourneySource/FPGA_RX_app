#ifndef __NETWORK_BUFFER_H__
#define __NETWORK_BUFFER_H__

#include "list.h"
#include "types.h"
#include "lock.h"
#include "wait.h"

#define MAX_CACHE_PACKET_ENTRY   (512)

struct vb_packet;
struct vb_packet_queue;
struct vb_packet_pool;

struct vb_packet{
    u32 packet_id;

    struct list_head head;

    u8 *data;
    u32 payload_len;
    u32 consumer_len;

    u32 packet_size;
    u32 base_bits;
    u32 bits;

    void *private;
};

struct vb_packet_pool_operation{
    int (*init)(struct vb_packet_pool *);
    int (*release)(struct vb_packet_pool *);

    int (*get_packet_from_pool)(struct vb_packet_pool *, struct vb_packet **, u32 );
    int (*put_packet_to_pool)(struct vb_packet_pool *, struct vb_packet *);
    int (*get_entry_number)(struct vb_packet_pool *);
};

struct vb_packet_pool{
    int entry_number;

    int mmap;
    struct list_head head;
    lock_t lock;
    struct vb_packet *packet;
    u8 *data;
    u32 data_size;
    unsigned long *mask;
    u32 bits;

    struct vb_packet_pool_operation *op;
};

struct vb_packet_queue_operation{
    int (*init)(struct vb_packet_queue *);
    int (*release)(struct vb_packet_queue *);

    int (*get_curr_consumer)(struct vb_packet_queue *);
    int (*get)(struct vb_packet_queue *, struct vb_packet **);
    int (*put)(struct vb_packet_queue *, struct vb_packet *);
    int (*release_curr_consumer)(struct vb_packet_queue *, struct vb_packet_pool *);
    int (*get_curr_producer)(struct vb_packet_queue *, struct vb_packet_pool *, u32 );
    int (*put_curr_producer)(struct vb_packet_queue *);
    int (*release_curr_producer)(struct vb_packet_queue *, struct vb_packet_pool *);
    int (*get_entry_number)(struct vb_packet_queue *);
    int (*wait)(struct vb_packet_queue *);
};

struct vb_packet_queue{
    struct list_head head;

    lock_t lock;

    wait_t wait;

    struct vb_packet *curr_producer;
    struct vb_packet *curr_consumer;

    struct vb_packet_queue_operation *op;
};

int init_packet_queue(struct vb_packet_queue *queue);
int remove_packet_queue(struct vb_packet_queue *queue);

struct vb_packet_pool *create_vb_packet_pool(u32 entry, u32 size);
void destroy_vb_packet_pool(struct vb_packet_pool * packet_pool);

#endif //__NETWORK_BUFFER_H__
