/*************************************************************************
	> File Name: protocol.h
	> Author: 
	> Mail: 
	> Created Time: Wed 26 Sep 2018 02:43:24 PM CST
 ************************************************************************/

#ifndef _PROTOCOL_H
#define _PROTOCOL_H

#include <pthread.h>
#include <time.h>

#define DEVICE_ID_MIN (1111100000)
#define DEVICE_ID_MAX (1111199999)

/**************************************************
 * API��������.
 **************************************************/

/**
 * �豸�걸��Ϣ.
 */
typedef struct _prot_device_info {
    /**
     * �豸ID.
     * {@val DEVICE_ID_MIN}
     * {@val DEVICE_ID_MAX}
     */
    unsigned long id;
} prot_device_info;

#define IP_BUFFER_LENGTH_MAX (20)

/**
 * �������걸��Ϣ.
 */
typedef struct _prot_server_info {
    /**
     * ������IP.
     * "172.20.1.222\0"
     */
    char ip[IP_BUFFER_LENGTH_MAX];

    /**
     * �������˿ں�.
     */
    unsigned short port;
} prot_server_info;

#define PROT_ENCODE_TYPE_UNKNOWN (0)
/*AUDIO*/
#define PROT_ENCODE_TYPE_OPUS (0x0001)
/*VIDEO*/
#define PROT_ENCODE_TYPE_H264 (0x0101)
#define PROT_ENCODE_TYPE_H265 (0x0102)

/**
 * ��Ƶ�����걸��Ϣ.
 */
typedef struct _prot_audio_info {
    /**
     * ������.
     * ��λ: Hz.
     */
    int sample_rate;

    /**
     * ͨ����.
     */
    int channel_count;

    /**
     * ����λ��.
     * ��λ: bit.
     */
    int quantization;

    /**
     * ����.
     * ��λ: bps.
     */
    int bitrate;

    /**
     * ��������.
     * {@val PROT_ENCODE_TYPE_OPUS}
     */
    int encode_type;
} prot_audio_info;

/**
 * ��Ƶ�����걸��Ϣ.
 */
typedef struct _prot_video_info {
    /**
     * ��.
     */
    int width;

    /**
     * ��.
     */
    int height;

    /**
     * ֡��.
     */
    int frame_rate;
    
    /**
     * ����.
     * ��λ: bps.
     */
    int bitrate;

    /**
     * ��������.
     * {@val PROT_ENCODE_TYPE_H264}
     */
    int encode_type;
} prot_video_info;

/**
 * �豸����״̬.
 */
typedef struct _prot_device_state {
    /**
     * ��ص�ѹ.
     * ��λ: cV(���, �ٷ�֮һ��).
     */
    int voltage;
    /**
     * �����ٷֱ�.
     */
    int capacity;
    /**
     * �豸�¶�.
     * ���ȼ�: CPU�¶� > ����¶�.
     * ��λ: ���϶�.
     */
    int temperature;
    /**
     * CPUʹ�ðٷֱ�.
     */
    int cpu_usage;
    /**
     * �ڴ�ʣ����ÿռ�.
     * ��λ: MB.
     */
    int memory_available;
    /**
     * ����ʣ����ÿռ�.
     * ��λ: MB.
     */
    int storage_available;
    /**
     * ����ʱ��.
     * ��λ: s.
     */
    unsigned int run_time;
} prot_device_state;

/**
 * λ����Ϣ.
 */
typedef struct _prot_location_info {
    /**
     * λ�òɼ�ʱ���.
     * ��λ: s.
     */
    time_t utc;
    /**
     * ����: ��.
     * ȡֵ��Χ: -179~179.
     * ��ֵ: ����
     * ��ֵ: ����
     */
    int longitude_degree;
    /**
     * ����: ��.
     * ȡֵ��Χ: 0~59.
     */
    unsigned short longitude_minute;
    /**
     * ����: ��.
     * ȡֵ��Χ: 0~59.
     */
    unsigned short longitude_second;
    /**
     * γ��: ��.
     * ȡֵ��Χ: -89~89.
     * ��ֵ: ��γ
     * ��ֵ: ��γ
     */
    int latitude_degree;
    /**
     * γ��: ��.
     * ȡֵ��Χ: 0~59.
     */
    unsigned short latitude_minute;
    /**
     * γ��: ��.
     * ȡֵ��Χ: 0~59.
     */
    unsigned short latitude_second;
    /**
     * ��������.
     * ȡֵ��Χ: 0~9999.
     * ��λ: ʮ��֮һ����/Сʱ.
     */
    unsigned int speed;
    /**
     * ���溽��: ��.
     * ���汱Ϊ�ο���׼.
     * ȡֵ��Χ: 0~359.
     */
    unsigned short direction_degree;
    /**
     * ���溽��: ��.
     * ���汱Ϊ�ο���׼.
     * ȡֵ��Χ: 0~59.
     */
    unsigned short direction_minute;
} prot_location_info;

/**************************************************
 * Callback/��������.
 **************************************************/

typedef enum _prot_result {
    PROT_SUCCESS = 0,
    PROT_FAILED = -1,
} prot_result;

/**
 * ��¼ʧ�ܴ�����.
 */
typedef enum _prot_login_errno {
    /**
     * δ֪����.
     */
    LE_UNKNOWN = -1,
    /**
     * ��Ч�ʺ�.
     */
    LE_INVALID_ACCOUNT = -2,
    /**
     * �������.
     */
    LE_WRONG_PASSWD = -3,
    /**
     * ��֤��ʱ.
     */
    LE_TIMEOUT = -4,
    /**
     * �ظ���¼.
     */
    LE_RELOGIN = -5,
    /**
     * ���������û�����.
     */
    LE_BEYOND_UPPER_LIMIT = -6,
} prot_login_errno;

/**
 * {@func on_video_parameters_change()}�Ĵ�ȡ��.
 */
typedef enum _prot_on_video_parameters_change_errno {
    /**
     * �ɹ�.
     */
    VPCE_SUCCESS = 0,
    /**
     * ������֧��.
     */
    VPCE_UNSUPPORTED = -1,
    /**
     * �豸æ.
     */
    VPCE_BUSY = -2,
    /**
     * ��Ƶ�ɼ��쳣.
     */
    VPCE_CAPTURER_EXCEPTION = -3,
    /**
     * ��Ƶ�����쳣.
     */
    VPCE_ENCODER_EXCEPTION = -4,
} prot_on_video_parameters_change_errno;

/**
 * ��Ƶ����.
 */
typedef struct _prot_video_parameters {
    /**
     * ��Ƶ��.
     */
    int width;
    /**
     * ��Ƶ��.
     */
    int height;
    /**
     * ֡��.
     */
    int framerate;
    /**
     * ����.
     * ��λ: bps.
     */
    int bitrate;
} prot_video_parameters;

/**
 * ����ͷSensor����.
 */
typedef struct _prot_camera_parameters {
    /**
     * ����ǧ�ֱ�.
     */
    unsigned int brightness;
    /**
     * �Աȶ�ǧ�ֱ�.
     */
    unsigned int contrast;
    /**
     * ���Ͷ�ǧ�ֱ�.
     */
    unsigned int saturation;
    /**
     * ɫ��ǧ�ֱ�.
     */
    unsigned int hue;
    /**
     * ���ǧ�ֱ�.
     */
    unsigned int sharpness;
    /**
     * �ع��ǧ�ֱ�.
     */
    unsigned int exposure;
} prot_camera_parameters;

/**
 * ͼ��˳ʱ����ת�Ƕ�ö��ֵ.
 */
typedef enum _prot_rotation {
    PROT_ROTATION_0 = 0,
    PROT_ROTATION_90 = 1,
    PROT_ROTATION_180 = 2,
    PROT_ROTATION_270 = 3,
} prot_rotation;

/**
 * ͼ��תö��ֵ.
 */
typedef enum _prot_flip {
    PROT_FLIP_NONE = 0,
    PROT_FLIP_VERTICAL = 1,
    PROT_FLIP_HORIZONTAL = 2,
} prot_flip;

/**
 * ������ö��ֵ.
 */
typedef enum _prot_switch {
    PROT_OFF = 0,
    PROT_ON = 1,
} prot_switch;

/**
 * ����ͷͼ�����.
 */
typedef struct _prot_image_parameters {
    /**
     * ͼ����ת�Ƕ�ö��ֵ.
     */
    prot_rotation rotation;
    /**
     * ͼ��תö��ֵ.
     */
    prot_flip flip;
    /**
     * ����ͷ����忪��.
     * {@enum PROT_OFF} �ر�
     * {@enum PROT_ON}  ��
     */
    prot_switch light_buffle_switch;
    /**
     * ��ɫģʽ.
     * {@enum PROT_OFF} �ڰ�ģʽ
     * {@enum PROT_ON}  ��ɫģʽ
     */
    prot_switch colour_mode;
    /**
     * ����ģʽ.
     * {@enum PROT_OFF} ����ģʽ
     * {@enum PROT_ON}  ����ģʽ
     */
    prot_switch mirror_mode;
} prot_image_parameters;

#define PROT_CAMERA_SCENE_DESCRIBER_LENGTH_MAX (32) /*{@val MAX_SCENE_DESCRIBE_COUNT}*/
#define PROT_CAMERA_SCENE_NUMBER_MAX 3 /*{@val MAX_VIDEO_COLOR_SCENE_NUM}*/

/**
 * ����ͷ��������.
 */
typedef struct _prot_camera_scene {
    /**
     * �������.
     */
    int number;
    /**
     * ��������.
     */
    char describer[PROT_CAMERA_SCENE_DESCRIBER_LENGTH_MAX];
    /**
     * ��������.
     */
    prot_camera_parameters params;
} prot_camera_scene;

/**
 * �ص�������.
 */
typedef struct _prot_callback {
    /**
     * @brief ��¼ʧ�ܻص�
     *
     * @param errno
     */
    void (*on_login_failed)(prot_login_errno errno);
    /**
     * @brief �豸���߻ص�
     */
    void (*on_online)();
    /**
     * @brief �豸���߻ص�
     */
    void (*on_offline)();
    /**
     * @brief ��ʼ�ϴ��ص�
     */
    void (*on_stream_start)();
    /**
     * @brief ֹͣ�ϴ��ص�
     */
    void (*on_stream_stop)();
    /**
     * @brief ������Ƶ�����ص�
     *
     * @param params
     */
    prot_on_video_parameters_change_errno (*on_video_parameters_change)(prot_video_parameters *params);
    /**
     * @brief ��ȡ����ͷSensor�����ص�
     *
     * @param params    �ص�ʵ�������ýṹ��
     *
     * @return
     * @retval {@enum PROT_SUCCESS}
     * @retval {@enum PROT_FAILED}
     */
    int (*on_camera_parameters_get)(prot_camera_parameters *params);
    /**
     * @brief ��������ͷSensor�����ص�
     *
     * @param params
     *
     * @return
     * @retval {@enum PROT_SUCCESS}
     * @retval {@enum PROT_FAILED}
     */
    int (*on_camera_parameters_set)(prot_camera_parameters *params);
    /**
     * @brief ��������ͷSensor�����ص�
     *
     * @return
     * @retval {@enum PROT_SUCCESS}
     * @retval {@enum PROT_FAILED}
     */
    int (*on_camera_parameters_reset)();
    /**
     * @brief ��ȡ����ͷͼ������ص�
     *
     * @param params    �ص�ʵ�������ýṹ��
     *
     * @return
     * @retval {@enum PROT_SUCCESS}
     * @retval {@enum PROT_FAILED}
     */
    int (*on_image_parameters_get)(prot_image_parameters *params);
    /**
     * @brief ��������ͷͼ������ص�
     *
     * @param params
     *
     * @return
     * @retval {@enum PROT_SUCCESS}
     * @retval {@enum PROT_FAILED}
     */
    int (*on_image_parameters_set)(prot_image_parameters *params);
    /**
     * @brief ��ȡ����ͷ���������ص�
     *
     * @param scenes    �ص�ʵ�������ýṹ������
     * @param number    �ص�ʵ���и�ֵ��������
     *          ȡֵ��Χ: 0~{@val PROT_CAMERA_SCENE_NUMBER_MAX}
     *
     * @return
     * @retval {@enum PROT_SUCCESS}
     * @retval {@enum PROT_FAILED}
     */
    int (*on_camera_scenes_get)(prot_camera_scene *scenes, int *number);
    /**
     * @brief ��������ͷ���������ص�
     *
     * @param scene
     *
     * @return
     * @retval {@enum PROT_SUCCESS}
     * @retval {@enum PROT_FAILED}
     */
    int (*on_camera_scenes_set)(prot_camera_scene *scene);
    /**
     * @brief Ӧ������ͷ���������ص�
     *
     * @param number
     *
     * @return
     * @retval {@enum PROT_SUCCESS}
     * @retval {@enum PROT_FAILED}
     */
    int (*on_camera_scenes_apply)(int number);
} prot_callback;

/**************************************************
 * �ڲ�����.
 **************************************************/

#define PROT_STATE_INVALID (0)
#define PROT_STATE_VALID (1)

#define PROT_FLAG_LOGIN (1)
#define PROT_FLAG_LOGOUT (2)

#define PROT_TURE (1)
#define PROT_FALSE (0)

#define PROT_STREAM_FLAG_UPLOAD (0x00000001)
#define PROT_STREAM_FLAG_RECORD (0x00000002)

/**
 * �Ự�걸��Ϣ.
 */
typedef struct _prot_session_info {
    /**
     * �ỰID.
     */
    unsigned int id;
    /**
     * Դ������ID.
     */
    unsigned int src_server_id;
    /**
     * ԴID.
     */
    unsigned int src_object_id;
    /**
     * Ŀ�������ID.
     */
    unsigned int dst_server_id;
    /**
     * Ŀ��ID.
     */
    unsigned int dst_object_id;
} prot_session_info;

/**
 * Э������걸��Ϣ.
 */
typedef struct _prot_info {
    /**
     * ������.
     */
    pthread_mutex_t mutex;

    /**
     * ״̬.
     * ��ʶ{@func prot_init}, {@func prot_fini}����״̬.
     * {@val PROT_STATE_INVALID}
     * {@val PROT_STATE_VALID}
     */
    int state;

    /**
     * �豸�걸��Ϣ.
     */
    prot_device_info device;

    /**
     * �������걸��Ϣ.
     */
    prot_server_info server;

    /**
     * ��Ƶ�����걸��Ϣ.
     */
    prot_audio_info audio;
    /**
     * ��Ƶ��ID.
     */
    int audio_stream_id;

    /**
     * ��Ƶ�����걸��Ϣ.
     */
    prot_video_info video;
    /**
     * ��Ƶ��ID.
     */
    int video_stream_id;

    /**
     * ���ϴ���ʶ.
     * ���±�ʶ���:
     * {@val PROT_STREAM_FLAG_UPLOAD}
     * {@val PROT_STREAM_FLAG_RECORD}
     */
    int stream_upload_flag;

    /**
     * �Ự�걸��Ϣ.
     */
    prot_session_info session;

    /**
     * �ص�������.
     */
    prot_callback *callback;

    /**
     * ״̬����߳�ID.
     * ���߱���.
     * �����ص�¼.
     */
    pthread_t monitor_thread_pid;

    /**
     * ��Ϣ�����߳�ID.
     */
    pthread_t msg_recv_thread_pid;

    /**
     * ��¼��ʶ.
     * ��ʶ{@func prot_login}, {@func prot_logout}����״̬.
     * {@val PROT_FLAG_LOGOUT} : ����Ҫ��¼
     * {@val PROT_FLAG_LOGIN} : ��Ҫ��¼
     */
    int login_flag;

    /**
     * �Ƿ�����.
     * �������߱���/�����ص�¼.
     */
    int is_online;

    /**
     * ����ʱ���.
     * ��λ: ms.
     */
    unsigned long keep_alive_timestamp;
} prot_info;

/**************************************************
 * ��������.
 **************************************************/

#define PROT_FRAME_I (1)
#define PROT_FRAME_P (2)
#define PROT_FRAME_B (3)

#ifdef __cplusplus
extern "C" {
#endif
/**************************************************
 * API.
 **************************************************/
    void prot_set_device_info(prot_device_info *device);
    void prot_set_server_info(prot_server_info *server);
    void prot_set_callback(prot_callback *callback);
    int prot_init();
    int prot_fini();
    int prot_login();
    int prot_logout();
    int prot_update_audio_info(prot_audio_info *audio);
    int prot_update_video_info(prot_video_info *video);
    int prot_audio_frame_send(const char *buffer, int length, long timestamp);
    int prot_video_frame_send(const char *buffer, int length, int frame_type, long timestamp);
    int prot_device_state_send(prot_device_state *state, prot_location_info *location);

/**************************************************
 * �ڲ�ʹ��.
 **************************************************/
    int prot_keep_alive();

    /**
     * Э����Ϣ����.
     */
    int prot_process_res_login(const char *msg, int length);
    int prot_process_res_keep_alive(const char *msg, int length);
    int prot_process_req_stream_info(const char *msg, int length);
    int prot_process_nfy_start_stream(const char *msg, int length);
    int prot_process_nfy_stop_stream(const char *msg, int length);
    int prot_process_req_change_video_parameters(const char *msg, int length);

    int prot_process_req_get_camera_parameters(const char *msg, int length);
    int prot_process_req_set_camera_parameters(const char *msg, int length);
    int prot_process_req_reset_camera_parameters(const char *msg, int length);
    int prot_process_req_get_image_parameters(const char *msg, int length);
    int prot_process_req_set_image_parameters(const char *msg, int length);
    int prot_process_req_get_scenes(const char *msg, int length);
    int prot_process_req_set_scene(const char *msg, int length);
    int prot_process_req_apply_scene(const char *msg, int length);
#ifdef __cplusplus
}
#endif

#endif
