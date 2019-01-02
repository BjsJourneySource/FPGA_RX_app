#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <strings.h>
#include <signal.h>
#include <getopt.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sched.h>
#include <pthread.h>
#include <asm/types.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "vb_buffer.h"
#include <stdint.h>
#include <sys/select.h>

#include "nalu.h"
#include "protocol.h"
#include "protocol_errno.h"


typedef  unsigned char          u8;
typedef  unsigned short         u16;
typedef  unsigned int           u32;

#define  MEM_DEVICE             "/dev/mem"

#define  RF_REG_MAP_SIZE        0x100000
#define  RF_REG_MEM_BASE        0x43c00000

#define  RF_RECV_BUF_SIZE       0x100000
#define  RF_RECV_BUF_BASE       0x0f000000

#define  REG_RF_CONTROL         (0x3FFFC)

#define  RF_ON                  (1 << 0)
#define  RF_RESET               (0 << 0)

#define  CH_0                   0
#define  CH_1                   1
#define  CH_2                   2
#define  CH_3                   3
#define  CH_4                   4
#define  CH_5                   5

#define  REG_A_STATUS_CH(ch)    (0x10 + ch * 0x10)
#define  REG_A_LEN_CH(ch)       (0x14 + ch * 0x10)
#define  REG_B_STATUS_CH(ch)    (0x18 + ch * 0x10)
#define  REG_B_LEN_CH(ch)       (0x1c + ch * 0x10)

#define  BUF_READY              (1 << 0)

#define  BUF_A_DATA_CH(ch)      (0 * 0x800 + ch * 0x1000)
#define  BUF_B_DATA_CH(ch)      (1 * 0x800 + ch * 0x1000)

struct sync_head { 
	u32  magic;
	u32  id;
	u32  len;
    u8   data[0];
};

#define RF_VALID_LEN            (548)
#define RF_BUF_SIZE             (100 * 1024)
#define SYNC_HEAD_MAGIC         0xfefefefe

static u8 rf_buf[RF_BUF_SIZE];

static volatile u32 reg_base;
static volatile u32 data_base;

//bjs ++
#define v4l2_fourcc(a, b, c, d)\
((__u32)(a) | ((__u32)(b) << 8) | ((__u32)(c) << 16) | ((__u32)(d) << 24))

#undef PIX_FMT_H264
#define PIX_FMT_H264 v4l2_fourcc('H','2','6','4') /* H264 */
#undef PIX_FMT_MP2T
#define PIX_FMT_MP2T v4l2_fourcc('M','P','2','T') /* MPEG-2 TS */

typedef struct {
	/* Option info */
	short do_capture;
	short do_upload;
	short do_quit;			//stop the capture thread.
	short do_xu_get_br;
	short do_xu_set_br;
	short prot_online;		//if connecting to remote server

	/* Media info */
	unsigned int width;
	unsigned int height;
	unsigned int framerate;
	int pixelformat;
	unsigned int delay;
	double bitrate;

	/* Buffer info */
	//unsigned int nframes;
	//unsigned int nbufs;
	//void *mem0[V4L_BUFFERS_MAX];
	//size_t mlength;	//length of mmap

	//char *dev_name;
	//int devfd;
	unsigned long retval;
}tVidInfo;

tVidInfo g_VidInfo = {0};
//bjs --


struct RF_receiver {
    struct vb_packet_pool *tx_pool;
    struct vb_packet_queue tx_queue;
};

struct RF_receiver RF_recv;

static u32 reg_read(unsigned int addr)
{
	u32 value;
	value = *((volatile unsigned int *)(addr + reg_base));
	return value;
}

static void reg_write(unsigned int addr, unsigned int value)
{
	*((volatile u32 *)(addr + reg_base)) = value;
}

static u32 data_read(unsigned int addr)
{
	u32 value;
	value = *((volatile unsigned int *)(addr + data_base));
	return value;
}

static void data_write(unsigned int addr, unsigned int value)
{
	*((volatile u32 *)(addr + data_base)) = value;
}

static int mmap_init()
{
	int fd;

	fd = open(MEM_DEVICE, O_RDWR, 0);

	if (fd == -1) {
		printf("cannot open %s (%s)\n", MEM_DEVICE, strerror(errno));
		return -1;
	}

	reg_base = (unsigned long)mmap(NULL, RF_REG_MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, RF_REG_MEM_BASE);
	if((int *)reg_base == NULL || (int)reg_base == -1) {
		printf("reg mmap failed.\n");
		return -1;
	}

	data_base = (unsigned long)mmap(NULL, RF_RECV_BUF_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, RF_RECV_BUF_BASE);
	if((int *)data_base == NULL || (int)data_base == -1) {
		printf("buf mmap failed.\n");
		return -1;
	}

	return 0;
}

static void file_save(char *file, char *buf, int size)
{
	static int s_fd = -1;

    if(s_fd < 0){
        s_fd = open(file, O_RDWR | O_CREAT | O_APPEND, 0655);
    }

    if(s_fd > 0){
        write(s_fd, buf, size);
    }
}

/*****************************  uvc notify  *********************************/

typedef int (*UVC_Notify)(void *data, int size, void *context);

UVC_Notify encode_notify_func = NULL;

static void uvc_register_notify(UVC_Notify notify)
{
	encode_notify_func = notify;
}

static void uvc_unregister_notify(void)
{
	encode_notify_func = NULL;
}

static int brvs_send_data(void *data, int size, unsigned int *id)
{
	int ret;
	
	if(encode_notify_func)
		ret = (*encode_notify_func)(data, size, id);

	return ret;
}

/*****************************  RF channel *********************************/

#define RECV_PORT  3000
#define SEND_PORT  6050
#define SEND_ADDR  "20.0.0.100"

#define BUFF_SIZE  (20 * 1024)

int src_port[6] = {3000, 3000, 3000, 3000, 3000, 3000};
int dst_port[6] = {6050, 6050, 6050, 6050, 6050, 6050};
char *dst_addr[6] = {SEND_ADDR, SEND_ADDR, SEND_ADDR, SEND_ADDR, SEND_ADDR, SEND_ADDR};

//bjs ++
void sonix_on_login_failed(prot_login_errno errno) {
    printf("%s: login failed(%d).\n", __func__, errno);
}

prot_callback sonix_prot_callback = {
    .on_login_failed = sonix_on_login_failed,
    // TODO Implement the rest callback functions.
};
//bjs --

static void *read_ch0_data(void *context)
{
    int size = 0;
	u32 state = 0;
	u32 buf_A_B = 0;

    printf("%s...\n", __func__);
	u8 *recv_buf = malloc(BUFF_SIZE);
    if (!recv_buf) {
        printf("no memory\n");
        return NULL;
    }
	memset(recv_buf, 0, sizeof(recv_buf));

	for(;;) {
		state = reg_read(REG_A_STATUS_CH(CH_0));
		if((state & BUF_READY) && (buf_A_B % 2 == 0)) {
			buf_A_B++;
			size = reg_read(REG_A_LEN_CH(CH_0));
			printf("read ch %d buf_A len %d\n", CH_0, size);
			reg_write(REG_A_STATUS_CH(CH_0), BUF_READY);
		}

		state = reg_read(REG_B_STATUS_CH(CH_0));
		if((state & BUF_READY) && (buf_A_B % 2 == 1)) {
			buf_A_B++;
			size = reg_read(REG_B_LEN_CH(CH_0));
			printf("read ch %d buf_B len %d\n", CH_0, size);
			reg_write(REG_B_STATUS_CH(CH_0), BUF_READY);
		}

		usleep(3000);
	}

	return NULL;
}

static void *send_ch1_data(void *context)
{
	u32 id = 0;
	int head_len, off = 0;
	struct sync_head *header;
	struct vb_packet *RF_packet;
	struct vb_packet_pool *RF_packet_pool;
	struct vb_packet_queue *RF_packet_queue;
	int ret;

	head_len = sizeof(struct sync_head);

	RF_packet_pool = RF_recv.tx_pool;
	RF_packet_queue = &RF_recv.tx_queue;

	printf("%s...\n", __func__);
	for (;;) {
		//bjs++
		if (g_VidInfo.do_quit) {
			printf("Exitting the send_ch1_data thread process... ...\n");
			//goto ERR;
			return &g_VidInfo;
		}
		//bjs--
		
		if (RF_packet_queue->op->get_entry_number(RF_packet_queue) || (RF_packet_queue->curr_consumer)) {
			RF_packet_queue->op->get_curr_consumer(RF_packet_queue);
			if (RF_packet_queue->curr_consumer) {				
				RF_packet = RF_packet_queue->curr_consumer;

				header = (struct sync_head *)RF_packet->data;
				if (header->magic == SYNC_HEAD_MAGIC) {					
					//printf("!!!!!!!!!!!! %s:%d !!!!!!!!!!!!\n", __func__, __LINE__);
					if (off > head_len) {
						ret = brvs_send_data((void *)((u32)rf_buf + head_len), off - head_len, &id);
						if (ret != 0) {
							printf("Unable to send data in callback func (%d).\n", ret);
							//goto ERR;
							//return &g_VidInfo;
						}
					}
					off = 0;
					id  = header->id;
				}

				if ((off + RF_VALID_LEN) < RF_BUF_SIZE) {
					memcpy((void *)((u32)rf_buf + off), RF_packet->data, RF_VALID_LEN);
					off += RF_VALID_LEN;
				} else {
					off = 0;
					printf("spi recv too long\n");
				}

				RF_packet_queue->op->release_curr_consumer(RF_packet_queue, RF_packet_pool);
			}
		}
	}

	return NULL;
}

static void *read_ch1_data(void *context)
{
	u8  type;
	int size = 0;
	int sock = -1;
	u32 state = 0;
	u32 buf_A_B = 0;
	static pthread_t s_pid;
	struct sockaddr_in mcast_addr;
	void *tret = NULL;

	struct vb_packet *RF_packet;
	struct vb_packet_pool *RF_packet_pool;
	struct vb_packet_queue *RF_packet_queue;

	printf("%s. port %d\n", __func__, dst_port[CH_1]);
	u8 *send_buf = malloc(BUFF_SIZE);
	if (!send_buf) {
		printf("no memory\n");
		return NULL;
	}
	memset(send_buf, 0, sizeof(send_buf));

	RF_recv.tx_pool = create_vb_packet_pool(512, 0x400000);
	if (!RF_recv.tx_pool) {
		printf("can't create tx_packet pool\n");
		return NULL;
	}
	init_packet_queue(&RF_recv.tx_queue);

	RF_packet_pool = RF_recv.tx_pool;
	RF_packet_queue = &RF_recv.tx_queue;

	pthread_create(&s_pid, NULL, send_ch1_data, NULL);

	if (sock < 0) {
		sock = socket(AF_INET, SOCK_DGRAM, 0);
		if (sock == -1) {
			printf("socket fd %d\n", sock);
		}
		memset(&mcast_addr, 0, sizeof(mcast_addr));
		mcast_addr.sin_family      = AF_INET;
		mcast_addr.sin_port        = htons(dst_port[CH_1]);
		mcast_addr.sin_addr.s_addr = inet_addr(dst_addr[CH_1]);
	}

	for(;;) {
		state = reg_read(REG_A_STATUS_CH(CH_1));
		if ((state & BUF_READY) && (buf_A_B % 2 == 0)) {
			buf_A_B++;
			size = reg_read(REG_A_LEN_CH(CH_1));

			type = *(u8 *)((u32)data_base + BUF_A_DATA_CH(CH_1) + 15);
			if (type == 0x83) {
				//printf("read ch %d buf_A type 0x%02x len %d\n", CH_1, type, size);

				if (size == 608) {
					RF_packet_pool->op->get_packet_from_pool(RF_packet_pool, &RF_packet, RF_VALID_LEN);
					if (RF_packet) {
						memcpy(RF_packet->data, (void *)((u32)data_base + BUF_A_DATA_CH(CH_1) + 52), RF_VALID_LEN);
						RF_packet->packet_size = RF_VALID_LEN;
						RF_packet_queue->op->put(RF_packet_queue, RF_packet);
					}
				}
			}

			reg_write(REG_A_STATUS_CH(CH_1), BUF_READY);
		}

		state = reg_read(REG_B_STATUS_CH(CH_1));
		if ((state & BUF_READY) && (buf_A_B % 2 == 1)) {
			buf_A_B++;
			size = reg_read(REG_B_LEN_CH(CH_1));

			type = *(u8 *)((u32)data_base + BUF_B_DATA_CH(CH_1) + 15);
			if (type == 0x83) {
				//printf("read ch %d buf_B type 0x%02x len %d\n", CH_1, type, size);

				if (size == 608) {
					RF_packet_pool->op->get_packet_from_pool(RF_packet_pool, &RF_packet, RF_VALID_LEN);
					if (RF_packet) {
						memcpy(RF_packet->data, (void *)((u32)data_base + BUF_B_DATA_CH(CH_1) + 52), RF_VALID_LEN);
						RF_packet->packet_size = RF_VALID_LEN;
						RF_packet_queue->op->put(RF_packet_queue, RF_packet);
					}
				}
			}

			reg_write(REG_B_STATUS_CH(CH_1), BUF_READY);
		}

		usleep(2000);

		//bjs++
		if (g_VidInfo.do_quit) {
			printf("Exitting the read_ch1_data thread ... ...\n");
			goto ERR;
		}
		//bjs--
	}
	
	return NULL;

//bjs++
ERR:
	remove_packet_queue(&RF_recv.tx_queue);
	destroy_vb_packet_pool(RF_recv.tx_pool);

	if (s_pid) {
		pthread_join(s_pid, &tret);
		printf("send_ch1_data thread ret value:%4lu\n", (*(tVidInfo *)tret).retval);
		s_pid = 0;
	}
	
	return &g_VidInfo;
//bjs--	
}

static void *read_ch2_data(void *context)
{
    int size = 0;
	int sock = -1;
	u32 state = 0;
	u32 buf_A_B = 0;
	struct sockaddr_in mcast_addr;

    printf("%s. port %d\n", __func__, dst_port[CH_2]);
	u8 *send_buf = malloc(BUFF_SIZE);
    if (!send_buf) {
        printf("no memory\n");
        return NULL;
    }
	memset(send_buf, 0, sizeof(send_buf));

	if (sock < 0) {
		sock = socket(AF_INET, SOCK_DGRAM, 0);
		if (sock == -1) {
			printf("socket fd %d\n", sock);
		}
		memset(&mcast_addr, 0, sizeof(mcast_addr));
		mcast_addr.sin_family      = AF_INET;
		mcast_addr.sin_port        = htons(dst_port[CH_2]);
		mcast_addr.sin_addr.s_addr = inet_addr(dst_addr[CH_2]);
	}

	for(;;) {
		state = reg_read(REG_A_STATUS_CH(CH_2));
		if((state & BUF_READY) && (buf_A_B % 2 == 0)) {
			buf_A_B++;
			size = reg_read(REG_A_LEN_CH(CH_2));
			memcpy(send_buf, (void *)((u32)data_base + BUF_A_DATA_CH(CH_2)), size);
			printf("read ch %d buf_A len %d\n", CH_2, size);
			reg_write(REG_A_STATUS_CH(CH_2), BUF_READY);

			int ret = sendto(sock, send_buf, size, 0, (const struct sockaddr*)&mcast_addr, sizeof(mcast_addr));
			if (ret < 0) {
				printf("udp send err. ret %d\n", ret);
			}
		}

		state = reg_read(REG_B_STATUS_CH(CH_2));
		if((state & BUF_READY) && (buf_A_B % 2 == 1)) {
			buf_A_B++;
			size = reg_read(REG_B_LEN_CH(CH_2));
			memcpy(send_buf, (void *)((u32)data_base + BUF_B_DATA_CH(CH_2)), size);
			printf("read ch %d buf_B len %d\n", CH_2, size);
			reg_write(REG_B_STATUS_CH(CH_2), BUF_READY);

			int ret = sendto(sock, send_buf, size, 0, (const struct sockaddr*)&mcast_addr, sizeof(mcast_addr));
			if (ret < 0) {
				printf("udp send err. ret %d\n", ret);
			}
		}

		usleep(3000);
	}
	
	return NULL;
}

static void *read_ch3_data(void *context)
{
    int size = 0;
	int sock = -1;
	u32 state = 0;
	u32 buf_A_B = 0;
	struct sockaddr_in mcast_addr;

    printf("%s. port %d\n", __func__, dst_port[CH_3]);
	u8 *send_buf = malloc(BUFF_SIZE);
    if (!send_buf) {
        printf("no memory\n");
        return NULL;
    }
	memset(send_buf, 0, sizeof(send_buf));

	if (sock < 0) {
		sock = socket(AF_INET, SOCK_DGRAM, 0);
		if (sock == -1) {
			printf("socket fd %d\n", sock);
		}
		memset(&mcast_addr, 0, sizeof(mcast_addr));
		mcast_addr.sin_family      = AF_INET;
		mcast_addr.sin_port        = htons(dst_port[CH_3]);
		mcast_addr.sin_addr.s_addr = inet_addr(dst_addr[CH_3]);
	}

	for(;;) {
		state = reg_read(REG_A_STATUS_CH(CH_3));
		if((state & BUF_READY) && (buf_A_B % 2 == 0)) {
			buf_A_B++;
			size = reg_read(REG_A_LEN_CH(CH_3));
			memcpy(send_buf, (void *)((u32)data_base + BUF_A_DATA_CH(CH_3)), size);
			printf("read ch %d buf_A len %d\n", CH_3, size);
			reg_write(REG_A_STATUS_CH(CH_3), BUF_READY);

			int ret = sendto(sock, send_buf, size, 0, (const struct sockaddr*)&mcast_addr, sizeof(mcast_addr));
			if (ret < 0) {
				printf("udp send err. ret %d\n", ret);
			}
		}

		state = reg_read(REG_B_STATUS_CH(CH_3));
		if((state & BUF_READY) && (buf_A_B % 2 == 1)) {
			buf_A_B++;
			size = reg_read(REG_B_LEN_CH(CH_3));
			memcpy(send_buf, (void *)((u32)data_base + BUF_B_DATA_CH(CH_3)), size);
			printf("read ch %d buf_B len %d\n", CH_3, size);
			reg_write(REG_B_STATUS_CH(CH_3), BUF_READY);

			int ret = sendto(sock, send_buf, size, 0, (const struct sockaddr*)&mcast_addr, sizeof(mcast_addr));
			if (ret < 0) {
				printf("udp send err. ret %d\n", ret);
			}
		}

		usleep(3000);
	}

	return NULL;
}

int ch5_sock = -1;
struct sockaddr_in ch5_addr;

static void *read_ch4_data(void *context)
{
	int size = 0;
	u32 state = 0;
	u32 buf_A_B = 0;
	struct timeval tv;

	printf("%s...\n", __func__);
	u8 *send_buf = malloc(BUFF_SIZE);
	if (!send_buf) {
		printf("no memory\n");
		return NULL;
	}
	memset(send_buf, 0, sizeof(send_buf));

	for(;;) {
		state = reg_read(REG_A_STATUS_CH(CH_4));
		if((state & BUF_READY) && (buf_A_B % 2 == 0)) {
			buf_A_B++;
			gettimeofday(&tv, NULL);
			size = reg_read(REG_A_LEN_CH(CH_4));
			memcpy(send_buf, (void *)((u32)data_base + BUF_A_DATA_CH(CH_4)), size);
			printf("%ld.%03ld: read ch %d buf_A len %d\n", tv.tv_sec, tv.tv_usec / 1000, CH_4, size);
			reg_write(REG_A_STATUS_CH(CH_4), BUF_READY);

			int ret = sendto(ch5_sock, send_buf, size, 0, (const struct sockaddr*)&ch5_addr, sizeof(ch5_addr));
			if (ret < 0) {
				printf("udp send err. ret %d\n", ret);
			}
		}

		state = reg_read(REG_B_STATUS_CH(CH_4));
		if((state & BUF_READY) && (buf_A_B % 2 == 1)) {
			buf_A_B++;
			gettimeofday(&tv, NULL);
			size = reg_read(REG_B_LEN_CH(CH_4));
			memcpy(send_buf, (void *)((u32)data_base + BUF_B_DATA_CH(CH_4)), size);
			printf("%ld.%03ld: read ch %d buf_B len %d\n", tv.tv_sec, tv.tv_usec / 1000, CH_4, size);
			reg_write(REG_B_STATUS_CH(CH_4), BUF_READY);

			int ret = sendto(ch5_sock, send_buf, size, 0, (const struct sockaddr*)&ch5_addr, sizeof(ch5_addr));
			if (ret < 0) {
				printf("udp send err. ret %d\n", ret);
			}
		}

		usleep(3000);
	}
	
	return NULL;
}

static void swap_data_32bit(u8 *buf, int size)
{
	int i;
	u32 temp, *data;

	for(i = 0, data = (u32 *)buf; i < size / 4; i++, data++){
		temp = 0;
		temp |= (*data & 0xff000000) >> 24;
		temp |= (*data & 0x00ff0000) >> 8;
		temp |= (*data & 0x0000ff00) << 8;
		temp |= (*data & 0x000000ff) << 24;
		*data = temp;
	}
}

static void *recv_ch5_data(void *context)
{
    int size = 0;
    int err = -1;
	u32 state = 0;
	u32 buf_A_B = 0;
    socklen_t addr_len = 0;
    struct timeval tv;

    printf("%s. port %d\n", __func__, RECV_PORT);
	u8 *recv_buf = malloc(BUFF_SIZE);
    if (!recv_buf) {
        printf("no memory\n");
        return NULL;
    }
	memset(recv_buf, 0, sizeof(recv_buf));

    ch5_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (ch5_sock == -1) {
		printf("socket fd %d\n", ch5_sock);
        return NULL;
    }

    ch5_addr.sin_family      = AF_INET;
    ch5_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    ch5_addr.sin_port        = htons(RECV_PORT);

    err = bind(ch5_sock, (struct sockaddr*)&ch5_addr, sizeof(ch5_addr));
    if (err < 0) {
    	printf("bind error\n");
        return NULL;
    }

	addr_len = sizeof(ch5_addr);

	for(;;){
		size = recvfrom(ch5_sock, recv_buf, BUFF_SIZE, 0, (struct sockaddr*)&ch5_addr, &addr_len);
		if (size == -1) {
			printf("set ch5_sock error\n");
			continue;
		} else {
			printf("recv udp data %d bytes\n", size);
		}
		swap_data_32bit(recv_buf, size);

		if (buf_A_B++ % 2 == 0) {
			do {
				state = reg_read(REG_A_STATUS_CH(CH_5));
			} while(state & BUF_READY);

			reg_write(REG_A_LEN_CH(CH_5), size);
			memcpy(((void *)(u32)data_base + BUF_A_DATA_CH(CH_5)), recv_buf, size);
			gettimeofday(&tv, NULL);
			printf("%ld.%03ld: write ch %d buf_A size %d\n", tv.tv_sec, tv.tv_usec / 1000, CH_5, size);
			reg_write(REG_A_STATUS_CH(CH_5), BUF_READY);
		} else {
			do {
				state = reg_read(REG_B_STATUS_CH(CH_5));
			} while(state & BUF_READY);

			reg_write(REG_B_LEN_CH(CH_5), size);
			memcpy(((void *)(u32)data_base + BUF_B_DATA_CH(CH_5)), recv_buf, size);
			gettimeofday(&tv, NULL);
			printf("%ld.%03ld: write ch %d buf_B size %d\n", tv.tv_sec, tv.tv_usec / 1000, CH_5, size);
			reg_write(REG_B_STATUS_CH(CH_5), BUF_READY);
		}

		usleep(3000);
	}

    close(ch5_sock);
	
	return NULL;
}

//bjs++
#define BV_TEMP 1
static int uvc_send_data(void *data, int size, void *context)
{
#if BV_TEMP
#define MCAST_PORT 7777
#define MCAST_ADDR "224.0.0.88" 
#define UDP_MUT_SIZE 1472
#endif

	if (BV_TEMP) {		//temp
		//char MCAST_ADDR[16] = {0};
		//strncpy(MCAST_ADDR,client_info.ip,sizeof(MCAST_ADDR));
		//int MCAST_PORT = client_info.port+1;
		static int sock = -1;
		static struct sockaddr_in mcast_addr;
		
		if (sock < 0) {
			sock=socket(AF_INET,SOCK_DGRAM,0);
			if (sock==-1) {                 
				perror ("socket");
			}
			memset(&mcast_addr,0,sizeof(mcast_addr));
			mcast_addr.sin_family=AF_INET;
			mcast_addr.sin_addr.s_addr=inet_addr(MCAST_ADDR);
			mcast_addr.sin_port=htons(MCAST_PORT);

			fcntl(sock, F_SETFL, fcntl(sock, F_GETFL) | O_NONBLOCK);
		}
		
		char *data_payload = data;
		int payload_size = size;
		
		while (payload_size > 0) {
			int len = (payload_size > UDP_MUT_SIZE)?(UDP_MUT_SIZE):(payload_size);
			int ret =  sendto(sock,data_payload,len,0,(const struct sockaddr*)&mcast_addr, sizeof(mcast_addr));
			data_payload +=len;
			payload_size -= len;
		}
	}

#if 0
 	//保存数据流到文件
	file_save("recv.264", data, size);
	printf("data id-%d bytes-%d\n", *(unsigned int *)context, size);
#else
	int ret;
	unsigned int framerate = 30;
	int pixelformat = PIX_FMT_H264;
	//int freeram = 0;

	//double fps;

	//unsigned int i;
	unsigned int delay = 0;
	unsigned int width = 1920;//1280;
	unsigned int height = 1080;//720;
	//unsigned int nframes = (unsigned int)-1;
	//unsigned int nbufs = V4L_BUFFERS_DEFAULT;

	//void *mem0[V4L_BUFFERS_MAX] = {0};
	//char *dev_name = "/dev/video1";
	unsigned long device_id = 0;
	char server_ip[20] = {0};
	short server_port = 0;
	double m_BitRate = 4000000.0;//2000000.0;	
	char do_xu_get_br = 0;
	//char do_xu_set_br = 1;

	
	//do_xu_get_br = g_VidInfo.do_xu_get_br;
	//do_xu_set_br = g_VidInfo.do_xu_set_br;
	g_VidInfo.do_quit = 0;
	g_VidInfo.do_upload = 1;
	g_VidInfo.do_capture = 1;
	//delay = g_VidInfo.delay;
	//dev_name = g_VidInfo.dev_name;
	//framerate = g_VidInfo.framerate;
	//width = g_VidInfo.width;
	//height = g_VidInfo.height;
	//g_VidInfo.nbufs = nbufs;
	//pixelformat = g_VidInfo.pixelformat;
	struct timeval ts;
	struct h264_frame_info frame_info;

	if (!g_VidInfo.do_capture) {
		goto ERR;
	}
	
	if (do_xu_get_br) {
		/* To do: call sdk(brvs) to get bitrate from frontend camera. */
		//m_BitRate = brvs_sdk();
		g_VidInfo.bitrate = m_BitRate;
	}

	device_id = 1111199999;
	strcpy(server_ip, "172.20.30.113");
	server_port = 8888;

	if (!g_VidInfo.prot_online) {
		if (g_VidInfo.do_upload == 1 && pixelformat == PIX_FMT_H264) {
			prot_device_info device;
			device.id = device_id;
			prot_set_device_info(&device);

			prot_server_info server;
			memcpy(server.ip, server_ip, sizeof(server.ip));
			server.port = server_port;
			prot_set_server_info(&server);
		
			prot_set_callback(&sonix_prot_callback);	//camera controls command callback.
			
			ret = prot_init();
			if (ret != SUCCESS) {
				printf("protocol init failed(%d).\n", ret);
				goto ERR;
			}
			
			ret = prot_login();
			if (ret != SUCCESS) {
				printf("protocol login failed(%d).\n", ret);
				goto ERR;
			}
		
			prot_video_info video;
			video.width = width;
			video.height = height;
			video.frame_rate = framerate;
			video.bitrate = m_BitRate;
			video.encode_type = PROT_ENCODE_TYPE_H264;
			prot_update_video_info(&video);
		
			g_VidInfo.prot_online = 1;
		} else {
			g_VidInfo.do_upload = 0;
		}
	}

	if (g_VidInfo.do_upload) {
		//gettimeofday(&start, NULL);
		gettimeofday(&ts, NULL);
		printf("Frame[%4u] %u bytes %ld.%06ld\n ", *(unsigned int *)context, size, ts.tv_sec, ts.tv_usec);
		
		if (h264_frame_parse(data, 0, size, &frame_info)) {
			if (frame_info.is_i_frame) {
				//TestAp_Printf(TESTAP_DBG_FRAME, "##### I frame.\n");
				ret = prot_video_frame_send(data + frame_info.header_offset, \
										frame_info.header_length + frame_info.data_length, \
										PROT_FRAME_I, \
										ts.tv_sec * 1000 + ts.tv_usec / 1000);
				if (ret != SUCCESS) {
					printf("prot_video_frame_send failed:(%d)\n", ret);
					return -1;
				}
				//TestAp_Printf(TESTAP_DBG_FRAME, "##### Send I frame, ts: %ld.%06ld #######\n", ts.tv_sec, ts.tv_usec);
			} else {
				ret = prot_video_frame_send(data + frame_info.data_offset, \
										frame_info.data_length, \
										PROT_FRAME_P, \
										ts.tv_sec * 1000 + ts.tv_usec / 1000);
				if (ret != SUCCESS) {				
					printf("prot_video_frame_send failed:(%d)\n", ret);
					return -1;
				}
				//TestAp_Printf(TESTAP_DBG_FRAME, "###### Send B/P frame, ts: %ld.%06ld ######\n", ts.tv_sec, ts.tv_usec);
			}
		} else {
			printf("Parse H264 frame failed.\n");
		}
	}
	
	if (delay > 0) {
		usleep(delay * 1000);
	}
	
	fflush(stdout);

#endif

ERR:
	return ret;
}
//bjs--

int main(int argc, char *argv[])
{
	char ch = 0;
	int ret = 0;
	//u32 len = 0;
	//u32 state = 0;
	static pthread_t s_pid[6] = {0};
	void *tret = NULL;
	int i;

	ret = mmap_init();
	if (ret) {
		printf("\nmmap_init err.\n");
	} else {
		printf("\nmmap succeed. reg_base = 0x%08x\n\n", reg_base);
	}

	uvc_register_notify(uvc_send_data);

	reg_write(REG_RF_CONTROL, RF_RESET);
	reg_write(REG_RF_CONTROL, RF_ON);
	usleep(1000);

	if(argc == 13){
		src_port[CH_1] = atoi(argv[1]);	src_port[CH_2] = atoi(argv[2]);
		src_port[CH_3] = atoi(argv[3]);	src_port[CH_4] = atoi(argv[4]);

		dst_port[CH_1] = atoi(argv[5]);	dst_port[CH_2] = atoi(argv[6]);
		dst_port[CH_3] = atoi(argv[7]);	dst_port[CH_4] = atoi(argv[8]);

		dst_addr[CH_1] = argv[9];	dst_addr[CH_2] = argv[10];
		dst_addr[CH_3] = argv[11];	dst_addr[CH_4] = argv[12];
	} else {
		printf("use default val.\n");
	}

	//pthread_create(&s_pid[0], NULL, read_ch0_data, NULL);
	ret = pthread_create(&s_pid[1], NULL, read_ch1_data, NULL);
	if(ret != 0) {
		printf("Create read_ch1_data pthread error!\n");
		s_pid[1] = 0;
		ret = 1;
		goto ERR;
	}

	//pthread_create(&s_pid[2], NULL, read_ch2_data, NULL);
	//pthread_create(&s_pid[3], NULL, read_ch3_data, NULL);
	//pthread_create(&s_pid[4], NULL, read_ch4_data, NULL);
	//pthread_create(&s_pid[5], NULL, recv_ch5_data, NULL);

	for(;;){
		usleep(60000);
		
		ch = getchar();
		fflush(stdin);

		if ('q' == ch) {
			g_VidInfo.do_quit = 1;
			goto ERR;
		}
	}

	return ret;

ERR:
	for (i = 0; i < 6; i++) {
		if (s_pid[i] != 0) {
			pthread_join(s_pid[i], &tret);
			printf("read_ch1_data Thread ret value:%4lu\n", (*(tVidInfo *)tret).retval);
			s_pid[1] = 0;
		}
	}	

	return ret;
}
