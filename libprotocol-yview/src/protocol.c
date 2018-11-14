/*************************************************************************
	> File Name: protocol.c
	> Author: 
	> Mail: 
	> Created Time: Wed 26 Sep 2018 11:28:50 AM CST
 ************************************************************************/

#define DEBUG

#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h>

#include "yvcommon.h"
#include "yvcommon_def.h"
#include "yvcommon_srv_dev_msg_struct.h"
#include "yvcommon_data_struct.h"
#include "yvcommon_srv_dev_msg_type.h"

#include "udpx.h"
#include "udpx_buffer.h"

#include "protocol_log.h"
#include "protocol_errno.h"
#include "protocol.h"

/**
 * ��/��Ƶ��ID.
 * ��֧�ֶ���.
 */
#define PROT_AUDIO_STREAM_ID (0)
#define PROT_VIDEO_STREAM_ID (30)

/**
 * WTF.
 * �ͻ���д����ֵ.
 */
// ����.
#define PROT_CLI_BRIGHTNESS_MIN (0)
#define PROT_CLI_BRIGHTNESS_MAX (200)
// �Աȶ�.
#define PROT_CLI_CONTRAST_MIN (0)
#define PROT_CLI_CONTRAST_MAX (50)
// ���Ͷ�.
#define PROT_CLI_SATURATION_MIN (0)
#define PROT_CLI_SATURATION_MAX (200)
// ɫ��.
#define PROT_CLI_HUE_MIN (0)
#define PROT_CLI_HUE_MAX (50)
// ���.
#define PROT_CLI_SHARPNESS_MIN (0)
#define PROT_CLI_SHARPNESS_MAX (50)
// �ع��.
#define PROT_CLI_EXPOSURE_MIN (0)
#define PROT_CLI_EXPOSURE_MAX (10)


/**
 * �߳�.
 */
void *prot_msg_recv_thread(void *param);
void *prot_monitor_thread(void *param);

/**
 * ȫ�ֱ���.
 */
static prot_info prot_instance;

/**************************************************
 * ����.
 **************************************************/

/**
 * @brief ҵ��ͷͨ�ó�ʼ��
 *
 * @param pstHdr
 */
void InterCommHdr_init(yv_InterCommHdr *pstHdr) {
    if (pstHdr != NULL) {
        memset(pstHdr, 0, sizeof(yv_InterCommHdr));

        pstHdr->Version = 0;
        pstHdr->HdrLen = 0;
        memset(&pstHdr->reserved, 0, sizeof(pstHdr->reserved));
        pstHdr->nPort = 0;
        pstHdr->IP = 0;
    }
}

/**
 * @brief ������ҵ��ͷ������Ӧҵ��ͷ
 *
 * @param pstResHdr
 * @param pstReqHdr
 */
void InterCommHdr_req2res(yv_InterCommHdr *pstResHdr, yv_InterCommHdr *pstReqHdr) {
    if (pstResHdr == NULL || pstReqHdr == NULL) {
        return;
    }

    pstResHdr->Version = pstReqHdr->Version;
    pstResHdr->HdrLen = pstReqHdr->HdrLen;
    memcpy(&pstResHdr->reserved, &pstReqHdr->reserved, sizeof(pstResHdr->reserved));
    pstResHdr->nPort = pstReqHdr->nPort;
    pstResHdr->IP = pstReqHdr->IP;

    pstResHdr->SServerID = pstReqHdr->DServerID;
    pstResHdr->SObjID = pstReqHdr->DObjID;
    pstResHdr->DServerID = pstReqHdr->SServerID;
    pstResHdr->DObjID = pstReqHdr->SObjID;
    pstResHdr->SessionID = pstReqHdr->SessionID;
}

/**
 * @brief ��ȡ��ǰʱ���
 *
 * @return ��ǰʱ���. ��λ: ms
 */
unsigned long prot_get_timestamp() {
    unsigned long ts = 0;
    struct timeval now;
    if (gettimeofday(&now, NULL) == 0) {
        ts = (now.tv_sec * 1000) + (now.tv_usec / 1000);
    }
    return ts;
}

/**
 * @brief ��ȡ��ǰʱ���
 *
 * @return ��ǰʱ���. ��λ: us
 */
unsigned long long prot_get_timestamp_us() {
    unsigned long long ts = 0;
    struct timeval now;
    if (gettimeofday(&now, NULL) == 0) {
        ts = (now.tv_sec * 1000000) + (now.tv_usec);
    }
    return ts;
}

#ifndef __ANDROID__
#include <sys/syscall.h>

/**
 * @brief ��������߳�ID.
 */
pid_t gettid(void) {
    return syscall(SYS_gettid);
}
#endif

/**
 * @brief ��ȡ�ֱ���ö��ֵ
 *
 * @param w ��
 * @param h ��
 *
 * @return
 */
int prot_get_resolution_enum(int w, int h) {
    int retval = 0;
    if (w == 176 && h == 144) {
        // 176��144
        retval = QCIF;
    } else if (w == 352 && h == 288) {
        // 352��288
        retval = CIF;
    } else if (w == 704 && h == 288) {
        // 704��288
        retval = HalfD1;
    } else if (w == 704 && h == 576) {
        // 704x576
        retval = D1;
    } else if (w == 640 && h == 480) {
        // 640x480
        retval = VGA;
    } else if (w == 1280 && h == 720) {
        // 1280x720
        retval = HD720;
    } else if (w == 1920 && h == 1080) {
        // 1920x1080
        retval = HD1080;
    } else {
        retval = UNKnow_DPII;
    }

    return retval;
}

/**
 * @brief �ɷֱ���ö��ֵ��ȡ�ֱ���
 *
 * @param resolution_enum   �ֱ���ö��ֵ
 * @param width ��. ��������
 * @param height    ��. ��������
 *
 * @return
 * @retval {@enum SUCCESS}
 * @retval {@enum ENULL_POINTER}
 * @retval {@enum EINVALID_PARAMETER}
 */
int prot_get_resolution(int resolution_enum, int *width, int *height) {
    int retval = SUCCESS;

    if (width == NULL || height == NULL) {
        retval = ENULL_POINTER;
        goto ERR;
    }

    switch (resolution_enum) {
        case QCIF: 
            *width = 176;
            *height = 144;
            break;
        case CIF: 
            *width = 352;
            *height = 288;
            break;
        case HalfD1: 
            *width = 704;
            *height = 288;
            break;
        case D1: 
            *width = 704;
            *height = 576;
            break;
        case VGA: 
            *width = 640;
            *height = 480;
            break;
        case HD720: 
            *width = 1280;
            *height = 720;
            break;
        case HD1080: 
            *width = 1920;
            *height = 1080;
            break;
        default: 
            retval = EINVALID_PARAMETER;
            break;
    }

ERR:
    return retval;
}

/**
 * @brief ��ȡ������ö��ֵ
 *
 * @param sample_rate   ��HzΪ��λ�Ĳ�����
 *
 * @return
 */
int prot_get_sample_rate_enum(int sample_rate) {
    int retval = 0;
    switch (sample_rate) {
        case 8000:
            retval = ASAMPLE_8000HZ;
            break;
        case 11025:
            retval = ASAMPLE_11025HZ;
            break;
        case 22050:
            retval = ASAMPLE_22050HZ;
            break;
        case 32000:
            retval = ASAMPLE_32000HZ;
            break;
        case 44100:
            retval = ASAMPLE_44100HZ;
            break;
        case 47250:
            retval = ASAMPLE_47250HZ;
            break;
        case 48000:
            retval = ASAMPLE_48000HZ;
            break;
        case 50000:
            retval = ASAMPLE_50000HZ;
            break;
        case 50400:
            retval = ASAMPLE_50400HZ;
            break;
        case 96000:
            retval = ASAMPLE_96000HZ;
            break;
        case 192000:
            retval = ASAMPLE_192000HZ;
            break;
        case 2822400:
            retval = ASAMPLE_2822400HZ;
            break;
        case 24000:
            retval = ASAMPLE_24000HZ;
            break;
        case 4000:
            retval = ASAMPLE_4000HZ;
            break;
        case 6000:
            retval = ASAMPLE_6000HZ;
            break;
        case 12000:
            retval = ASAMPLE_12000HZ;
            break;
        case 16000:
            retval = ASAMPLE_16000HZ;
            break;
        default:
            retval = 0;
            break;
    }
    return retval;
}

/**
 * @brief ��ȡ��������ö��ֵ
 *
 * @param type
 *
 * @return
 */
int prot_get_encode_type_enum(int type) {
    int retval = 0;
    switch (type) {
        case PROT_ENCODE_TYPE_OPUS:
            retval = ACODEC_OPUS;
            break;
        case PROT_ENCODE_TYPE_H264:
            retval = VIDEOCODEC_H264;
            break;
#if 0
        case PROT_ENCODE_TYPE_H265:
            retval = VIDEOCODEC_H265;
            break;
#endif
        default:
            retval = 0;
            break;
    }

    return retval;
}

/**
 * @brief ����ͷSensor����: �ɾ���ֵ��ȡ��ֵ
 *
 * @param rat   ��ֵ. ��������
 * @param abs   ����ֵ
 *
 * @return
 * @retval {@enum SUCCESS}
 * @retval {@enum ENULL_POINTER}
 */
int prot_get_camera_parameters_ratio(prot_camera_parameters *rat, ST_VideoColorAttr *abs) {
    int retval = SUCCESS;

    if (rat == NULL || abs == NULL) {
        retval = ENULL_POINTER;
        goto ERR;
    }

    rat->brightness = 1000 * (abs->cBrightness - PROT_CLI_BRIGHTNESS_MIN) / (PROT_CLI_BRIGHTNESS_MAX - PROT_CLI_BRIGHTNESS_MIN);
    rat->contrast = 1000 * (abs->cContrast - PROT_CLI_CONTRAST_MIN) / (PROT_CLI_CONTRAST_MAX - PROT_CLI_CONTRAST_MIN);
    rat->saturation = 1000 * (abs->cSaturation - PROT_CLI_SATURATION_MIN) / (PROT_CLI_SATURATION_MAX - PROT_CLI_SATURATION_MIN);
    rat->hue = 1000 * (abs->cHue - PROT_CLI_HUE_MIN) / (PROT_CLI_HUE_MAX - PROT_CLI_HUE_MIN);
    rat->sharpness = 1000 * (abs->cAcutance - PROT_CLI_SHARPNESS_MIN) / (PROT_CLI_SHARPNESS_MAX - PROT_CLI_SHARPNESS_MIN);
    rat->exposure = 1000 * (abs->cWdr - PROT_CLI_EXPOSURE_MIN) / (PROT_CLI_EXPOSURE_MAX - PROT_CLI_EXPOSURE_MIN);

ERR:
    return retval;
}

/**
 * @brief ����ͷSensor����: �ɱ�ֵ��ȡ����ֵ
 *
 * @param abs   ����ֵ. ��������
 * @param rat   ��ֵ
 *
 * @return
 * @retval {@enum SUCCESS}
 * @retval {@enum ENULL_POINTER}
 */
int prot_get_camera_parameters_absolute(ST_VideoColorAttr *abs, prot_camera_parameters *rat) {
    int retval = SUCCESS;

    if (abs == NULL || rat == NULL) {
        retval = ENULL_POINTER;
        goto ERR;
    }

    abs->lImageScale = 0;
    abs->cBrightness = (PROT_CLI_BRIGHTNESS_MAX - PROT_CLI_BRIGHTNESS_MIN) * rat->brightness / 1000;
    abs->cContrast = (PROT_CLI_CONTRAST_MAX - PROT_CLI_CONTRAST_MIN) * rat->contrast / 1000;
    abs->cSaturation = (PROT_CLI_SATURATION_MAX - PROT_CLI_SATURATION_MIN) * rat->saturation / 1000;
    abs->cHue = (PROT_CLI_HUE_MAX - PROT_CLI_HUE_MIN) * rat->hue / 1000;
    abs->cAcutance = (PROT_CLI_SHARPNESS_MAX - PROT_CLI_SHARPNESS_MIN) * rat->sharpness / 1000;
    abs->cWdr = (PROT_CLI_EXPOSURE_MAX - PROT_CLI_EXPOSURE_MIN) * rat->exposure / 1000;

ERR:
    return retval;
}

/**
 * @brief ����ͷͼ�����: �ɱ��ض������ͻ�ȡЭ�鶨������
 *
 * @param protocol  Э�鶨������
 * @param local     ���ض�������
 *
 * @return
 * @retval {@enum SUCCESS}
 * @retval {@enum ENULL_POINTER}
 */
int prot_image_parameters_ltop(ST_CameraCfg *protocol, prot_image_parameters *local) {
    int retval = SUCCESS;
    if (protocol == NULL || local == NULL) {
        retval = ENULL_POINTER;
        goto ERR;
    }

    protocol->lRotation = local->rotation;
    protocol->lFlip = local->flip;
    protocol->lCamerEnable = local->light_buffle_switch;
    protocol->lCamerColorSw = local->colour_mode;
    protocol->lMirror = local->mirror_mode;
    memset(&protocol->reserver, 0, sizeof(protocol->reserver));

ERR:
    return retval;
}

/**
 * @brief ����ͷͼ�����: ��Э�鶨�����ͻ�ȡ���ض�������
 *
 * @param local     ���ض�������
 * @param protocol  Э�鶨������
 *
 * @return
 * @retval {@enum SUCCESS}
 * @retval {@enum ENULL_POINTER}
 */
int prot_image_parameters_ptol(prot_image_parameters *local, ST_CameraCfg *protocol) {
    int retval = SUCCESS;
    if (local == NULL || protocol == NULL) {
        retval = ENULL_POINTER;
        goto ERR;
    }

    local->rotation = protocol->lRotation;
    local->flip = protocol->lFlip;
    local->light_buffle_switch = protocol->lCamerEnable;
    local->colour_mode = protocol->lCamerColorSw;
    local->mirror_mode = protocol->lMirror;

ERR:
    return retval;
}

/**
 * @brief �豸����״̬: �ɱ��ض������ͻ�ȡЭ�鶨������
 *
 * @param protocol  Э�鶨������
 * @param local     ���ض�������
 *
 * @return
 * @retval {@enum SUCCESS}
 * @retval {@enum ENULL_POINTER}
 */
int prot_device_state_ltop(ST_DevStatusInfo *protocol, prot_device_state *local) {
    int retval = SUCCESS;
    if (protocol == NULL || local == NULL) {
        retval = ENULL_POINTER;
        goto ERR;
    }

    protocol->stDevBatteryInfo.lBattery = local->capacity;
    protocol->stDevBatteryInfo.lVotage = local->voltage;
    protocol->lCpuTemperature = local->temperature;
    protocol->lCpuOccupy = local->cpu_usage;
    protocol->lMemRemaining = local->memory_available;
    protocol->lStorageRemaining = local->storage_available;
    protocol->lRunTime = local->run_time;

ERR:
    return retval;
}

/**
 * @brief λ����Ϣ: �ɱ��ض������ͻ�ȡЭ�鶨������
 *
 * @param protocol  Э�鶨������
 * @param local     ���ض�������
 *
 * @return
 * @retval {@enum SUCCESS}
 * @retval {@enum ENULL_POINTER}
 */
int prot_location_info_ltop(ST_PositionData *protocol, prot_location_info *local) {
    int retval = SUCCESS;
    if (protocol == NULL || local == NULL) {
        retval = ENULL_POINTER;
        goto ERR;
    }

    if (/*����*/local->longitude_degree < -179 
        || local->longitude_degree > 179 
        || local->longitude_minute > 59 
        || local->longitude_second > 59 
        || /*γ��*/local->latitude_degree < -89 
        || local->latitude_degree > 89 
        || local->latitude_minute > 59 
        || local->latitude_second > 59 
        || /*��������*/local->speed > 9999 
        || /*���溽��*/local->direction_degree > 359 
        || local->direction_minute > 59) {
            retval = EINVALID_PARAMETER;
            goto ERR;
        }

    char c = 0;
    /*UTC*/
    struct tm *p_tm = gmtime(&local->utc);
    strftime(protocol->szUTC, sizeof(protocol->szUTC), "%g%m%d%H%M%S", p_tm);
    /*����*/
    if (local->longitude_degree < 0) {
        c = 'W';
        local->longitude_degree *= -1;
    } else {
        c = 'E';
    }
    snprintf(protocol->szLongitude, sizeof(protocol->szLongitude), "%c%3d%2d.%d", c, local->longitude_degree, local->longitude_minute, 10000 * local->longitude_second / 60);
    /*γ��*/
    if (local->latitude_degree < 0) {
        c = 'S';
        local->latitude_degree *= -1;
    } else {
        c = 'N';
    }
    snprintf(protocol->szLatitude, sizeof(protocol->szLatitude), "%c%2d%2d.%d", c, local->latitude_degree, local->latitude_minute, 10000 * local->latitude_second / 60);
    /*��������*/
    snprintf(protocol->szSpeed, sizeof(protocol->szSpeed), "%3d.%d", local->speed / 10, local->speed % 10);
    /*���溽��*/
    snprintf(protocol->szOrientation, sizeof(protocol->szOrientation), "%3d.%d", local->direction_degree, 10 * local->direction_minute / 60);

ERR:
    return retval;
}

/**
 * @brief ����ͷ��������: �ɱ��ض������ͻ�ȡЭ�鶨������
 *
 * @param protocol  Э�鶨������
 * @param local     ���ض�������
 *
 * @return
 * @retval {@enum SUCCESS}
 * @retval {@enum ENULL_POINTER}
 */
int prot_camera_scene_ltop(ST_VideoColorScene *protocol, prot_camera_scene *local) {
    int retval = SUCCESS;
    if (protocol == NULL || local == NULL) {
        retval = ENULL_POINTER;
        goto ERR;
    }

    protocol->lVideoColorSceneNum = local->number;
    memcpy(protocol->szSceneDescribe, local->describer, PROT_CAMERA_SCENE_DESCRIBER_LENGTH_MAX);
    prot_get_camera_parameters_absolute(&protocol->stVideoColorAttr, &local->params);

ERR:
    return retval;
}

/**
 * @brief ����ͷ��������: ��Э�鶨�����ͻ�ȡ���ض�������
 *
 * @param local     ���ض�������
 * @param protocol  Э�鶨������
 *
 * @return
 * @retval {@enum SUCCESS}
 * @retval {@enum ENULL_POINTER}
 */
int prot_camera_scene_ptol(prot_camera_scene *local, ST_VideoColorScene *protocol) {
    int retval = SUCCESS;
    if (local == NULL || protocol == NULL) {
        retval = ENULL_POINTER;
        goto ERR;
    }

    local->number = protocol->lVideoColorSceneNum;
    memcpy(local->describer, protocol->szSceneDescribe, PROT_CAMERA_SCENE_DESCRIBER_LENGTH_MAX);
    prot_get_camera_parameters_ratio(&local->params, &protocol->stVideoColorAttr);

ERR:
    return retval;
}

/**************************************************
 * ��������.
 **************************************************/

/**
 * @brief �����豸��Ϣ
 * ����{@func prot_init()}֮ǰ����.
 *
 * @param device    �豸��Ϣ
 *      NULL: ����豸��Ϣ
 */
void prot_set_device_info(prot_device_info *device) {
    if (device == NULL) {
        memset(&prot_instance.device, 0, sizeof(prot_device_info));
    } else {
        memcpy(&prot_instance.device, device, sizeof(prot_device_info));
    }
}

/**
 * @brief ���÷�������Ϣ
 * ����{@func prot_init()}֮ǰ����.
 *
 * @param server    ��������Ϣ
 *      NULL: ��շ�������Ϣ
 */
void prot_set_server_info(prot_server_info *server) {
    if (server == NULL) {
        memset(&prot_instance.server, 0, sizeof(prot_server_info));
    } else {
        memcpy(&prot_instance.server, server, sizeof(prot_server_info));
        prot_instance.server.ip[IP_BUFFER_LENGTH_MAX - 1] = 0;
    }
}

/**
 * @brief ���ûص�
 * ����{@func prot_init()}֮ǰ����.
 *
 * @param callback  �ص�
 */
void prot_set_callback(prot_callback *callback) {
    prot_instance.callback = callback;
}

/**
 * @brief ������Ƶ��Ϣ
 * ����{@func prot_init()}֮�����.
 *
 * @param audio ��Ƶ��Ϣ
 *      NULL: �����Ƶ��Ϣ
 *
 * @return
 * @retval {@enum SUCCESS}
 */
int prot_update_audio_info(prot_audio_info *audio) {
    int retval = SUCCESS;

    if (audio == NULL) {
        memset(&prot_instance.audio, 0, sizeof(prot_instance.audio));
        prot_instance.audio.encode_type = PROT_ENCODE_TYPE_UNKNOWN;
    } else {
        memcpy(&prot_instance.audio, audio, sizeof(prot_instance.audio));
        prot_instance.audio_stream_id = PROT_AUDIO_STREAM_ID;
    }

    return retval;
}

/**
 * @brief ������Ƶ��Ϣ
 * ����{@func prot_init()}֮�����.
 *
 * @param video ��Ƶ��Ϣ
 *      NULL: �����Ƶ��Ϣ
 *
 * @return
 * @retval {@enum SUCCESS}
 */
int prot_update_video_info(prot_video_info *video) {
    int retval = SUCCESS;

    if (video == NULL) {
        memset(&prot_instance.video, 0, sizeof(prot_instance.video));
        prot_instance.video.encode_type = PROT_ENCODE_TYPE_UNKNOWN;
    } else {
        memcpy(&prot_instance.video, video, sizeof(prot_instance.video));
        prot_instance.video_stream_id = PROT_VIDEO_STREAM_ID;
    }

    return retval;
}

/**************************************************
 * ��ʼ��/��Դ�ͷ�.
 **************************************************/

/**
 * @brief Э����ʼ��
 *
 * @return
 * @retval {@enum SUCCESS}  �ɹ�
 * @retval {@enum EINVALID_DEVICE_ID}   �豸ID���Ϸ�
 * @retval {@enum EINVALID_SERVER_IP}   ������IP���Ϸ�
 * @retval {@enum EUNKNOWN} δ֪����
 */
int prot_init() {
    int retval = SUCCESS;

    /**
     * �豸ID�Ϸ��Լ��.
     */
    if (prot_instance.device.id < DEVICE_ID_MIN || prot_instance.device.id > DEVICE_ID_MAX) {
        LOGE("%s: invalid device id(%lu).\n", __func__, prot_instance.device.id);
        retval = EINVALID_DEVICE_ID;
        goto ERR;
    }

    /**
     * ������IP�Ϸ��Լ��.
     */
    struct in_addr addr;
    if (inet_aton(prot_instance.server.ip, &addr) == 0) {
        LOGE("%s: invalid server ip(%s).\n", __func__, prot_instance.server.ip);
        retval = EINVALID_SERVER_IP;
        goto ERR;
    }

    // 
    prot_instance.state = PROT_STATE_VALID;

    //
    pthread_mutex_init(&prot_instance.mutex, NULL);

    // 
    memset(&prot_instance.audio, 0, sizeof(prot_instance.audio));
    memset(&prot_instance.video, 0, sizeof(prot_instance.video));
    memset(&prot_instance.session, 0, sizeof(prot_instance.session));
    prot_instance.login_flag = PROT_FLAG_LOGOUT;
    prot_instance.monitor_thread_pid = 0;
    prot_instance.is_online = PROT_FALSE;
    prot_instance.keep_alive_timestamp = 0;

    /**
     * ������ʼ��.
     */
    int err = udpx_init(prot_instance.server.ip, prot_instance.server.port);
    if (err != SUCCESS) {
        LOGE("%s: udpx init failed, errno: %d.\n", __func__, err);
        retval = err;
        goto ERR;
    }
    err = udpx_start();
    if (err != SUCCESS) {
        LOGE("%s: udpx start failed, errno: %d.\n", __func__, err);
        retval = err;
        goto ERR;
    }

    /**
     * ������Ϣ�����߳�.
     */
    err = pthread_create(&prot_instance.msg_recv_thread_pid, NULL, &prot_msg_recv_thread, NULL);
    if (err != 0) {
        prot_instance.msg_recv_thread_pid = 0;
        LOGE("%s: recv thread: pthread_create() failed: %s.\n", __func__, strerror(err));
        retval = EUNKNOWN;
        goto ERR;
    }

    return retval;

ERR:
    prot_fini();
    return retval;
}

/**
 * @brief Э�����Դ�ͷ�
 *
 * @return
 * @retval {@enum SUCCESS}
 */
int prot_fini() {
    int retval = SUCCESS;
    int err = 0;

    if (prot_instance.login_flag == PROT_FLAG_LOGIN) {
        prot_logout();
    }

    // 
    prot_instance.state = PROT_STATE_INVALID;

    /**
     * ֹͣ��Ϣ�����߳�.
     */
    err = pthread_join(prot_instance.msg_recv_thread_pid, NULL);
    if (err != 0) {
        LOGW("%s: recv thread: pthread_join() failed: %s.\n", __func__, strerror(err));
    }
    prot_instance.msg_recv_thread_pid = 0;

    /**
     * �������Դ�ͷ�.
     */
    udpx_fini();

    // 
    memset(&prot_instance.audio, 0, sizeof(prot_instance.audio));
    memset(&prot_instance.video, 0, sizeof(prot_instance.video));
    memset(&prot_instance.session, 0, sizeof(prot_instance.session));
    prot_instance.callback = NULL;
    prot_instance.login_flag = PROT_FLAG_LOGOUT;
    prot_instance.monitor_thread_pid = 0;
    prot_instance.is_online = PROT_FALSE;
    prot_instance.keep_alive_timestamp = 0;

    // 
    pthread_mutex_destroy(&prot_instance.mutex);

    return retval;
}

/**************************************************
 * ��¼/ע��/����.
 **************************************************/

/**
 * @brief ��¼
 *
 * @return
 * @retval {@enum EINVALID_STATE}   δ��ʼ��
 * @retval {@enum EMALLOC_FAILED}   �ڴ�������
 */
int prot_login() {
    int retval = SUCCESS;

    if (prot_instance.state != PROT_STATE_VALID) {
        LOGW("%s: instance not ready.\n", __func__);
        retval = EINVALID_STATE;
        goto ERR;
    }

	ST_DevReqSrvDeviceAuthen *pstAuthenReq = NULL; /*�豸��֤����*/

    /**/
    int msg_length_max = sizeof(ST_DevReqSrvDeviceAuthen);

	udpx_buffer *pBuffer = udpx_get_buffer(msg_length_max);
    if (pBuffer == NULL) {
        LOGE("%s: get buffer failed.\n", __func__);
        retval = EMALLOC_FAILED;
        goto ERR;
    }
	pstAuthenReq = (ST_DevReqSrvDeviceAuthen *) (pBuffer->content + pBuffer->offset);
    memset(pstAuthenReq, 0, msg_length_max);

	/*ҵ��ͷ��*/
    InterCommHdr_init(&pstAuthenReq->stInterHdr);
	pstAuthenReq->stInterHdr.MsgType = DEV_REQ_SRV_DEVICE_AUTHEN;
    pstAuthenReq->stInterHdr.SServerID = 0;
	pstAuthenReq->stInterHdr.SObjID = prot_instance.device.id;
    pstAuthenReq->stInterHdr.DServerID = 0;
    pstAuthenReq->stInterHdr.DObjID = prot_instance.device.id;
	pstAuthenReq->stInterHdr.SessionID = DEFAULT_SESSION_ID;
    /*���ݲ�*/
	pstAuthenReq->stDataHdr.lTransactionID = 0;
    pstAuthenReq->stDataHdr.lBusinessID = 0;
    /*��Ϣ�غ�*/
	pstAuthenReq->stDevice.lDeviceID= prot_instance.device.id;
    /*����*/
    pstAuthenReq->stDataHdr.lDataLength = sizeof(ST_Device);
	pstAuthenReq->stInterHdr.DataLength = sizeof(ST_DataHdr) + pstAuthenReq->stDataHdr.lDataLength;
	/*���͵�½����*/
	pBuffer->payload_length = sizeof(yv_InterCommHdr) + pstAuthenReq->stInterHdr.DataLength;
	udpx_send_register(pBuffer);

    // 
    prot_instance.login_flag = PROT_FLAG_LOGIN;

    prot_instance.keep_alive_timestamp = prot_get_timestamp();
    /**
     * �����������߳�.
     */
    if (prot_instance.monitor_thread_pid == 0) {
        int err = pthread_create(&prot_instance.monitor_thread_pid, NULL, &prot_monitor_thread, NULL);
        if (err != 0) {
            prot_instance.monitor_thread_pid = 0;
            LOGE("%s: monitor thread: pthread_create() failed: %s.\n", __func__, strerror(err));
            retval = EUNKNOWN;
            goto ERR;
        }
    }

    return retval;

ERR:
    prot_logout();
    return retval;
}

/**
 * @brief ע��
 *
 * @return
 * @retval {@enum EINVALID_STATE}   δ��ʼ��
 * @retval {@enum EMALLOC_FAILED}   �ڴ�������
 */
int prot_logout() {
    int retval = SUCCESS;

    if (prot_instance.state != PROT_STATE_VALID) {
        LOGW("%s: instance not ready.\n", __func__);
        retval = EINVALID_STATE;
        goto ERR_1;
    }

	ST_DevNfySrvDeviceLogout *pstLogoutNfy = NULL;

    /**/
    int msg_length_max = sizeof(ST_DevNfySrvDeviceLogout);

	udpx_buffer *pBuffer = udpx_get_buffer(msg_length_max);
    if (pBuffer == NULL) {
        LOGE("%s: get buffer failed.\n", __func__);
        retval = EMALLOC_FAILED;
        goto ERR_2;
    }
    pstLogoutNfy = (ST_DevNfySrvDeviceLogout *) (pBuffer->content + pBuffer->offset);
    memset(pstLogoutNfy, 0, msg_length_max);

    /*ҵ��ͷ��*/
    InterCommHdr_init(&pstLogoutNfy->stInterHdr);
	pstLogoutNfy->stInterHdr.MsgType = DEV_NFY_SRV_DEVICE_LOGOUT;
    pstLogoutNfy->stInterHdr.SServerID = prot_instance.session.src_server_id;
	pstLogoutNfy->stInterHdr.SObjID = prot_instance.session.src_object_id;
    pstLogoutNfy->stInterHdr.DServerID = prot_instance.session.dst_server_id;
    pstLogoutNfy->stInterHdr.DObjID = prot_instance.session.dst_object_id;
	pstLogoutNfy->stInterHdr.SessionID = prot_instance.session.id;
    /*��Ϣ�غ�*/
    pstLogoutNfy->stDevice.lDeviceID = prot_instance.device.id;
    /*����*/
	pstLogoutNfy->stInterHdr.DataLength = sizeof(ST_Device);
	/*�����豸����֪ͨ*/
	pBuffer->payload_length = sizeof(yv_InterCommHdr) + pstLogoutNfy->stInterHdr.DataLength;
	udpx_send_data(pBuffer);

ERR_2:
    //
    prot_instance.is_online = PROT_FALSE;
    prot_instance.login_flag = PROT_FLAG_LOGOUT;

    /**
     * ֹͣ�������߳�.
     */
    int err = pthread_join(prot_instance.monitor_thread_pid, NULL);
    if (err != 0) {
        LOGW("%s: monitor thread: pthread_join() failed: %s.\n", __func__, strerror(err));
    }
    prot_instance.monitor_thread_pid = 0;

ERR_1:
    return retval;
}

/**
 * @brief ����
 *
 * @return
 * @retval {@enum EINVALID_STATE}   δ��ʼ��
 * @retval {@enum EMALLOC_FAILED}   �ڴ�������
 */
int prot_keep_alive() {
    int retval = SUCCESS;

    if (prot_instance.state != PROT_STATE_VALID) {
        LOGW("%s: instance not ready.\n", __func__);
        retval = EINVALID_STATE;
        goto ERR;
    }
    
    ST_DevReqSrvKeepAlive *pstKeepAliveReq = NULL;

    /**/
    int msg_length_max = sizeof(ST_DevReqSrvKeepAlive);

    udpx_buffer *pBuffer = udpx_get_buffer(msg_length_max);
    if (pBuffer == NULL) {
        LOGE("%s: get buffer failed.\n", __func__);
        retval = EMALLOC_FAILED;
        goto ERR;
    }
    pstKeepAliveReq = (ST_DevReqSrvKeepAlive *) (pBuffer->content + pBuffer->offset);
    memset(pstKeepAliveReq, 0, msg_length_max);

    /*ҵ��ͷ��*/
    InterCommHdr_init(&pstKeepAliveReq->stInterHdr);
	pstKeepAliveReq->stInterHdr.MsgType = DEV_REQ_SRV_KEEP_ALIVE;
    pstKeepAliveReq->stInterHdr.SServerID = prot_instance.session.src_server_id;
	pstKeepAliveReq->stInterHdr.SObjID = prot_instance.session.src_object_id;
    pstKeepAliveReq->stInterHdr.DServerID = prot_instance.session.dst_server_id;
    pstKeepAliveReq->stInterHdr.DObjID = prot_instance.session.dst_object_id;
	pstKeepAliveReq->stInterHdr.SessionID = prot_instance.session.id;
    /*���ݲ�*/
	pstKeepAliveReq->stDataHdr.lTransactionID = 0;
    pstKeepAliveReq->stDataHdr.lBusinessID = 0;
    /*����*/
    pstKeepAliveReq->stDataHdr.lDataLength = 0;
	pstKeepAliveReq->stInterHdr.DataLength = sizeof(ST_DataHdr) + pstKeepAliveReq->stDataHdr.lDataLength;
	/*����������Ϣ*/
	pBuffer->payload_length = sizeof(yv_InterCommHdr) + pstKeepAliveReq->stInterHdr.DataLength;
	udpx_send_data(pBuffer);

ERR: 
    return retval;
}

#define INTERVAL_KEEP_ALIVE_DEVICE_MS (INTERVAL_KEEP_ALIVE_DEVICE / 1000)
#define MAX_TIMEOUT_SESSION_MS (MAX_TIMEOUT_SESSION / 1000)

/**
 * @brief ״̬����߳�
 * ���߱���.
 * �����ص�¼.
 *
 * @param param
 */
void *prot_monitor_thread(void *param) {
    LOGI("%s: pid is %d.\n", __func__, gettid());

    unsigned long current_timestamp = prot_get_timestamp();
    unsigned long backup_timestamp = current_timestamp;

    while (prot_instance.login_flag == PROT_FLAG_LOGIN) {
        sleep(1);

        current_timestamp = prot_get_timestamp();
        if (current_timestamp - backup_timestamp >= INTERVAL_KEEP_ALIVE_DEVICE_MS/*5s*/) {
            backup_timestamp = current_timestamp;

            if (prot_instance.is_online == PROT_TURE && current_timestamp - prot_instance.keep_alive_timestamp >= MAX_TIMEOUT_SESSION_MS/*30s*/) {
                LOGE("%s: session timeout, device is offline.\n", __func__);
                prot_instance.is_online = PROT_FALSE;

                // Callback.
                if (prot_instance.callback != NULL && prot_instance.callback->on_offline != NULL) {
                    prot_instance.callback->on_offline();
                }
            }

            if (prot_instance.login_flag == PROT_FLAG_LOGIN) {
                if (prot_instance.is_online == PROT_TURE) {
                    // ���߱���.
                    prot_keep_alive();
                } else {
                    // �����ص�¼.
                    LOGI("%s: device is offline, retry login.\n", __func__);
                    prot_login();
                }
            }
        }
    }

    LOGI("%s: exit, pid is %d.\n", __func__, gettid());
    return NULL;
}

/**************************************************
 * ����֡����.
 **************************************************/

#define PROT_AUDIO_PACKET_HEADER_LENGTH \
        (sizeof(yv_InterCommHdr) + sizeof(ST_StreamHdr) + sizeof(ST_AudioStreamExtInfo) + sizeof(ST_AudioPrivateData))
#define PROT_AUDIO_PACKET_LENGTH_MAX \
        (UDPX_PAYLOAD_MAX_LENGTH - PROT_AUDIO_PACKET_HEADER_LENGTH)

static unsigned int prot_audio_seqno = 0;
static unsigned long prot_audio_timestamp = 0;
static int prot_audio_frame_count = 0;
static int prot_audio_length = 0;
static char prot_audio_packet_buffer[PROT_AUDIO_PACKET_LENGTH_MAX];

/**
 * @brief ��Ƶ������
 * ��������CBR����ģʽ.
 *
 * @param buffer    ��Ƶ�����ݻ���
 * @param length    ��Ƶ�����ݳ���
 * @param timestamp ��Ƶ������֡��Ƶ֡ʱ���
 * @param frame_count   ��Ƶ��������Ƶ֡֡��
 *
 * @return
 * @retval {@enum SUCCESS}
 * @retval {@enum EINVALID_STATE}
 * @retval {@enum ENULL_POINTER}
 * @retval {@enum EDATA_TOO_LARGE}
 * @retval {@enum EMALLOC_FAILED}
 */
int prot_audio_packet_send(const char * buffer, int length, long timestamp, int frame_count) {
    int retval = SUCCESS;

    if (prot_instance.is_online == PROT_FALSE || (prot_instance.stream_upload_flag & PROT_STREAM_FLAG_UPLOAD) == 0) {
        prot_audio_seqno = 0;
        retval = EINVALID_STATE;
        goto ERR;
    }

    if (buffer == NULL) {
        LOGE("%s: buffer is null.\n", __func__);
        retval = ENULL_POINTER;
        goto ERR;
    }
    if (length > sizeof(prot_audio_packet_buffer)) {
        LOGE("%s: audio packet length is too large.\n", __func__);
        retval = EDATA_TOO_LARGE;
        goto ERR;
    }

    ST_DevSrmSrvMediaStream *pstStream = NULL;
    ST_AudioStreamExtInfo extInfo;
    ST_AudioPrivateData privData;

    /**/
    int msg_length_max = PROT_AUDIO_PACKET_HEADER_LENGTH + length;

    udpx_buffer *pBuffer = udpx_get_buffer(msg_length_max);
    if (pBuffer == NULL) {
        LOGE("%s: get buffer failed.\n", __func__);
        retval = EMALLOC_FAILED;
        goto ERR;
    }
    pstStream = (ST_DevSrmSrvMediaStream *) (pBuffer->content + pBuffer->offset);
    memset(pstStream, 0, msg_length_max);

    /*ҵ��ͷ��*/
    InterCommHdr_init(&pstStream->stInterCommHdr);
    pstStream->stInterCommHdr.MsgType = DEV_SRM_SRV_MEDIA_STREAM;
    pstStream->stInterCommHdr.SServerID = prot_instance.session.src_server_id;
    pstStream->stInterCommHdr.SObjID = prot_instance.session.src_object_id;
    pstStream->stInterCommHdr.DServerID = prot_instance.session.dst_server_id;
    pstStream->stInterCommHdr.DObjID = prot_instance.session.dst_object_id;
    pstStream->stInterCommHdr.SessionID = prot_instance.session.id;
    // 
    pstStream->stInterCommHdr.DataLength = sizeof(ST_StreamHdr) + sizeof(ST_AudioStreamExtInfo) + sizeof(ST_AudioPrivateData) + length;
    /*��ý��ͷ��*/
    pstStream->stStreamHdr.lStreamID = prot_instance.audio_stream_id;
    pstStream->stStreamHdr.lType = STREAM_TYPE_AUDIO;
    pstStream->stStreamHdr.lLen = length;
    pstStream->stStreamHdr.lExpInfoType = EXPINFOTYPE_AUDIOCODEC;
    pstStream->stStreamHdr.lExpInfoLen = sizeof(ST_AudioStreamExtInfo) + sizeof(ST_AudioPrivateData);

    pstStream->stStreamHdr.stProperty.audio.timeStamp = (unsigned int) timestamp;
    pstStream->stStreamHdr.stProperty.audio.seqNo = prot_audio_seqno;
    memset(&pstStream->stStreamHdr.stProperty.audio.reserve, 0, sizeof(pstStream->stStreamHdr.stProperty.audio.reserve));
    // 
    prot_audio_seqno += frame_count;
    /*��չ��Ϣ*/
    extInfo.lCodec = prot_get_encode_type_enum(prot_instance.audio.encode_type);
    extInfo.lSample = prot_get_sample_rate_enum(prot_instance.audio.sample_rate);
    extInfo.lBitwidth = prot_instance.audio.quantization;
    extInfo.lChannelNum = prot_instance.audio.channel_count;
    memcpy(pstStream->Content, &extInfo, sizeof(ST_AudioStreamExtInfo));
    /*˽������*/
    if (extInfo.lCodec == ACODEC_OPUS) {
        privData.stOpusCodecInfo.lBiteRate = extInfo.lSample;
        privData.stOpusCodecInfo.nFragSize = length;
        privData.stOpusCodecInfo.cFragCount = frame_count;
        privData.stOpusCodecInfo.cTsGap = (length * 8 * 1000) / prot_instance.audio.bitrate / frame_count;
    }
    memcpy(pstStream->Content + sizeof(ST_AudioStreamExtInfo), &privData, sizeof(ST_AudioPrivateData));
    /*�غ�*/
    memcpy(pstStream->Content + sizeof(ST_AudioStreamExtInfo) + sizeof(ST_AudioPrivateData), buffer, length);
    /*������Ƶ����*/
    pBuffer->payload_length = PROT_AUDIO_PACKET_HEADER_LENGTH + length;
    udpx_send_data(pBuffer);

    return retval;

ERR:
    return retval;
}

/**
 * @brief ��Ƶ֡����
 * ��������CBR����ģʽ.
 *
 * @param buffer    һ֡��Ƶ����
 * @param length    һ֡��Ƶ���ݳ���
 * @param timestamp ��Ƶ֡ʱ���
 *
 * @return
 * @retval {@enum SUCCESS}
 * @retval {@enum EINVALID_STATE}
 * @retval {@enum ENULL_POINTER}
 * @retval {@enum EDATA_TOO_LARGE}
 */
int prot_audio_frame_send(const char *buffer, int length, long timestamp) {
    int retval = SUCCESS;

    if (prot_instance.is_online == PROT_FALSE || (prot_instance.stream_upload_flag & PROT_STREAM_FLAG_UPLOAD) == 0) {
        // δ����, ���ϴ���ʶδ��.
        prot_audio_seqno = 0;
        prot_audio_frame_count = 0;
        prot_audio_length = 0;
        retval = EINVALID_STATE;
        goto ERR;
    }

    if (buffer == NULL) {
        LOGE("%s: buffer is null.\n", __func__);
        retval = ENULL_POINTER;
        goto ERR;
    }
    if (length > sizeof(prot_audio_packet_buffer)) {
        LOGE("%s: audio frame length is too large.\n", __func__);
        retval = EDATA_TOO_LARGE;
        goto ERR;
    }

    if (prot_audio_length + length > sizeof(prot_audio_packet_buffer)) {
        // ����
        prot_audio_packet_send(prot_audio_packet_buffer, prot_audio_length, prot_audio_timestamp, prot_audio_frame_count);
        
        prot_audio_frame_count = 0;
        prot_audio_length = 0;
    }

    if (prot_audio_frame_count == 0) {
        prot_audio_timestamp = timestamp;
    }

    memcpy(&prot_audio_packet_buffer[prot_audio_length], buffer, length);
    prot_audio_frame_count += 1;
    prot_audio_length += length;

    return retval;

ERR:
    return retval;
}

static unsigned int prot_video_seqno = 0;

/**
 * @brief ��Ƶ֡����
 *
 * @param buffer    ��Ƶ֡����
 * @param length    ��Ƶ֡���ݳ���
 * @param frame_type    ��Ƶ֡����
 * @param timestamp ��Ƶ֡ʱ���
 *
 * @return
 * @retval {@enum SUCCESS}
 * @retval {@enum EINVALID_STATE}
 * @retval {@enum ENULL_POINTER}
 * @retval {@enum EMALLOC_FAILED}
 */
int prot_video_frame_send(const char *buffer, int length, int frame_type, long timestamp) {
    int retval = SUCCESS;

    if (prot_instance.is_online == PROT_FALSE || (prot_instance.stream_upload_flag & PROT_STREAM_FLAG_UPLOAD) == 0) {
        prot_video_seqno = 0;
        retval = EINVALID_STATE;
        goto ERR;
    }

    if (buffer == NULL) {
        LOGE("%s: buffer is null.\n", __func__);
        retval = ENULL_POINTER;
        goto ERR;
    }

    ST_DevSrmSrvMediaStream *pstStream = NULL;
    ST_VideoStreamExtInfo *pstExtInfo = NULL;

    /**/
    int buffer_length = 0;
    if (frame_type == PROT_FRAME_I) {
        buffer_length = sizeof(yv_InterCommHdr) + sizeof(ST_StreamHdr) + sizeof(ST_VideoStreamExtInfo) + length;
    } else {
        buffer_length = sizeof(yv_InterCommHdr) + sizeof(ST_StreamHdr) + length;
    }

    udpx_buffer *pBuffer = udpx_get_buffer(buffer_length);
    if (pBuffer == NULL) {
        LOGE("%s: get buffer failed.\n", __func__);
        retval = EMALLOC_FAILED;
        goto ERR;
    }
    pstStream = (ST_DevSrmSrvMediaStream *) (pBuffer->content + pBuffer->offset);
    memset(pstStream, 0, buffer_length);

    /*ҵ��ͷ��*/
    InterCommHdr_init(&pstStream->stInterCommHdr);
    pstStream->stInterCommHdr.MsgType = DEV_SRM_SRV_MEDIA_STREAM;
    pstStream->stInterCommHdr.SServerID = prot_instance.session.src_server_id;
    pstStream->stInterCommHdr.SObjID = prot_instance.session.src_object_id;
    pstStream->stInterCommHdr.DServerID = prot_instance.session.dst_server_id;
    pstStream->stInterCommHdr.DObjID = prot_instance.session.dst_object_id;
    pstStream->stInterCommHdr.SessionID = prot_instance.session.id;
    // 
    pstStream->stInterCommHdr.DataLength = buffer_length - sizeof(yv_InterCommHdr);
    /*��ý��ͷ��*/
    pstStream->stStreamHdr.lStreamID = prot_instance.video_stream_id;
    pstStream->stStreamHdr.lType = STREAM_TYPE_VIDEO;
    pstStream->stStreamHdr.lLen = length;
    if (frame_type == PROT_FRAME_I) {
        /*�ؼ�֡*/
        pstStream->stStreamHdr.lExpInfoType = EXPINFOTYPE_VIDEOCODEC;
        pstStream->stStreamHdr.lExpInfoLen = sizeof(ST_VideoStreamExtInfo);
        pstStream->stStreamHdr.stProperty.video.keyFrame = 1;
        pstStream->stStreamHdr.stProperty.video.seqNo = prot_video_seqno++;
        pstStream->stStreamHdr.stProperty.video.timeStamp = (unsigned int) timestamp;
        /*��չ��Ϣ*/
        pstExtInfo = (ST_VideoStreamExtInfo *) pstStream->Content;
        pstExtInfo->lCodec = prot_get_encode_type_enum(prot_instance.video.encode_type);
        pstExtInfo->lDpi = prot_get_resolution_enum(prot_instance.video.width, prot_instance.video.height);
        pstExtInfo->lWidth = prot_instance.video.width;
        pstExtInfo->lHeight = prot_instance.video.height;
        /*�غ�*/
        memcpy(pstStream->Content + sizeof(ST_VideoStreamExtInfo), buffer, length);
    } else {
        /*�ǹؼ�֡*/
        pstStream->stStreamHdr.lExpInfoType = EXPINFOTYPE_NONE;
        pstStream->stStreamHdr.lExpInfoLen = 0;
        pstStream->stStreamHdr.stProperty.video.keyFrame = 0;
        pstStream->stStreamHdr.stProperty.video.seqNo = prot_video_seqno++;
        pstStream->stStreamHdr.stProperty.video.timeStamp = (unsigned int) timestamp;
        /*�غ�*/
        memcpy(pstStream->Content, buffer, length);
    }

    /*������Ƶ����*/
    pBuffer->payload_length = buffer_length;
    udpx_send_data(pBuffer);

    return retval;

ERR:
    return retval;
}

/**************************************************
 * ��Ϣ����(����).
 **************************************************/

/**
 * @brief �豸״̬�ϱ�
 * Ӧ���ѭ��(����5s)�����ϱ��豸״̬.
 *
 * @param state �豸����״̬
 * @param location  �豸λ����Ϣ
 *
 * @return
 */
int prot_device_state_send(prot_device_state *state, prot_location_info *location) {
    int retval = SUCCESS;

    if (prot_instance.is_online == PROT_FALSE) {
        /*δ����*/
        retval = EINVALID_STATE;
        goto ERR;
    }

    ST_DevNfySrvStatusInfo *pstStatusInfo = NULL;

    /**/
    int msg_length_max = sizeof(ST_DevNfySrvStatusInfo);

    udpx_buffer *pBuffer = udpx_get_buffer(msg_length_max);
    if (pBuffer == NULL) {
        LOGE("%s: get buffer failed.\n", __func__);
        retval = EMALLOC_FAILED;
        goto ERR;
    }
    pstStatusInfo = (ST_DevNfySrvStatusInfo *) (pBuffer->content + pBuffer->offset);
    memset(pstStatusInfo, 0, msg_length_max);

    /*ҵ��ͷ��*/
    InterCommHdr_init(&pstStatusInfo->stInterHdr);
    pstStatusInfo->stInterHdr.MsgType = DEV_NFY_SRV_STATUS_INFO;
    pstStatusInfo->stInterHdr.SServerID = prot_instance.session.src_server_id;
    pstStatusInfo->stInterHdr.SObjID = prot_instance.session.src_object_id;
    pstStatusInfo->stInterHdr.DServerID = prot_instance.session.dst_server_id;
    pstStatusInfo->stInterHdr.DObjID = prot_instance.session.dst_object_id;
    pstStatusInfo->stInterHdr.SessionID = prot_instance.session.id;
    /*�غ�*/
    if (state != NULL) {
        prot_device_state_ltop(&pstStatusInfo->stDevStatusInfo, state);
    }
    if (location != NULL) {
        prot_location_info_ltop(&pstStatusInfo->stDevStatusInfo.stPositionData, location);
    }
    /*����*/
    pstStatusInfo->stInterHdr.DataLength = sizeof(ST_DevStatusInfo);
    /*����*/
    pBuffer->payload_length = sizeof(yv_InterCommHdr) + pstStatusInfo->stInterHdr.DataLength;
    udpx_send_data(pBuffer);

    return retval;

ERR:
    return retval;
}

/**************************************************
 * Э����Ϣ����/����.
 **************************************************/

/**
 * @brief ��Ϣ�����߳�
 *
 * @param param
 */
void *prot_msg_recv_thread(void *param) {
    LOGI("%s: pid is %d.\n", __func__, gettid());

	udpx_buffer *pBuffer = NULL;
	yv_InterCommHdr *pstInterCommHdr = NULL;

    while (prot_instance.state == PROT_STATE_VALID) {
        pBuffer = udpx_get_message_timeout(1000);
        if (pBuffer == NULL) {
            LOGI("%s: timeout.\n", __func__);
            continue;
        }

        if (prot_instance.login_flag != PROT_FLAG_LOGIN) {
            LOGW("%s: device is not online, ignore this message.\n", __func__);
            udpx_free_message(pBuffer);
            continue;
        }

        if (pBuffer->length < sizeof(yv_InterCommHdr)) {
            LOGW("%s: invalid message, length is too short.\n", __func__);
            udpx_free_message(pBuffer);
            continue;
        }

        pstInterCommHdr = (yv_InterCommHdr *) (pBuffer->content + pBuffer->offset);
        switch (pstInterCommHdr->MsgType) {
            case DEV_RES_SRV_DEVICE_AUTHEN: 
                LOGD("%s: DEV_RES_SRV_DEVICE_AUTHEN\n", __func__);
                prot_process_res_login((char *) pstInterCommHdr, pBuffer->length);
                break;
            case DEV_RES_SRV_KEEP_ALIVE: 
                LOGD("%s: DEV_RES_SRV_KEEP_ALIVE\n", __func__);
                prot_process_res_keep_alive((char *) pstInterCommHdr, pBuffer->length);
                break;
            case SRV_REQ_DEV_STREAM_INFO: 
                LOGD("%s: SRV_REQ_DEV_STREAM_INFO\n", __func__);
                prot_process_req_stream_info((char *) pstInterCommHdr, pBuffer->length);
                break;
            case SRV_NFY_DEV_START_MEDIA_STREAM:
                LOGD("%s: SRV_NFY_DEV_START_MEDIA_STREAM\n", __func__);
                prot_process_nfy_start_stream((char *) pstInterCommHdr, pBuffer->length);
                break;
            case SRV_NFY_DEV_STOP_MEDIA_STREAM: 
                LOGD("%s: SRV_NFY_DEV_STOP_MEDIA_STREAM\n", __func__);
                prot_process_nfy_stop_stream((char *) pstInterCommHdr, pBuffer->length);
                break;
            case SRV_REQ_DEV_CHG_VIDEO_PARAM: 
                LOGD("%s: SRV_REQ_DEV_CHG_VIDEO_PARAM\n", __func__);
                prot_process_req_change_video_parameters((char *) pstInterCommHdr, pBuffer->length);
                break;
            case SRV_REQ_DEV_GET_VIDEO_COLOR_ATTR: 
                LOGD("%s: SRV_REQ_DEV_GET_VIDEO_COLOR_ATTR\n", __func__);
                prot_process_req_get_camera_parameters((char *) pstInterCommHdr, pBuffer->length);
                break;
            case SRV_REQ_DEV_SET_VIDEO_COLOR_ATTR: 
                LOGD("%s: SRV_REQ_DEV_SET_VIDEO_COLOR_ATTR\n", __func__);
                prot_process_req_set_camera_parameters((char *) pstInterCommHdr, pBuffer->length);
                break;
            case SRV_REQ_DEV_RESET_VIDEO_COLOR_ATTR: 
                LOGD("%s: SRV_REQ_DEV_RESET_VIDEO_COLOR_ATTR\n", __func__);
                prot_process_req_reset_camera_parameters((char *) pstInterCommHdr, pBuffer->length);
                break;
            case SRV_REQ_DEV_GET_CAMERA_CFG: 
                LOGD("%s: SRV_REQ_DEV_GET_CAMERA_CFG\n", __func__);
                prot_process_req_get_image_parameters((char *) pstInterCommHdr, pBuffer->length);
                break;
            case SRV_REQ_DEV_SET_CAMERA_CFG: 
                LOGD("%s: SRV_REQ_DEV_SET_CAMERA_CFG\n", __func__);
                prot_process_req_set_image_parameters((char *) pstInterCommHdr, pBuffer->length);
                break;
            case SRV_REQ_DEV_GET_MOTTON_DETECT_ATTR: 
                LOGD("%s: SRV_REQ_DEV_GET_MOTTON_DETECT_ATTR\n", __func__);
                break;
            case SRV_REQ_DEV_SET_MOTTON_DETECT_ATTR: 
                LOGD("%s: SRV_REQ_DEV_SET_MOTTON_DETECT_ATTR\n", __func__);
                break;
            case SRV_REQ_DEV_GET_VIDEO_COLOR_SCENE: 
                LOGD("%s: SRV_REQ_DEV_GET_VIDEO_COLOR_SCENE\n", __func__);
                prot_process_req_get_scenes((char *) pstInterCommHdr, pBuffer->length);
                break;
            case SRV_REQ_DEV_SET_VIDEO_COLOR_SCENE: 
                LOGD("%s: SRV_REQ_DEV_SET_VIDEO_COLOR_SCENE\n", __func__);
                prot_process_req_set_scene((char *) pstInterCommHdr, pBuffer->length);
                break;
            case SRV_REQ_DEV_USE_VIDEO_COLOR_SCENE: 
                LOGD("%s: SRV_REQ_DEV_USE_VIDEO_COLOR_SCENE\n", __func__);
                prot_process_req_apply_scene((char *) pstInterCommHdr, pBuffer->length);
                break;
            case SRV_REQ_DEV_PARAM_QUERY: 
                LOGD("%s: SRV_REQ_DEV_PARAM_QUERY\n", __func__);
                break;
            default:
                LOGE("%s: Unspported message: 0x%X.\n", __func__, pstInterCommHdr->MsgType);
                break;
        }
		udpx_free_message(pBuffer);
    }

    LOGI("%s: exit, pid is %d.\n", __func__, gettid());
    return NULL;
}

/**
 * @brief ��¼��֤��Ӧ��Ϣ����
 *
 * @param msg
 * @param length
 *
 * @return
 * @retval {@enum SUCCESS}
 * @retval {@enum ENULL_POINTER}
 * @retval {@enum EWRONG_BUFFER}
 */
int prot_process_res_login(const char *msg, int length) {
    int retval = SUCCESS;

    if (msg == NULL) {
        LOGW("%s: msg is null.\n", __func__);
        retval = ENULL_POINTER;
        goto ERR;
    }
    if (length < sizeof(ST_DevResSrvDeviceAuthen)) {
        LOGW("%s: length is too short.\n", __func__);
        retval = EWRONG_BUFFER;
        goto ERR;
    }

	ST_DevResSrvDeviceAuthen *pstAuthenRes = (ST_DevResSrvDeviceAuthen *) msg;

    if (pstAuthenRes->stMsgRet.lRet == 0) {
        LOGI("%s: login okay.\n", __func__);
        // ��¼�ɹ�.
        prot_instance.is_online = PROT_TURE;
        prot_instance.session.id = pstAuthenRes->stInterHdr.SessionID;
        prot_instance.session.src_server_id = pstAuthenRes->stInterHdr.DServerID;
        prot_instance.session.src_object_id = pstAuthenRes->stInterHdr.DObjID;
        prot_instance.session.dst_server_id = pstAuthenRes->stInterHdr.SServerID;
        prot_instance.session.dst_object_id = pstAuthenRes->stInterHdr.SObjID;

        udpx_set_session_id(prot_instance.session.id);

        prot_audio_seqno = 0;
        prot_video_seqno = 0;

        // Callback.
        if (prot_instance.callback != NULL && prot_instance.callback->on_online != NULL) {
            prot_instance.callback->on_online();
        }
    } else {
        prot_login_errno err = LE_UNKNOWN;
        // ��¼ʧ��.
        switch(pstAuthenRes->stMsgRet.lSts) {
            case ERR_LOGIN_UNKNOWN:
                err = LE_UNKNOWN;
                LOGE("��¼δ֪����");
                break;
            case ERR_LOGIN_ACCOUNT:
                err = LE_INVALID_ACCOUNT;
                LOGE("�˺Ų�����");
                break;
            case ERR_LOGIN_PASSWD:
                err = LE_WRONG_PASSWD;
                LOGE("�������");
                break;
            case ERR_LOGIN_TIMEOUT:
                err = LE_TIMEOUT;
                LOGE("��֤��ʱ");
                break;
            case ERR_LOGIN_RELOGIN:
                err = LE_RELOGIN;
                LOGE("�ظ���½");
                break;
            case ERR_LOGIN_LIMIT:
                err = LE_BEYOND_UPPER_LIMIT;
                LOGE("������������");
                break;
            default:
                err = LE_UNKNOWN;
                LOGE("δ֪��½����");
                break;
        }

        // Callback.
        if (prot_instance.callback != NULL && prot_instance.callback->on_login_failed != NULL) {
            prot_instance.callback->on_login_failed(err);
        }
    }

ERR:
    return retval;
}

/**
 * @brief ������Ӧ��Ϣ����
 *
 * @param msg
 * @param length
 *
 * @return
 * @retval {@enum SUCCESS}
 */
int prot_process_res_keep_alive(const char *msg, int length) {
    int retval = SUCCESS;

    prot_instance.keep_alive_timestamp = prot_get_timestamp();

    return retval;
}

/**
 * @brief ����Ϣ��ѯ��Ӧ��Ϣ����
 *
 * @param msg
 * @param length
 *
 * @return
 * @retval {@enum SUCCESS}
 * @retval {@enum ENULL_POINTER}
 * @retval {@enum EWRONG_BUFFER}
 * @retval {@enum EMALLOC_FAILED}
 */
int prot_process_req_stream_info(const char *msg, int length) {
    int retval = SUCCESS;

    if (msg == NULL) {
        LOGW("%s: msg is null.\n", __func__);
        retval = ENULL_POINTER;
        goto ERR;
    }
    if (length < sizeof(ST_SrvReqDevStreamInfo)) {
        LOGW("%s: length is too short.\n", __func__);
        retval = EWRONG_BUFFER;
        goto ERR;
    }

    ST_SrvReqDevStreamInfo *pstStreamInfoReq = (ST_SrvReqDevStreamInfo *) msg;
    ST_SrvResDevStreamInfo *pstStreamInfoRes = NULL;
    ST_VectorStreamInfo *pstStreamInfo = NULL;

    /**/
    int stream_count = 0;
    int msg_length_max = sizeof(ST_SrvResDevStreamInfo) + sizeof(ST_VectorStreamInfo) * MAX_SUPPORT_STREAM_COUNT;

    udpx_buffer *pBuffer = udpx_get_buffer(msg_length_max);
    if (pBuffer == NULL) {
        LOGE("%s: get buffer failed.\n", __func__);
        retval = EMALLOC_FAILED;
        goto ERR;
    }
    pstStreamInfoRes = (ST_SrvResDevStreamInfo *) (pBuffer->content + pBuffer->offset);
    memset(pstStreamInfoRes, 0, msg_length_max);

    /*ҵ��ͷ��*/
    InterCommHdr_req2res(&pstStreamInfoRes->stInterHdr, &pstStreamInfoReq->stInterHdr);
    pstStreamInfoRes->stInterHdr.MsgType = SRV_RES_DEV_STREAM_INFO;
    /*���ݲ�*/
    memcpy(&pstStreamInfoRes->stDataHdr, &pstStreamInfoReq->stDataHdr, sizeof(ST_DataHdr));
    /*��Ӧ���*/
    pstStreamInfoRes->stMsgRes.lRet = YV_SUCCESS;
    /*����Ϣ*/
    /*��Ƶ��*/
    if (prot_instance.video.encode_type != PROT_ENCODE_TYPE_UNKNOWN) {
        pstStreamInfo = &pstStreamInfoRes->stDevStreamInfo.stVectorStreamInfo[stream_count];
        pstStreamInfo->lStreamType = STREAM_TYPE_VIDEO;
        pstStreamInfo->lStreamID = prot_instance.video_stream_id;
        pstStreamInfo->stStreamCapParam.lVideo_picdpi = prot_get_resolution_enum(prot_instance.video.width, prot_instance.video.height);
        pstStreamInfo->stStreamCapParam.lVideo_codec = prot_get_encode_type_enum(prot_instance.video.encode_type);
        // 
        stream_count += 1;
    }
    /*��Ƶ��*/
    if (prot_instance.audio.encode_type != PROT_ENCODE_TYPE_UNKNOWN) {
        pstStreamInfo = &pstStreamInfoRes->stDevStreamInfo.stVectorStreamInfo[stream_count];
        pstStreamInfo->lStreamType = STREAM_TYPE_AUDIO;
        pstStreamInfo->lStreamID = prot_instance.audio_stream_id;
        pstStreamInfo->stStreamCapParam.lAudio_codec = prot_get_encode_type_enum(prot_instance.audio.encode_type);
        pstStreamInfo->stStreamCapParam.lAudio_sample = prot_get_sample_rate_enum(prot_instance.audio.sample_rate);
        pstStreamInfo->stStreamCapParam.lAudio_bitwidth = prot_instance.audio.quantization;
        pstStreamInfo->stStreamCapParam.lAudio_bitrate = prot_instance.audio.bitrate;
        // 
        stream_count += 1;
    }
    //
    pstStreamInfoRes->stDevStreamInfo.lStreamCount = stream_count;

    /*����*/
    pstStreamInfoRes->stDataHdr.lDataLength = sizeof(ST_MsgRespRet) + sizeof(ST_DevStreamInfo) + sizeof(ST_VectorStreamInfo) * stream_count;
    pstStreamInfoRes->stInterHdr.DataLength = sizeof(ST_DataHdr) + pstStreamInfoRes->stDataHdr.lDataLength;
    /*������Ӧ*/
    pBuffer->payload_length = sizeof(yv_InterCommHdr) + pstStreamInfoRes->stInterHdr.DataLength;
    udpx_send_data(pBuffer);

    return retval;

ERR:
    return retval;
}

/**
 * @brief ������֪ͨ��Ϣ����
 *
 * @param msg
 * @param length
 *
 * @return
 * @retval {@enum SUCCESS}
 */
int prot_process_nfy_start_stream(const char *msg, int length) {
    int retval = SUCCESS;

    prot_instance.stream_upload_flag |= PROT_STREAM_FLAG_UPLOAD;

    // Callback.
    if (prot_instance.callback != NULL && prot_instance.callback->on_stream_start != NULL) {
        prot_instance.callback->on_stream_start();
    }

    return retval;
}

/**
 * @brief ֹͣ��֪ͨ��Ϣ����
 *
 * @param msg
 * @param length
 *
 * @return
 * @retval {@enum SUCCESS}
 */
int prot_process_nfy_stop_stream(const char *msg, int length) {
    int retval = SUCCESS;

    prot_instance.stream_upload_flag &= !PROT_STREAM_FLAG_UPLOAD;

    // Callback.
    if (prot_instance.callback != NULL && prot_instance.callback->on_stream_stop != NULL) {
        prot_instance.callback->on_stream_stop();
    }

    return retval;
}

/**
 * @brief �޸���Ƶ����������
 *
 * @param msg
 * @param length
 *
 * @return
 * @retval {@enum SUCCESS}
 * @retval {@enum ENULL_POINTER}
 * @retval {@enum EWRONG_BUFFER}
 * @retval {@enum EMALLOC_FAILED}
 */
int prot_process_req_change_video_parameters(const char *msg, int length) {
    int retval = SUCCESS;

    if (msg == NULL) {
        LOGW("%s: msg is null.\n", __func__);
        retval = ENULL_POINTER;
        goto ERR;
    }
    if (length < sizeof(ST_SrvReqDevChgVideoParam)) {
        LOGW("%s: length is too short.\n", __func__);
        retval = EWRONG_BUFFER;
        goto ERR;
    }

    ST_SrvReqDevChgVideoParam *pstChgVideoParamReq = (ST_SrvReqDevChgVideoParam *) msg;
    ST_SrvResDevChgVideoParam *pstChgVideoParamRes = NULL;
    ST_CltChgVideoParam *pstVideoParam = &pstChgVideoParamReq->stCltChgVideoParam;
    ST_CltChgVideoParam *p = pstVideoParam;

    /*�޸���Ƶ����*/
    prot_on_video_parameters_change_errno err = VPCE_UNSUPPORTED;
    LOGD("%s: streamID(%d), bitrate(%d), framerate(%d), dpi(%d), brightness(%d), color(%d), contrast(%d), width(%d), height(%d), gop(%d).\n", __func__, p->lStreamID, p->lBitrate, p->lFramerate, p->lDpi, p->lBrightness, p->lColor, p->lContrast, p->lWidth, p->lHeight, p->lGop);

    if (prot_instance.callback != NULL && prot_instance.callback->on_video_parameters_change != NULL) {
        prot_video_parameters params = {0};
        if (prot_get_resolution(p->lDpi, &params.width, &params.height) == SUCCESS) {
            params.framerate = p->lFramerate;
            params.bitrate = p->lBitrate * 1000;
            // Callback.
            err = prot_instance.callback->on_video_parameters_change(&params);
        }
    }

    /**/
    int msg_length_max = sizeof(ST_SrvResDevChgVideoParam);

    udpx_buffer *pBuffer = udpx_get_buffer(msg_length_max);
    if (pBuffer == NULL) {
        LOGE("%s: get buffer failed.\n", __func__);
        retval = EMALLOC_FAILED;
        goto ERR;
    }
    pstChgVideoParamRes = (ST_SrvResDevChgVideoParam *) (pBuffer->content + pBuffer->offset);
    memset(pstChgVideoParamRes, 0, msg_length_max);

    /*ҵ��ͷ��*/
    InterCommHdr_req2res(&pstChgVideoParamRes->stInterHdr, &pstChgVideoParamReq->stInterHdr);
    pstChgVideoParamRes->stInterHdr.MsgType = SRV_RES_DEV_CHG_VIDEO_PARAM;
    /*���ݲ�*/
    memcpy(&pstChgVideoParamRes->stDataHdr, &pstChgVideoParamReq->stDataHdr, sizeof(ST_DataHdr));
    /*��Ӧ���*/
    if (err == VPCE_SUCCESS) {
        pstChgVideoParamRes->stMsgRet.lRet = YV_SUCCESS;
        pstChgVideoParamRes->stMsgRet.lSts = 0;
    } else {
        pstChgVideoParamRes->stMsgRet.lRet = YV_FAIL;
        switch (err) {
            case VPCE_UNSUPPORTED: 
                pstChgVideoParamRes->stMsgRet.lSts = ERR_DEV_VIDEO_SWITCH_NOT_SUPPORT;
                break;
            case VPCE_BUSY: 
                pstChgVideoParamRes->stMsgRet.lSts = ERR_DEV_VIDEO_SWITCH_TRY_AGAIN;
                break;
            case VPCE_CAPTURER_EXCEPTION: 
                pstChgVideoParamRes->stMsgRet.lSts = ERR_DEV_VIDEO_SWITCH_CAMERA_EXCEPTION;
                break;
            case VPCE_ENCODER_EXCEPTION: 
                pstChgVideoParamRes->stMsgRet.lSts = ERR_DEV_VIDEO_SWITCH_AVC_EXCEPTION;
                break;
            default: 
                pstChgVideoParamRes->stMsgRet.lSts = ERR_DEV_VIDEO_SWITCH_AVC_NORESPONSE;
                break;
        }
    }
    /*����*/
    pstChgVideoParamRes->stDataHdr.lDataLength = sizeof(ST_MsgRespRet);
    pstChgVideoParamRes->stInterHdr.DataLength = sizeof(ST_DataHdr) + pstChgVideoParamRes->stDataHdr.lDataLength;
    /*������Ӧ*/
    pBuffer->payload_length = sizeof(yv_InterCommHdr) + pstChgVideoParamRes->stInterHdr.DataLength;
    udpx_send_data(pBuffer);

    return retval;

ERR: 
    return retval;
}

/**
 * @brief ��ѯ����ͷSensor����������
 *
 * @param msg
 * @param length
 *
 * @return
 * @retval {@enum SUCCESS}
 * @retval {@enum ENULL_POINTER}
 * @retval {@enum EWRONG_BUFFER}
 * @retval {@enum EMALLOC_FAILED}
 */
int prot_process_req_get_camera_parameters(const char *msg, int length) {
    int retval = SUCCESS;

    if (msg == NULL) {
        LOGW("%s: msg is null.\n", __func__);
        retval = ENULL_POINTER;
        goto ERR;
    }
    if (length < sizeof(ST_SrvReqDevGetVideoColorAttr)) {
        LOGW("%s: length is too short.\n", __func__);
        retval = EWRONG_BUFFER;
        goto ERR;
    }

    ST_SrvReqDevGetVideoColorAttr *pstGetVideoColorAttrReq = (ST_SrvReqDevGetVideoColorAttr *) msg;
    ST_SrvResDevGetVideoColorAttr *pstGetVideoColorAttrRes = NULL;

    /**/
    int msg_length_max = sizeof(ST_SrvResDevGetVideoColorAttr);

    udpx_buffer *pBuffer = udpx_get_buffer(msg_length_max);
    if (pBuffer == NULL) {
        LOGE("%s: get buffer failed.\n", __func__);
        retval = EMALLOC_FAILED;
        goto ERR;
    }
    pstGetVideoColorAttrRes = (ST_SrvResDevGetVideoColorAttr *) (pBuffer->content + pBuffer->offset);
    memset(pstGetVideoColorAttrRes, 0, msg_length_max);

    /*ҵ��ͷ��*/
    InterCommHdr_req2res(&pstGetVideoColorAttrRes->stInterHdr, &pstGetVideoColorAttrReq->stInterHdr);
    pstGetVideoColorAttrRes->stInterHdr.MsgType = SRV_RES_DEV_GET_VIDEO_COLOR_ATTR;
    /*���ݲ�*/
    memcpy(&pstGetVideoColorAttrRes->stDataHdr, &pstGetVideoColorAttrReq->stDataHdr, sizeof(ST_DataHdr));
    /*��Ӧ���*/
    if (prot_instance.callback != NULL && prot_instance.callback->on_camera_parameters_get != NULL) {
        prot_camera_parameters params = {0};
        // Callback.
        int err = prot_instance.callback->on_camera_parameters_get(&params);
        if (err == PROT_SUCCESS) {
            prot_get_camera_parameters_absolute(&pstGetVideoColorAttrRes->stVideoColorAttr, &params);
            pstGetVideoColorAttrRes->stMsgRet.lRet = YV_SUCCESS;
            pstGetVideoColorAttrRes->stMsgRet.lSts = 0;
        } else {
            pstGetVideoColorAttrRes->stMsgRet.lRet = YV_FAIL;
            pstGetVideoColorAttrRes->stMsgRet.lSts = 0;
        }
    } else {
        pstGetVideoColorAttrRes->stMsgRet.lRet = YV_FAIL;
        pstGetVideoColorAttrRes->stMsgRet.lSts = ERR_COMMON_DEVICE_UPSUPPORT;
    }
    /*����*/
    pstGetVideoColorAttrRes->stDataHdr.lDataLength = sizeof(ST_MsgRespRet) + sizeof(ST_VideoColorAttr);
    pstGetVideoColorAttrRes->stInterHdr.DataLength = sizeof(ST_DataHdr) + pstGetVideoColorAttrRes->stDataHdr.lDataLength;
    /*������Ӧ*/
    pBuffer->payload_length = sizeof(yv_InterCommHdr) + pstGetVideoColorAttrRes->stInterHdr.DataLength;
    udpx_send_data(pBuffer);

    return retval;

ERR:
    return retval;
}

/**
 * @brief ��������ͷSensor����������
 *
 * @param msg
 * @param length
 *
 * @return
 * @retval {@enum SUCCESS}
 * @retval {@enum ENULL_POINTER}
 * @retval {@enum EWRONG_BUFFER}
 * @retval {@enum EMALLOC_FAILED}
 */
int prot_process_req_set_camera_parameters(const char *msg, int length) {
    int retval = SUCCESS;

    if (msg == NULL) {
        LOGW("%s: msg is null.\n", __func__);
        retval = ENULL_POINTER;
        goto ERR;
    }
    if (length < sizeof(ST_SrvReqDevSetVideoColorAttr)) {
        LOGW("%s: length is too short.\n", __func__);
        retval = EWRONG_BUFFER;
        goto ERR;
    }

    ST_SrvReqDevSetVideoColorAttr *pstSetVideoColorAttrReq = (ST_SrvReqDevSetVideoColorAttr *) msg;
    ST_SrvResDevSetVideoColorAttr *pstSetVideoColorAttrRes = NULL;

    /**/
    int msg_length_max = sizeof(ST_SrvResDevSetVideoColorAttr);

    udpx_buffer *pBuffer = udpx_get_buffer(msg_length_max);
    if (pBuffer == NULL) {
        LOGE("%s: get buffer failed.\n", __func__);
        retval = EMALLOC_FAILED;
        goto ERR;
    }
    pstSetVideoColorAttrRes = (ST_SrvResDevSetVideoColorAttr *) (pBuffer->content + pBuffer->offset);
    memset(pstSetVideoColorAttrRes, 0, msg_length_max);

    /*ҵ��ͷ��*/
    InterCommHdr_req2res(&pstSetVideoColorAttrRes->stInterHdr, &pstSetVideoColorAttrReq->stInterHdr);
    pstSetVideoColorAttrRes->stInterHdr.MsgType = SRV_RES_DEV_SET_VIDEO_COLOR_ATTR;
    /*���ݲ�*/
    memcpy(&pstSetVideoColorAttrRes->stDataHdr, &pstSetVideoColorAttrReq->stDataHdr, sizeof(ST_DataHdr));
    /*��Ӧ���*/
    if (prot_instance.callback != NULL && prot_instance.callback->on_camera_parameters_set != NULL) {
        prot_camera_parameters params = {0};
        prot_get_camera_parameters_ratio(&params, &pstSetVideoColorAttrReq->stVideoColorAttr);
        // Callback.
        int err = prot_instance.callback->on_camera_parameters_set(&params);
        if (err == PROT_SUCCESS) {
            pstSetVideoColorAttrRes->stMsgRet.lRet = YV_SUCCESS;
            pstSetVideoColorAttrRes->stMsgRet.lSts = 0;
        } else {
            pstSetVideoColorAttrRes->stMsgRet.lRet = YV_FAIL;
            pstSetVideoColorAttrRes->stMsgRet.lSts = 0;
        }
    } else {
        pstSetVideoColorAttrRes->stMsgRet.lRet = YV_FAIL;
        pstSetVideoColorAttrRes->stMsgRet.lSts = ERR_COMMON_DEVICE_UPSUPPORT;
    }
    /*����*/
    pstSetVideoColorAttrRes->stDataHdr.lDataLength = sizeof(ST_MsgRespRet);
    pstSetVideoColorAttrRes->stInterHdr.DataLength = sizeof(ST_DataHdr) + pstSetVideoColorAttrRes->stDataHdr.lDataLength;
    /*������Ӧ*/
    pBuffer->payload_length = sizeof(yv_InterCommHdr) + pstSetVideoColorAttrRes->stInterHdr.DataLength;
    udpx_send_data(pBuffer);

    return retval;

ERR:
    return retval;
}

/**
 * @brief ��������ͷSensor����������
 *
 * @param msg
 * @param length
 *
 * @return
 * @retval {@enum SUCCESS}
 * @retval {@enum ENULL_POINTER}
 * @retval {@enum EWRONG_BUFFER}
 * @retval {@enum EMALLOC_FAILED}
 */
int prot_process_req_reset_camera_parameters(const char *msg, int length) {
    int retval = SUCCESS;

    if (msg == NULL) {
        LOGW("%s: msg is null.\n", __func__);
        retval = ENULL_POINTER;
        goto ERR;
    }
    if (length < sizeof(ST_SrvReqDevResetVideoColorAttr)) {
        LOGW("%s: length is too short.\n", __func__);
        retval = EWRONG_BUFFER;
        goto ERR;
    }

    ST_SrvReqDevResetVideoColorAttr *pstResetVideoColorAttrReq = (ST_SrvReqDevResetVideoColorAttr *) msg;
    ST_SrvResDevResetVideoColorAttr *pstResetVideoColorAttrRes = NULL;

    /**/
    int msg_length_max = sizeof(ST_SrvResDevResetVideoColorAttr);

    udpx_buffer *pBuffer = udpx_get_buffer(msg_length_max);
    if (pBuffer == NULL) {
        LOGE("%s: get buffer failed.\n", __func__);
        retval = EMALLOC_FAILED;
        goto ERR;
    }
    pstResetVideoColorAttrRes = (ST_SrvResDevResetVideoColorAttr *) (pBuffer->content + pBuffer->offset);
    memset(pstResetVideoColorAttrRes, 0, msg_length_max);

    /*ҵ��ͷ��*/
    InterCommHdr_req2res(&pstResetVideoColorAttrRes->stInterHdr, &pstResetVideoColorAttrReq->stInterHdr);
    pstResetVideoColorAttrRes->stInterHdr.MsgType = SRV_RES_DEV_RESET_VIDEO_COLOR_ATTR;
    /*���ݲ�*/
    memcpy(&pstResetVideoColorAttrRes->stDataHdr, &pstResetVideoColorAttrReq->stDataHdr, sizeof(ST_DataHdr));
    /*��Ӧ���*/
    if (prot_instance.callback != NULL && prot_instance.callback->on_camera_parameters_reset != NULL) {
        // Callback.
        int err = prot_instance.callback->on_camera_parameters_reset();
        if (err == PROT_SUCCESS) {
            pstResetVideoColorAttrRes->stMsgRet.lRet = YV_SUCCESS;
            pstResetVideoColorAttrRes->stMsgRet.lSts = 0;
        } else {
            pstResetVideoColorAttrRes->stMsgRet.lRet = YV_FAIL;
            pstResetVideoColorAttrRes->stMsgRet.lSts = 0;
        }
    } else {
        pstResetVideoColorAttrRes->stMsgRet.lRet = YV_FAIL;
        pstResetVideoColorAttrRes->stMsgRet.lSts = ERR_COMMON_DEVICE_UPSUPPORT;
    }
    /*����*/
    pstResetVideoColorAttrRes->stDataHdr.lDataLength = sizeof(ST_MsgRespRet);
    pstResetVideoColorAttrRes->stInterHdr.DataLength = sizeof(ST_DataHdr) + pstResetVideoColorAttrRes->stDataHdr.lDataLength;
    /*������Ӧ*/
    pBuffer->payload_length = sizeof(yv_InterCommHdr) + pstResetVideoColorAttrRes->stInterHdr.DataLength;
    udpx_send_data(pBuffer);

    return retval;

ERR:
    return retval;
}

/**
 * @brief ��ѯ����ͷͼ�����������
 *
 * @param msg
 * @param length
 *
 * @return
 * @retval {@enum SUCCESS}
 * @retval {@enum ENULL_POINTER}
 * @retval {@enum EWRONG_BUFFER}
 * @retval {@enum EMALLOC_FAILED}
 */
int prot_process_req_get_image_parameters(const char *msg, int length) {
    int retval = SUCCESS;

    if (msg == NULL) {
        LOGW("%s: msg is null.\n", __func__);
        retval = ENULL_POINTER;
        goto ERR;
    }
    if (length < sizeof(ST_SrvReqDevGetCameraCfg)) {
        LOGW("%s: length is too short.\n", __func__);
        retval = EWRONG_BUFFER;
        goto ERR;
    }

    ST_SrvReqDevGetCameraCfg *pstGetCameraCfgReq = (ST_SrvReqDevGetCameraCfg *) msg;
    ST_SrvResDevGetCameraCfg *pstGetCameraCfgRes = NULL;

    /**/
    int msg_length_max = sizeof(ST_SrvResDevGetCameraCfg);

    udpx_buffer *pBuffer = udpx_get_buffer(msg_length_max);
    if (pBuffer == NULL) {
        LOGE("%s: get buffer failed.\n", __func__);
        retval = EMALLOC_FAILED;
        goto ERR;
    }
    pstGetCameraCfgRes = (ST_SrvResDevGetCameraCfg *) (pBuffer->content + pBuffer->offset);
    memset(pstGetCameraCfgRes, 0, msg_length_max);

    /*ҵ��ͷ��*/
    InterCommHdr_req2res(&pstGetCameraCfgRes->stInterHdr, &pstGetCameraCfgReq->stInterHdr);
    pstGetCameraCfgRes->stInterHdr.MsgType = SRV_RES_DEV_GET_CAMERA_CFG;
    /*���ݲ�*/
    memcpy(&pstGetCameraCfgRes->stDataHdr, &pstGetCameraCfgReq->stDataHdr, sizeof(ST_DataHdr));
    /*��Ӧ���*/
    if (prot_instance.callback != NULL && prot_instance.callback->on_image_parameters_get != NULL) {
        prot_image_parameters params = {0};
        // Callback.
        int err = prot_instance.callback->on_image_parameters_get(&params);
        if (err == PROT_SUCCESS) {
            prot_image_parameters_ltop(&pstGetCameraCfgRes->stCameraCfg, &params);
            pstGetCameraCfgRes->stMsgRet.lRet = YV_SUCCESS;
            pstGetCameraCfgRes->stMsgRet.lSts = 0;
        } else {
            pstGetCameraCfgRes->stMsgRet.lRet = YV_FAIL;
            pstGetCameraCfgRes->stMsgRet.lSts = 0;
        }
    } else {
        pstGetCameraCfgRes->stMsgRet.lRet = YV_FAIL;
        pstGetCameraCfgRes->stMsgRet.lSts = ERR_COMMON_DEVICE_UPSUPPORT;
    }
    /*����*/
    pstGetCameraCfgRes->stDataHdr.lDataLength = sizeof(ST_MsgRespRet) + sizeof(ST_CameraCfg);
    pstGetCameraCfgRes->stInterHdr.DataLength = sizeof(ST_DataHdr) + pstGetCameraCfgRes->stDataHdr.lDataLength;
    /*������Ӧ*/
    pBuffer->payload_length = sizeof(yv_InterCommHdr) + pstGetCameraCfgRes->stInterHdr.DataLength;
    udpx_send_data(pBuffer);

    return retval;

ERR:
    return retval;
}

/**
 * @brief ��������ͷͼ�����������
 *
 * @param msg
 * @param length
 *
 * @return
 * @retval {@enum SUCCESS}
 * @retval {@enum ENULL_POINTER}
 * @retval {@enum EWRONG_BUFFER}
 * @retval {@enum EMALLOC_FAILED}
 */
int prot_process_req_set_image_parameters(const char *msg, int length) {
    int retval = SUCCESS;

    if (msg == NULL) {
        LOGW("%s: msg is null.\n", __func__);
        retval = ENULL_POINTER;
        goto ERR;
    }
    if (length < sizeof(ST_SrvReqDevSetCameraCfg)) {
        LOGW("%s: length is too short.\n", __func__);
        retval = EWRONG_BUFFER;
        goto ERR;
    }

    ST_SrvReqDevSetCameraCfg *pstSetCameraCfgReq = (ST_SrvReqDevSetCameraCfg *) msg;
    ST_SrvResDevSetCameraCfg *pstSetCameraCfgRes = NULL;

    /**/
    int msg_length_max = sizeof(ST_SrvResDevSetCameraCfg);

    udpx_buffer *pBuffer = udpx_get_buffer(msg_length_max);
    if (pBuffer == NULL) {
        LOGE("%s: get buffer failed.\n", __func__);
        retval = EMALLOC_FAILED;
        goto ERR;
    }
    pstSetCameraCfgRes = (ST_SrvResDevSetCameraCfg *) (pBuffer->content + pBuffer->offset);
    memset(pstSetCameraCfgRes, 0, msg_length_max);

    /*ҵ��ͷ��*/
    InterCommHdr_req2res(&pstSetCameraCfgRes->stInterHdr, &pstSetCameraCfgReq->stInterHdr);
    pstSetCameraCfgRes->stInterHdr.MsgType = SRV_RES_DEV_SET_CAMERA_CFG;
    /*���ݲ�*/
    memcpy(&pstSetCameraCfgRes->stDataHdr, &pstSetCameraCfgReq->stDataHdr, sizeof(ST_DataHdr));
    /*��Ӧ���*/
    if (prot_instance.callback != NULL && prot_instance.callback->on_image_parameters_set != NULL) {
        prot_image_parameters params = {0};
        prot_image_parameters_ptol(&params, &pstSetCameraCfgReq->stCameraCfg);
        // Callback.
        int err = prot_instance.callback->on_image_parameters_set(&params);
        if (err == PROT_SUCCESS) {
            pstSetCameraCfgRes->stMsgRet.lRet = YV_SUCCESS;
            pstSetCameraCfgRes->stMsgRet.lSts = 0;
        } else {
            pstSetCameraCfgRes->stMsgRet.lRet = YV_FAIL;
            pstSetCameraCfgRes->stMsgRet.lSts = 0;
        }
    } else {
        pstSetCameraCfgRes->stMsgRet.lRet = YV_FAIL;
        pstSetCameraCfgRes->stMsgRet.lSts = ERR_COMMON_DEVICE_UPSUPPORT;
    }
    /*����*/
    pstSetCameraCfgRes->stDataHdr.lDataLength = sizeof(ST_MsgRespRet);
    pstSetCameraCfgRes->stInterHdr.DataLength = sizeof(ST_DataHdr) + pstSetCameraCfgRes->stDataHdr.lDataLength;
    /*������Ӧ*/
    pBuffer->payload_length = sizeof(yv_InterCommHdr) + pstSetCameraCfgRes->stInterHdr.DataLength;
    udpx_send_data(pBuffer);

    return retval;

ERR:
    return retval;
}

/**
 * @brief ��ѯ����ͷ��������������
 *
 * @param msg
 * @param length
 *
 * @return
 * @retval {@enum SUCCESS}
 * @retval {@enum ENULL_POINTER}
 * @retval {@enum EWRONG_BUFFER}
 * @retval {@enum EMALLOC_FAILED}
 */
int prot_process_req_get_scenes(const char *msg, int length) {
    int retval = SUCCESS;

    if (msg == NULL) {
        LOGW("%s: msg is null.\n", __func__);
        retval = ENULL_POINTER;
        goto ERR;
    }
    if (length < sizeof(ST_SrvReqDevGetVideoColorScene)) {
        LOGW("%s: length is too short.\n", __func__);
        retval = EWRONG_BUFFER;
        goto ERR;
    }

    ST_SrvReqDevGetVideoColorScene *pstGetVideoColorSceneReq = (ST_SrvReqDevGetVideoColorScene *) msg;
    ST_SrvResDevGetVideoColorScene *pstGetVideoColorSceneRes = NULL;

    /**/
    int msg_length_max = sizeof(ST_SrvResDevGetVideoColorScene);

    udpx_buffer *pBuffer = udpx_get_buffer(msg_length_max);
    if (pBuffer == NULL) {
        LOGE("%s: get buffer failed.\n", __func__);
        retval = EMALLOC_FAILED;
        goto ERR;
    }
    pstGetVideoColorSceneRes = (ST_SrvResDevGetVideoColorScene *) (pBuffer->content + pBuffer->offset);
    memset(pstGetVideoColorSceneRes, 0, msg_length_max);

    /*ҵ��ͷ��*/
    InterCommHdr_req2res(&pstGetVideoColorSceneRes->stInterHdr, &pstGetVideoColorSceneReq->stInterHdr);
    pstGetVideoColorSceneRes->stInterHdr.MsgType = SRV_RES_DEV_GET_VIDEO_COLOR_SCENE;
    /*���ݲ�*/
    memcpy(&pstGetVideoColorSceneRes->stDataHdr, &pstGetVideoColorSceneReq->stDataHdr, sizeof(ST_DataHdr));
    /*��Ӧ���*/
    if (prot_instance.callback != NULL && prot_instance.callback->on_camera_scenes_get != NULL) {
        int i = 0;
        prot_camera_scene scenes[MAX_VIDEO_COLOR_SCENE_NUM];
        int number = 0;
        // Callback.
        int err = prot_instance.callback->on_camera_scenes_get(scenes, &number);
        if (err == PROT_SUCCESS) {
            for (i = 0; i < number && i < MAX_VIDEO_COLOR_SCENE_NUM; i++) {
                prot_camera_scene_ltop(&pstGetVideoColorSceneRes->astVideoColorScene[i], &scenes[i]);
            }
            pstGetVideoColorSceneRes->stMsgRet.lRet = YV_SUCCESS;
            pstGetVideoColorSceneRes->stMsgRet.lSts = 0;
        } else {
            pstGetVideoColorSceneRes->stMsgRet.lRet = YV_FAIL;
            pstGetVideoColorSceneRes->stMsgRet.lSts = 0;
        }
    } else {
        pstGetVideoColorSceneRes->stMsgRet.lRet = YV_FAIL;
        pstGetVideoColorSceneRes->stMsgRet.lSts = ERR_COMMON_DEVICE_UPSUPPORT;
    }
    /*����*/
    pstGetVideoColorSceneRes->stDataHdr.lDataLength = sizeof(ST_MsgRespRet) + sizeof(ST_VideoColorScene) * MAX_VIDEO_COLOR_SCENE_NUM;
    pstGetVideoColorSceneRes->stInterHdr.DataLength = sizeof(ST_DataHdr) + pstGetVideoColorSceneRes->stDataHdr.lDataLength;
    /*������Ӧ*/
    pBuffer->payload_length = sizeof(yv_InterCommHdr) + pstGetVideoColorSceneRes->stInterHdr.DataLength;
    udpx_send_data(pBuffer);

    return retval;

ERR:
    return retval;
}

/**
 * @brief ��������ͷ��������������
 *
 * @param msg
 * @param length
 *
 * @return
 * @retval {@enum SUCCESS}
 * @retval {@enum ENULL_POINTER}
 * @retval {@enum EWRONG_BUFFER}
 * @retval {@enum EMALLOC_FAILED}
 */
int prot_process_req_set_scene(const char *msg, int length) {
    int retval = SUCCESS;

    if (msg == NULL) {
        LOGW("%s: msg is null.\n", __func__);
        retval = ENULL_POINTER;
        goto ERR;
    }
    if (length < sizeof(ST_SrvReqDevSevVideoColorScene)) {
        LOGW("%s: length is too short.\n", __func__);
        retval = EWRONG_BUFFER;
        goto ERR;
    }

    ST_SrvReqDevSevVideoColorScene *pstSetVideoColorSceneReq = (ST_SrvReqDevSevVideoColorScene *) msg;
    ST_SrvResDevSevVideoColorScene *pstSetVideoColorSceneRes = NULL;

    /**/
    int msg_length_max = sizeof(ST_SrvResDevSevVideoColorScene);

    udpx_buffer *pBuffer = udpx_get_buffer(msg_length_max);
    if (pBuffer == NULL) {
        LOGE("%s: get buffer failed.\n", __func__);
        retval = EMALLOC_FAILED;
        goto ERR;
    }
    pstSetVideoColorSceneRes = (ST_SrvResDevSevVideoColorScene *) (pBuffer->content + pBuffer->offset);
    memset(pstSetVideoColorSceneRes, 0, msg_length_max);

    /*ҵ��ͷ��*/
    InterCommHdr_req2res(&pstSetVideoColorSceneRes->stInterHdr, &pstSetVideoColorSceneReq->stInterHdr);
    pstSetVideoColorSceneRes->stInterHdr.MsgType = SRV_RES_DEV_SET_VIDEO_COLOR_SCENE;
    /*���ݲ�*/
    memcpy(&pstSetVideoColorSceneRes->stDataHdr, &pstSetVideoColorSceneReq->stDataHdr, sizeof(ST_DataHdr));
    /*��Ӧ���*/
    if (prot_instance.callback != NULL && prot_instance.callback->on_camera_scenes_set != NULL) {
        prot_camera_scene scene = {0};
        prot_camera_scene_ptol(&scene, &pstSetVideoColorSceneReq->stVideoColorScene);
        // Callback.
        int err = prot_instance.callback->on_camera_scenes_set(&scene);
        if (err == PROT_SUCCESS) {
            pstSetVideoColorSceneRes->stMsgRet.lRet = YV_SUCCESS;
            pstSetVideoColorSceneRes->stMsgRet.lSts = 0;
        } else {
            pstSetVideoColorSceneRes->stMsgRet.lRet = YV_FAIL;
            pstSetVideoColorSceneRes->stMsgRet.lSts = 0;
        }
    } else {
        pstSetVideoColorSceneRes->stMsgRet.lRet = YV_FAIL;
        pstSetVideoColorSceneRes->stMsgRet.lSts = ERR_COMMON_DEVICE_UPSUPPORT;
    }
    /*����*/
    pstSetVideoColorSceneRes->stDataHdr.lDataLength = sizeof(ST_MsgRespRet);
    pstSetVideoColorSceneRes->stInterHdr.DataLength = sizeof(ST_DataHdr) + pstSetVideoColorSceneRes->stDataHdr.lDataLength;
    /*������Ӧ*/
    pBuffer->payload_length = sizeof(yv_InterCommHdr) + pstSetVideoColorSceneRes->stInterHdr.DataLength;
    udpx_send_data(pBuffer);

    return retval;

ERR:
    return retval;
}

/**
 * @brief Ӧ������ͷ��������������
 *
 * @param msg
 * @param length
 *
 * @return
 * @retval {@enum SUCCESS}
 * @retval {@enum ENULL_POINTER}
 * @retval {@enum EWRONG_BUFFER}
 * @retval {@enum EMALLOC_FAILED}
 */
int prot_process_req_apply_scene(const char *msg, int length) {
    int retval = SUCCESS;

    if (msg == NULL) {
        LOGW("%s: msg is null.\n", __func__);
        retval = ENULL_POINTER;
        goto ERR;
    }
    if (length < sizeof(ST_SrvReqDevSetVideoColorScene)) {
        LOGW("%s: length is too short.\n", __func__);
        retval = EWRONG_BUFFER;
        goto ERR;
    }

    ST_SrvReqDevSetVideoColorScene *pstApplyVideoColorSceneReq = (ST_SrvReqDevSetVideoColorScene *) msg;
    ST_SrvResDevSetVideoColorScene *pstApplyVideoColorSceneRes = NULL;

    /**/
    int msg_length_max = sizeof(ST_SrvResDevSetVideoColorScene);

    udpx_buffer *pBuffer = udpx_get_buffer(msg_length_max);
    if (pBuffer == NULL) {
        LOGE("%s: get buffer failed.\n", __func__);
        retval = EMALLOC_FAILED;
        goto ERR;
    }
    pstApplyVideoColorSceneRes = (ST_SrvResDevSetVideoColorScene *) (pBuffer->content + pBuffer->offset);
    memset(pstApplyVideoColorSceneRes, 0, msg_length_max);

    /*ҵ��ͷ��*/
    InterCommHdr_req2res(&pstApplyVideoColorSceneRes->stInterHdr, &pstApplyVideoColorSceneReq->stInterHdr);
    pstApplyVideoColorSceneRes->stInterHdr.MsgType = SRV_RES_DEV_USE_VIDEO_COLOR_SCENE;
    /*���ݲ�*/
    memcpy(&pstApplyVideoColorSceneRes->stDataHdr, &pstApplyVideoColorSceneReq->stDataHdr, sizeof(ST_DataHdr));
    /*��Ӧ���*/
    if (prot_instance.callback != NULL && prot_instance.callback->on_camera_scenes_apply != NULL) {
        // Callback.
        int err = prot_instance.callback->on_camera_scenes_apply(pstApplyVideoColorSceneReq->lSceneID);
        if (err == PROT_SUCCESS) {
            pstApplyVideoColorSceneRes->stMsgRet.lRet = YV_SUCCESS;
            pstApplyVideoColorSceneRes->stMsgRet.lSts = 0;
        } else {
            pstApplyVideoColorSceneRes->stMsgRet.lRet = YV_FAIL;
            pstApplyVideoColorSceneRes->stMsgRet.lSts = 0;
        }
    } else {
        pstApplyVideoColorSceneRes->stMsgRet.lRet = YV_FAIL;
        pstApplyVideoColorSceneRes->stMsgRet.lSts = ERR_COMMON_DEVICE_UPSUPPORT;
    }
    /*����*/
    pstApplyVideoColorSceneRes->stDataHdr.lDataLength = sizeof(ST_MsgRespRet);
    pstApplyVideoColorSceneRes->stInterHdr.DataLength = sizeof(ST_DataHdr) + pstApplyVideoColorSceneRes->stDataHdr.lDataLength;
    /*������Ӧ*/
    pBuffer->payload_length = sizeof(yv_InterCommHdr) + pstApplyVideoColorSceneRes->stInterHdr.DataLength;
    udpx_send_data(pBuffer);

    return retval;

ERR:
    return retval;
}

