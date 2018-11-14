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
 * API参数定义.
 **************************************************/

/**
 * 设备完备信息.
 */
typedef struct _prot_device_info {
    /**
     * 设备ID.
     * {@val DEVICE_ID_MIN}
     * {@val DEVICE_ID_MAX}
     */
    unsigned long id;
} prot_device_info;

#define IP_BUFFER_LENGTH_MAX (20)

/**
 * 服务器完备信息.
 */
typedef struct _prot_server_info {
    /**
     * 服务器IP.
     * "172.20.1.222\0"
     */
    char ip[IP_BUFFER_LENGTH_MAX];

    /**
     * 服务器端口号.
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
 * 音频参数完备信息.
 */
typedef struct _prot_audio_info {
    /**
     * 采样率.
     * 单位: Hz.
     */
    int sample_rate;

    /**
     * 通道数.
     */
    int channel_count;

    /**
     * 量化位数.
     * 单位: bit.
     */
    int quantization;

    /**
     * 码率.
     * 单位: bps.
     */
    int bitrate;

    /**
     * 编码类型.
     * {@val PROT_ENCODE_TYPE_OPUS}
     */
    int encode_type;
} prot_audio_info;

/**
 * 视频参数完备信息.
 */
typedef struct _prot_video_info {
    /**
     * 宽.
     */
    int width;

    /**
     * 高.
     */
    int height;

    /**
     * 帧率.
     */
    int frame_rate;
    
    /**
     * 码率.
     * 单位: bps.
     */
    int bitrate;

    /**
     * 编码类型.
     * {@val PROT_ENCODE_TYPE_H264}
     */
    int encode_type;
} prot_video_info;

/**
 * 设备运行状态.
 */
typedef struct _prot_device_state {
    /**
     * 电池电压.
     * 单位: cV(厘伏, 百分之一伏).
     */
    int voltage;
    /**
     * 电量百分比.
     */
    int capacity;
    /**
     * 设备温度.
     * 优先级: CPU温度 > 电池温度.
     * 单位: 摄氏度.
     */
    int temperature;
    /**
     * CPU使用百分比.
     */
    int cpu_usage;
    /**
     * 内存剩余可用空间.
     * 单位: MB.
     */
    int memory_available;
    /**
     * 磁盘剩余可用空间.
     * 单位: MB.
     */
    int storage_available;
    /**
     * 运行时长.
     * 单位: s.
     */
    unsigned int run_time;
} prot_device_state;

/**
 * 位置信息.
 */
typedef struct _prot_location_info {
    /**
     * 位置采集时间戳.
     * 单位: s.
     */
    time_t utc;
    /**
     * 经度: 度.
     * 取值范围: -179~179.
     * 正值: 东经
     * 负值: 西经
     */
    int longitude_degree;
    /**
     * 经度: 分.
     * 取值范围: 0~59.
     */
    unsigned short longitude_minute;
    /**
     * 经度: 秒.
     * 取值范围: 0~59.
     */
    unsigned short longitude_second;
    /**
     * 纬度: 度.
     * 取值范围: -89~89.
     * 正值: 北纬
     * 负值: 南纬
     */
    int latitude_degree;
    /**
     * 纬度: 分.
     * 取值范围: 0~59.
     */
    unsigned short latitude_minute;
    /**
     * 纬度: 秒.
     * 取值范围: 0~59.
     */
    unsigned short latitude_second;
    /**
     * 地面速率.
     * 取值范围: 0~9999.
     * 单位: 十分之一海里/小时.
     */
    unsigned int speed;
    /**
     * 地面航向: 度.
     * 以真北为参考基准.
     * 取值范围: 0~359.
     */
    unsigned short direction_degree;
    /**
     * 地面航向: 分.
     * 以真北为参考基准.
     * 取值范围: 0~59.
     */
    unsigned short direction_minute;
} prot_location_info;

/**************************************************
 * Callback/参数定义.
 **************************************************/

typedef enum _prot_result {
    PROT_SUCCESS = 0,
    PROT_FAILED = -1,
} prot_result;

/**
 * 登录失败错误码.
 */
typedef enum _prot_login_errno {
    /**
     * 未知错误.
     */
    LE_UNKNOWN = -1,
    /**
     * 无效帐号.
     */
    LE_INVALID_ACCOUNT = -2,
    /**
     * 密码错误.
     */
    LE_WRONG_PASSWD = -3,
    /**
     * 认证超时.
     */
    LE_TIMEOUT = -4,
    /**
     * 重复登录.
     */
    LE_RELOGIN = -5,
    /**
     * 超过在线用户上限.
     */
    LE_BEYOND_UPPER_LIMIT = -6,
} prot_login_errno;

/**
 * {@func on_video_parameters_change()}的错取码.
 */
typedef enum _prot_on_video_parameters_change_errno {
    /**
     * 成功.
     */
    VPCE_SUCCESS = 0,
    /**
     * 参数不支持.
     */
    VPCE_UNSUPPORTED = -1,
    /**
     * 设备忙.
     */
    VPCE_BUSY = -2,
    /**
     * 视频采集异常.
     */
    VPCE_CAPTURER_EXCEPTION = -3,
    /**
     * 视频编码异常.
     */
    VPCE_ENCODER_EXCEPTION = -4,
} prot_on_video_parameters_change_errno;

/**
 * 视频参数.
 */
typedef struct _prot_video_parameters {
    /**
     * 视频宽.
     */
    int width;
    /**
     * 视频高.
     */
    int height;
    /**
     * 帧率.
     */
    int framerate;
    /**
     * 码率.
     * 单位: bps.
     */
    int bitrate;
} prot_video_parameters;

/**
 * 摄像头Sensor参数.
 */
typedef struct _prot_camera_parameters {
    /**
     * 亮度千分比.
     */
    unsigned int brightness;
    /**
     * 对比度千分比.
     */
    unsigned int contrast;
    /**
     * 饱和度千分比.
     */
    unsigned int saturation;
    /**
     * 色度千分比.
     */
    unsigned int hue;
    /**
     * 锐度千分比.
     */
    unsigned int sharpness;
    /**
     * 曝光度千分比.
     */
    unsigned int exposure;
} prot_camera_parameters;

/**
 * 图像顺时针旋转角度枚举值.
 */
typedef enum _prot_rotation {
    PROT_ROTATION_0 = 0,
    PROT_ROTATION_90 = 1,
    PROT_ROTATION_180 = 2,
    PROT_ROTATION_270 = 3,
} prot_rotation;

/**
 * 图像翻转枚举值.
 */
typedef enum _prot_flip {
    PROT_FLIP_NONE = 0,
    PROT_FLIP_VERTICAL = 1,
    PROT_FLIP_HORIZONTAL = 2,
} prot_flip;

/**
 * 开关量枚举值.
 */
typedef enum _prot_switch {
    PROT_OFF = 0,
    PROT_ON = 1,
} prot_switch;

/**
 * 摄像头图像参数.
 */
typedef struct _prot_image_parameters {
    /**
     * 图像旋转角度枚举值.
     */
    prot_rotation rotation;
    /**
     * 图像翻转枚举值.
     */
    prot_flip flip;
    /**
     * 摄像头挡光板开关.
     * {@enum PROT_OFF} 关闭
     * {@enum PROT_ON}  打开
     */
    prot_switch light_buffle_switch;
    /**
     * 彩色模式.
     * {@enum PROT_OFF} 黑白模式
     * {@enum PROT_ON}  彩色模式
     */
    prot_switch colour_mode;
    /**
     * 镜像模式.
     * {@enum PROT_OFF} 正常模式
     * {@enum PROT_ON}  镜像模式
     */
    prot_switch mirror_mode;
} prot_image_parameters;

#define PROT_CAMERA_SCENE_DESCRIBER_LENGTH_MAX (32) /*{@val MAX_SCENE_DESCRIBE_COUNT}*/
#define PROT_CAMERA_SCENE_NUMBER_MAX 3 /*{@val MAX_VIDEO_COLOR_SCENE_NUM}*/

/**
 * 摄像头场景参数.
 */
typedef struct _prot_camera_scene {
    /**
     * 场景序号.
     */
    int number;
    /**
     * 场景描述.
     */
    char describer[PROT_CAMERA_SCENE_DESCRIBER_LENGTH_MAX];
    /**
     * 场景参数.
     */
    prot_camera_parameters params;
} prot_camera_scene;

/**
 * 回调函数集.
 */
typedef struct _prot_callback {
    /**
     * @brief 登录失败回调
     *
     * @param errno
     */
    void (*on_login_failed)(prot_login_errno errno);
    /**
     * @brief 设备上线回调
     */
    void (*on_online)();
    /**
     * @brief 设备下线回调
     */
    void (*on_offline)();
    /**
     * @brief 开始上传回调
     */
    void (*on_stream_start)();
    /**
     * @brief 停止上传回调
     */
    void (*on_stream_stop)();
    /**
     * @brief 调整视频参数回调
     *
     * @param params
     */
    prot_on_video_parameters_change_errno (*on_video_parameters_change)(prot_video_parameters *params);
    /**
     * @brief 获取摄像头Sensor参数回调
     *
     * @param params    回调实现中填充该结构体
     *
     * @return
     * @retval {@enum PROT_SUCCESS}
     * @retval {@enum PROT_FAILED}
     */
    int (*on_camera_parameters_get)(prot_camera_parameters *params);
    /**
     * @brief 设置摄像头Sensor参数回调
     *
     * @param params
     *
     * @return
     * @retval {@enum PROT_SUCCESS}
     * @retval {@enum PROT_FAILED}
     */
    int (*on_camera_parameters_set)(prot_camera_parameters *params);
    /**
     * @brief 重置摄像头Sensor参数回调
     *
     * @return
     * @retval {@enum PROT_SUCCESS}
     * @retval {@enum PROT_FAILED}
     */
    int (*on_camera_parameters_reset)();
    /**
     * @brief 获取摄像头图像参数回调
     *
     * @param params    回调实现中填充该结构体
     *
     * @return
     * @retval {@enum PROT_SUCCESS}
     * @retval {@enum PROT_FAILED}
     */
    int (*on_image_parameters_get)(prot_image_parameters *params);
    /**
     * @brief 设置摄像头图像参数回调
     *
     * @param params
     *
     * @return
     * @retval {@enum PROT_SUCCESS}
     * @retval {@enum PROT_FAILED}
     */
    int (*on_image_parameters_set)(prot_image_parameters *params);
    /**
     * @brief 获取摄像头场景参数回调
     *
     * @param scenes    回调实现中填充该结构体数组
     * @param number    回调实现中赋值场景个数
     *          取值范围: 0~{@val PROT_CAMERA_SCENE_NUMBER_MAX}
     *
     * @return
     * @retval {@enum PROT_SUCCESS}
     * @retval {@enum PROT_FAILED}
     */
    int (*on_camera_scenes_get)(prot_camera_scene *scenes, int *number);
    /**
     * @brief 设置摄像头场景参数回调
     *
     * @param scene
     *
     * @return
     * @retval {@enum PROT_SUCCESS}
     * @retval {@enum PROT_FAILED}
     */
    int (*on_camera_scenes_set)(prot_camera_scene *scene);
    /**
     * @brief 应用摄像头场景参数回调
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
 * 内部定义.
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
 * 会话完备信息.
 */
typedef struct _prot_session_info {
    /**
     * 会话ID.
     */
    unsigned int id;
    /**
     * 源服务器ID.
     */
    unsigned int src_server_id;
    /**
     * 源ID.
     */
    unsigned int src_object_id;
    /**
     * 目标服务器ID.
     */
    unsigned int dst_server_id;
    /**
     * 目标ID.
     */
    unsigned int dst_object_id;
} prot_session_info;

/**
 * 协议相关完备信息.
 */
typedef struct _prot_info {
    /**
     * 互斥锁.
     */
    pthread_mutex_t mutex;

    /**
     * 状态.
     * 标识{@func prot_init}, {@func prot_fini}调用状态.
     * {@val PROT_STATE_INVALID}
     * {@val PROT_STATE_VALID}
     */
    int state;

    /**
     * 设备完备信息.
     */
    prot_device_info device;

    /**
     * 服务器完备信息.
     */
    prot_server_info server;

    /**
     * 音频参数完备信息.
     */
    prot_audio_info audio;
    /**
     * 音频流ID.
     */
    int audio_stream_id;

    /**
     * 视频参数完备信息.
     */
    prot_video_info video;
    /**
     * 视频流ID.
     */
    int video_stream_id;

    /**
     * 流上传标识.
     * 以下标识组合:
     * {@val PROT_STREAM_FLAG_UPLOAD}
     * {@val PROT_STREAM_FLAG_RECORD}
     */
    int stream_upload_flag;

    /**
     * 会话完备信息.
     */
    prot_session_info session;

    /**
     * 回调函数集.
     */
    prot_callback *callback;

    /**
     * 状态监控线程ID.
     * 在线保活.
     * 掉线重登录.
     */
    pthread_t monitor_thread_pid;

    /**
     * 消息接收线程ID.
     */
    pthread_t msg_recv_thread_pid;

    /**
     * 登录标识.
     * 标识{@func prot_login}, {@func prot_logout}调用状态.
     * {@val PROT_FLAG_LOGOUT} : 不需要登录
     * {@val PROT_FLAG_LOGIN} : 需要登录
     */
    int login_flag;

    /**
     * 是否在线.
     * 用于在线保活/掉线重登录.
     */
    int is_online;

    /**
     * 保活时间戳.
     * 单位: ms.
     */
    unsigned long keep_alive_timestamp;
} prot_info;

/**************************************************
 * 函数声明.
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
 * 内部使用.
 **************************************************/
    int prot_keep_alive();

    /**
     * 协议消息处理.
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
