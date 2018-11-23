#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <getopt.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <linux/videodev2.h>

#include "nalu.h"
#include "protocol.h"
#include "protocol_errno.h"
#include "sonix_xu_ctrls.h"


#define V4L_BUFFERS_DEFAULT	 6
#define V4L_BUFFERS_MAX		 16

struct H264Format *gH264fmt = NULL;

int Dbg_Param = 0x1f;
#define TestAp_Printf(flag, msg...) ((Dbg_Param & flag)?printf(msg):flag)

#define TESTAP_DBG_USAGE	(1 << 0)
#define TESTAP_DBG_ERR		(1 << 1)
#define TESTAP_DBG_FLOW		(1 << 2)
#define TESTAP_DBG_FRAME	(1 << 3)
#define TESTAP_DBG_BW	    (1 << 4)

//bjs ++
#undef V4L2_PIX_FMT_H264
#define V4L2_PIX_FMT_H264 v4l2_fourcc('H','2','6','4') /* H264 */
#undef V4L2_PIX_FMT_MP2T
#define V4L2_PIX_FMT_MP2T v4l2_fourcc('M','P','2','T') /* MPEG-2 TS */
//bjs --

typedef int (*UVC_Notify)(void *data, int size, void *context);

UVC_Notify encode_notify_func = NULL;

static void UVC_RegisterNotify(UVC_Notify notify)
{
	encode_notify_func = notify;
}

static void UVC_UnRegisterNotify(void)
{
	encode_notify_func = NULL;
}

static int brvs_send_data(void *data, int size, unsigned int *id)
{
	int ret = 0;
	
	if(encode_notify_func)
		(*encode_notify_func)(data, size, id);

	return ret;
}

static int video_open(const char *devname)
{
    struct v4l2_capability cap;
    int dev, ret;

    dev = open(devname, O_RDWR);
    if (dev < 0) {
        TestAp_Printf(TESTAP_DBG_ERR, "Error opening device %s: %d.\n", devname, errno);
        return dev;
    }

    memset(&cap, 0, sizeof(cap));
    ret = ioctl(dev, VIDIOC_QUERYCAP, &cap);
    if (ret < 0) {
        TestAp_Printf(TESTAP_DBG_ERR, "Error opening device %s: unable to query device.\n",
            devname);
        close(dev);
        return ret;
    }

    if ((cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) == 0) {
        TestAp_Printf(TESTAP_DBG_ERR, "Error opening device %s: video capture not supported.\n",
            devname);
        close(dev);
        return -EINVAL;
    }

    printf( "Device %s opened: %s.\n", devname, cap.card);
    return dev;
}

static int video_set_format(int dev, unsigned int w, unsigned int h, unsigned int format)
{
    struct v4l2_format fmt;
    int ret;

    memset(&fmt, 0, sizeof(fmt));
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = w;
    fmt.fmt.pix.height = h;
    fmt.fmt.pix.pixelformat = format;
    fmt.fmt.pix.field = V4L2_FIELD_ANY;

    ret = ioctl(dev, VIDIOC_S_FMT, &fmt);
    if (ret < 0) {
        TestAp_Printf(TESTAP_DBG_ERR, "Unable to set format: %d.\n", errno);
        return ret;
    }

    TestAp_Printf(TESTAP_DBG_FLOW, "Video format set: width: %u height: %u buffer size: %u\n",
        fmt.fmt.pix.width, fmt.fmt.pix.height, fmt.fmt.pix.sizeimage);
    return 0;
}

static int video_set_framerate(int dev, int framerate, unsigned int *MaxPayloadTransferSize)
{
    struct v4l2_streamparm parm;
    int ret;

    memset(&parm, 0, sizeof(parm));
    parm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    ret = ioctl(dev, VIDIOC_G_PARM, &parm);
    if (ret < 0) {
        TestAp_Printf(TESTAP_DBG_ERR, "Unable to get frame rate: %d.\n", errno);
        return ret;
    }

    TestAp_Printf(TESTAP_DBG_FLOW, "Current frame rate: %u/%u\n",
        parm.parm.capture.timeperframe.numerator,
        parm.parm.capture.timeperframe.denominator);

    parm.parm.capture.timeperframe.numerator = 1;
    parm.parm.capture.timeperframe.denominator = framerate;

    ret = ioctl(dev, VIDIOC_S_PARM, &parm);
    if (ret < 0) {
        TestAp_Printf(TESTAP_DBG_ERR, "Unable to set frame rate: %d.\n", errno);
        return ret;
    }

    if(MaxPayloadTransferSize)
        *MaxPayloadTransferSize = parm.parm.capture.reserved[0];

    ret = ioctl(dev, VIDIOC_G_PARM, &parm);
    if (ret < 0) {
        TestAp_Printf(TESTAP_DBG_ERR, "Unable to get frame rate: %d.\n", errno);
        return ret;
    }

    TestAp_Printf(TESTAP_DBG_FLOW, "Frame rate set: %u/%u\n",
        parm.parm.capture.timeperframe.numerator,
        parm.parm.capture.timeperframe.denominator);
    return 0;
}

int video_get_framerate(int dev, int *framerate)
{
	struct v4l2_streamparm parm;
	int ret;

	memset(&parm, 0, sizeof parm);
	parm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	ret = ioctl(dev, VIDIOC_G_PARM, &parm);
	if (ret < 0) {
		TestAp_Printf(TESTAP_DBG_ERR, "Unable to get frame rate(%d): %s.\n", errno, strerror(errno));
		return ret;
	}

	TestAp_Printf(TESTAP_DBG_FLOW, "Current frame rate: %u/%u\n",
		parm.parm.capture.timeperframe.numerator,
		parm.parm.capture.timeperframe.denominator);
    *framerate = parm.parm.capture.timeperframe.denominator;
    
	return 0;
}

static int video_reqbufs(int dev, int nbufs)
{
	struct v4l2_requestbuffers rb;
	int ret;

	memset(&rb, 0, sizeof(rb));
	rb.count = nbufs;
	rb.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	rb.memory = V4L2_MEMORY_MMAP;

	ret = ioctl(dev, VIDIOC_REQBUFS, &rb);
	if (ret < 0) {
		TestAp_Printf(TESTAP_DBG_ERR, "Unable to allocate buffers: %d.\n", errno);
		return ret;
	}

	TestAp_Printf(TESTAP_DBG_FLOW, "%u buffers allocated.\n", rb.count);
	return rb.count;
}

static int video_enable(int dev, int enable)
{
	int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	int ret;

	ret = ioctl(dev, enable ? VIDIOC_STREAMON : VIDIOC_STREAMOFF, &type);
	if (ret < 0) {
		TestAp_Printf(TESTAP_DBG_ERR, "Unable to %s capture: %d.\n", enable ? "start" : "stop", errno);
		return ret;
	}

	return 0;
}

#if 0		//add by BJS
static int video_close(int fd)
{

}
#endif

static int GetFreeRam(int *freeram)
{
	FILE *meminfo = fopen("/proc/meminfo", "r");
	char line[256];
	if (meminfo == NULL) {
		TestAp_Printf(TESTAP_DBG_ERR, "/proc/meminfo can't open\n");
		return 0;
	}
	
	while (fgets(line, sizeof(line), meminfo)) {
		if (sscanf(line, "MemFree: %d kB", freeram) == 1) {
			*freeram <<= 10;
			fclose(meminfo);
			return 1;
		}
	}

	fclose(meminfo);
	return 0;
}

// yview protocol ++
void sonix_on_login_failed(prot_login_errno errno) {
    TestAp_Printf(TESTAP_DBG_ERR, "%s: login failed(%d).\n", __func__, errno);
}

prot_callback sonix_prot_callback = {
    .on_login_failed = sonix_on_login_failed,
    // TODO Implement the rest callback functions.
};
// yview protocol --

static void *video_capture_pthread(void *context)
{
	int dev, ret;
	int do_record = 1;
	int framerate = 30;
	int pixelformat = V4L2_PIX_FMT_H264;
	int freeram = 0;

	double fps;

	unsigned int i;
	unsigned int delay = 0;
	unsigned int width = 1920;//1280;
	unsigned int height = 1080;//720;
	unsigned int nframes = (unsigned int)-1;
	unsigned int nbufs = V4L_BUFFERS_DEFAULT;

	void *mem0[V4L_BUFFERS_MAX] = {0};

	char *dev_name = "/dev/video1";

//bjs++
	/* Options parsings */
	//char do_save = 0;
	char do_capture = 1;
	//char do_list_controls = 0, do_set_input = 0;
	//char *endptr;
	//int c;
	
	unsigned char do_upload = 1;
	unsigned long device_id = 0;
	char server_ip[20] = {0};
	short server_port = 0;
	double m_BitRate = 0;
//bjs--

	struct v4l2_buffer buf0;
	struct timeval start, end, ts;

	/* Open the video device. */
	dev = video_open(dev_name);
	if (dev < 0)
		return NULL;
	
	if (!do_capture) {
		goto ERR;
	}

	/* Set the video format. */
	if (video_set_format(dev, width, height, pixelformat) < 0) {
		goto ERR;
	}

	/* Set the frame rate. */
	if (video_set_framerate(dev, framerate, NULL) < 0) {
		goto ERR;
	}

//bjs ++
	char do_xu_get_br = 0;
	char do_xu_set_br = 1;
	
	m_BitRate =2000000.0;

	/* Set the bit rate of UVC device. */
	ret = XU_Ctrl_ReadChipID(dev);
	if (ret < 0) {
		TestAp_Printf(TESTAP_DBG_ERR, "SONiX_UVC_TestAP @main : XU_Ctrl_ReadChipID Failed\n");
		goto ERR;
	}

	if (do_xu_get_br) {
		XU_H264_Get_BitRate(dev, &m_BitRate);
		if (m_BitRate < 0 ) {
			TestAp_Printf(TESTAP_DBG_ERR, "SONiX_UVC_TestAP @main : XU_H264_Get_BitRate Failed\n");
			goto ERR;
		}
		TestAp_Printf(TESTAP_DBG_FLOW, "Current bit rate: %.2f Kbps\n",m_BitRate);
	}

	if (do_xu_set_br) {
		if (XU_H264_Set_BitRate(dev, m_BitRate) < 0 ) {
			TestAp_Printf(TESTAP_DBG_ERR, "SONiX_UVC_TestAP @main : XU_H264_Set_BitRate Failed\n");
			goto ERR;
		}
	}


	if (GetFreeRam(&freeram) && freeram < (1843200 * nbufs + 4194304)) {
		TestAp_Printf(TESTAP_DBG_ERR, "free memory isn't enough(%d)\n", freeram);
		ret = 1;
		goto ERR;
	}
//bjs --

	/* Allocate buffers. */
	if ((int)(nbufs = video_reqbufs(dev, nbufs)) < 0) {
		goto ERR;

	}

	/* Map the buffers. */
	for (i = 0; i < nbufs; i++) {
		memset(&buf0, 0, sizeof(buf0));
		buf0.index = i;
		buf0.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf0.memory = V4L2_MEMORY_MMAP;
		ret = ioctl(dev, VIDIOC_QUERYBUF, &buf0);
		if (ret < 0) {
			TestAp_Printf(TESTAP_DBG_ERR, "Unable to query buffer %u (%d).\n", i, errno);
			goto ERR;
		}
		TestAp_Printf(TESTAP_DBG_FLOW, "length: %u offset: %10u     --  ", buf0.length, buf0.m.offset);

		mem0[i] = mmap(0, buf0.length, PROT_READ, MAP_SHARED, dev, buf0.m.offset);
		if (mem0[i] == MAP_FAILED) {
			TestAp_Printf(TESTAP_DBG_ERR, "Unable to map buffer %u (%d)\n", i, errno);
			goto ERR;
		}
		TestAp_Printf(TESTAP_DBG_FLOW, "Buffer %u mapped at address %p.\n", i, mem0[i]);
	}

	/* Queue the buffers. */
	for (i = 0; i < nbufs; i++) {
		memset(&buf0, 0, sizeof(buf0));
		buf0.index = i;
		buf0.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf0.memory = V4L2_MEMORY_MMAP;
		ret = ioctl(dev, VIDIOC_QBUF, &buf0);
		if (ret < 0) {
			TestAp_Printf(TESTAP_DBG_ERR, "Unable to queue buffer0(%d).\n", errno);
			goto ERR;
		}
	}

	/* Start streaming. */
	ret = video_enable(dev, 1);
	if (0 != ret) {		
		TestAp_Printf(TESTAP_DBG_ERR, "Unable to start  streaming: (%d).\n", errno);
		goto ERR;
	}
	
//bjs ++
	framerate = 30;
	//int bit_num = 0, tmp = 0;
	//int m_QP_Val = 0;

	//do_capture = 1;
	//m_BitRate = atoi(optarg);
	//m_BitRate =2000000;
	
	//do_upload = 1;
	device_id = 1111199999;
	//memcpy(server_ip, "172.20.30.113", sizeof(server_ip));
	strcpy(server_ip, "172.20.30.113");
	server_port = 8888;

	// yview protocol ++
	if (do_upload == 1 && pixelformat == V4L2_PIX_FMT_H264) {
		prot_device_info device;
		device.id = device_id;
		prot_set_device_info(&device);

		prot_server_info server;
		memcpy(server.ip, server_ip, sizeof(server.ip));
		server.port = server_port;
		prot_set_server_info(&server);

		//prot_set_callback(&sonix_prot_callback);	//camera controls command callback.
		
		ret = prot_init();		
		if (ret != SUCCESS) {
			TestAp_Printf(TESTAP_DBG_ERR, "protocol init failed(%d).\n", ret);
			goto ERR;
		}
		
		ret = prot_login();
		if (ret != SUCCESS) {
			TestAp_Printf(TESTAP_DBG_ERR, "protocol login failed(%d).\n", ret);
			goto ERR;
		}

		prot_video_info video;
		video.width = width;
		video.height = height;
		video.frame_rate = framerate;
		video.bitrate = m_BitRate;
		video.encode_type = PROT_ENCODE_TYPE_H264;
		prot_update_video_info(&video);
	} else {
		do_upload = 0;
	}
	// yview protocol --
//bjs --

	gettimeofday(&start, NULL);

	for (i = 0; i >= 0; i++) {
		/* Dequeue a buffer. */
		memset(&buf0, 0, sizeof(buf0));
		buf0.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf0.memory = V4L2_MEMORY_MMAP;
		ret = ioctl(dev, VIDIOC_DQBUF, &buf0);
		if (ret < 0) {
			TestAp_Printf(TESTAP_DBG_ERR, "Unable to dequeue buffer0 (%d).\n", errno);
			goto ERR;
		}

		gettimeofday(&ts, NULL);
		TestAp_Printf(TESTAP_DBG_FRAME, "Frame[%4u] %u bytes %ld.%06ld %ld.%06ld\n ", \
					i, buf0.bytesused, buf0.timestamp.tv_sec, buf0.timestamp.tv_usec, ts.tv_sec, ts.tv_usec);
		//if (i == 0) start = ts;

		if (do_upload) {
			ret = brvs_send_data(mem0[buf0.index], buf0.bytesused, &i);
			if (ret != 0) {
				TestAp_Printf(TESTAP_DBG_ERR, "Unable to send data in callback func (%d).\n", errno);
				goto ERR;
			}
		}
		//TestAp_Printf(TESTAP_DBG_FRAME, "!!!!!!!!Frame[%4u] callback send data success!!!!!!!\n ", i);


		/* Requeue the buffer. */
		if (delay > 0) {
			usleep(delay * 1000);
		}

		ret = ioctl(dev, VIDIOC_QBUF, &buf0);
		if (ret < 0) {
			TestAp_Printf(TESTAP_DBG_ERR, "Unable to requeue buffer0 (%d).\n", errno);
			goto ERR;
		}
		//TestAp_Printf(TESTAP_DBG_FRAME, "@@@@@@@Frame[%4u] V4L2 put buffer success@@@@@@@\n ", i);

		fflush(stdout);
	}
	
	gettimeofday(&end, NULL);
	end.tv_sec -= start.tv_sec;
	end.tv_usec -= start.tv_usec;

	if (end.tv_usec < 0) {
		end.tv_sec--;
		end.tv_usec += 1000000;
	}
	fps = (i - 1) / (end.tv_usec + 1000000.0 * end.tv_sec) * 1000000.0;
	printf("Captured %u frames in %lu.%06lu seconds (%f fps).\n", i - 1, end.tv_sec, end.tv_usec, fps);

	//return ret = 0;

ERR:
	if (do_upload) {
		prot_logout();
		prot_fini();
	}
	
	if (gH264fmt) {
		free(gH264fmt);
		gH264fmt = NULL;
	}

	close(dev);
	return NULL;
}

static int uvc_send_data(void *data, int size, void *context)
{
	int ret = 0;

#if 0
	static FILE *rec_fp1 = NULL;
	char *rec_filename = "uvc_cam.h264";

	/* Record the H264 video file */
	if(rec_fp1 == NULL)
		rec_fp1 = fopen(rec_filename, "a+b");

    if(rec_fp1 != NULL)
        fwrite(data, size, 1, rec_fp1);

#else	

	//TestAp_Printf(TESTAP_DBG_FRAME, "save frame_id %d data %d bytes.\n", *(unsigned int *)context, size);
	/*
		to do ...
	*/

	struct timeval ts;
	struct h264_frame_info frame_info;
	
	//TestAp_Printf(TESTAP_DBG_FRAME, "$$$$$$$$running in callback$$$$$$$$$.\n");

	gettimeofday(&ts, NULL);

	if (h264_frame_parse(data, 0, size, &frame_info)) {
		if (frame_info.is_i_frame) {
			TestAp_Printf(TESTAP_DBG_FRAME, "##### I frame, ts: %ld.%06ld #######\n", ts.tv_sec, ts.tv_usec);
			//TestAp_Printf(TESTAP_DBG_FRAME, "##### I frame.\n");
			ret = prot_video_frame_send(data + frame_info.header_offset, \
									frame_info.header_length + frame_info.data_length, \
									PROT_FRAME_I, \
									ts.tv_sec * 1000 + ts.tv_usec / 1000);
			if (ret != SUCCESS) {
				TestAp_Printf(TESTAP_DBG_ERR, "prot_video_frame_send failed:(%d)\n", ret);
				return -1;
			}
		} else {
			//TestAp_Printf(TESTAP_DBG_FRAME, "###### B/P frame, ts: %ld.%06ld ######\n", ts.tv_sec, ts.tv_usec);
			ret = prot_video_frame_send(data + frame_info.data_offset, \
									frame_info.data_length, \
									PROT_FRAME_P, \
									ts.tv_sec * 1000 + ts.tv_usec / 1000);
			if (ret != SUCCESS) {				
				TestAp_Printf(TESTAP_DBG_ERR, "prot_video_frame_send failed:(%d)\n", ret);
				return -1;
			}
		}
	} else {
		TestAp_Printf(TESTAP_DBG_ERR, "Parse H264 frame failed.\n");
	}
#endif
	return ret;

}

int main(int argc, char *argv[])
{
	static pthread_t s_pid;
	int ret;

	UVC_RegisterNotify(uvc_send_data);
	ret = pthread_create(&s_pid, NULL, video_capture_pthread, NULL);
	if(ret != 0) {
		TestAp_Printf(TESTAP_DBG_ERR, "Create video capture pthread error!\n");
		s_pid = 0;
		ret = 1;
		goto ERR;
	}
	
	while(1){
		//usleep(10000);
		sleep(1);
	}

ERR:
	if (s_pid != 0) {
		pthread_join(s_pid, NULL);
		s_pid = 0;
	}

	return ret;
}
