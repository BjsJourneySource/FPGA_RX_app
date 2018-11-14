/***************************************************************************************
 * 文 件 名 : yvcommon_srv_dev_msg_type.h
 * 项目名称 : YVGA1207001A                                                            
 * 模 块 名 : 公共头文件
 * 功    能 : 服务器与设备交互的外部消息类型定义
 * 操作系统 : LINUX                                                                  
 * 修改记录 : 
 * 版    本 : Rev 2.2.0                                                               
 *--------------------------------------------------------------------------------------
 * 设    计 : liangy      '2013-07-02                                                  
 * 编    码 : liangy      '2013-07-02
 * 修    改 :                                                                          
 ***************************************************************************************
 *- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 * 公司介绍及版权说明                                                                   
 *                                                                                      
 *               (C)Copyright 2013 YView    Corporation All Rights Reserved.            
 *- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 ***************************************************************************************/

#ifndef _YVCOMMON_SRV_DEV_MSG_TYPE_H_
#define _YVCOMMON_SRV_DEV_MSG_TYPE_H_

typedef enum
{
	/* 设备管理相关消息类型定义 */
	SRV_REQ_DEV_QRY_SERVERIP        = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x0001), /* 查询设备设置服务器地址请求 服务器->设备 */
	SRV_RES_DEV_QRY_SERVERIP        = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x0002), /* 查询设备设置服务器地址响应 设备->服务器 */

	SRV_REQ_DEV_SET_SERVERIP        = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x0003), /* 更改设备设置服务器地址请求 服务器->设备 */
	SRV_RES_DEV_SET_SERVERIP        = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x0004), /* 更改设备设置服务器地址响应 设备->服务器 */

	SRV_REQ_DEV_QRY_TRANSMODE       = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x0005), /* 查询设备传输模式请求 服务器->设备 */
	SRV_RES_DEV_QRY_TRANSMODE       = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x0006), /* 查询设备传输模式响应 设备->服务器 */

	SRV_REQ_DEV_SET_TRANSMODE       = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x0007), /* 更改设备传输模式请求 服务器->设备 */
	SRV_RES_DEV_SET_TRANSMODE       = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x0008), /* 更改设备传输模式响应 设备->服务器 */

	SRV_REQ_DEV_QRY_AUDIOPARAM      = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x0009), /* 查询设备音频参数配置请求 服务器->设备 */
	SRV_RES_DEV_QRY_AUDIOPARAM      = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x000A), /* 查询设备音频参数配置响应 设备->服务器 */

	SRV_REQ_DEV_SET_AUDIOPARAM      = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x000B), /* 更改设备音频参数配置请求 服务器->设备 */
	SRV_RES_DEV_SET_AUDIOPARAM      = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x000C), /* 更改设备音频参数配置响应 设备->服务器 */

	SRV_REQ_DEV_QRY_PHONENO         = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x000D), /* 查询设备捆绑的手机号请求 服务器->设备 */
	SRV_RES_DEV_QRY_PHONENO         = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x000E), /* 查询设备捆绑的手机号响应 设备->服务器 */

	SRV_REQ_DEV_SET_PHONENO         = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x000F), /* 锁定设备捆绑的手机号请求 服务器->设备 */
	SRV_RES_DEV_SET_PHONENO         = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x0010), /* 锁定设备捆绑的手机号响应 设备->服务器 */

	SRV_REQ_DEV_UNSET_PHONENO       = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x0011), /* 解锁设备捆绑的手机号请求 服务器->设备 */
	SRV_RES_DEV_UNSET_PHONENO       = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x0012), /* 解锁设备捆绑的手机号响应 设备->服务器 */

	SRV_REQ_DEV_QRY_ACTIVE_PHONENO  = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x0013), /* 查询设备设置的激活手机号请求 服务器->设备 */
	SRV_RES_DEV_QRY_ACTIVE_PHONENO  = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x0014), /* 查询设备设置的激活手机号响应 设备->服务器 */

	SRV_REQ_DEV_ADD_ACTIVE_PHONENO  = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x0015), /* 添加设备设置的激活手机号请求 服务器->设备 */
	SRV_RES_DEV_ADD_ACTIVE_PHONENO  = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x0016), /* 添加设备设置的激活手机号响应 设备->服务器 */

	SRV_REQ_DEV_DEL_ACTIVE_PHONENO  = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x0017), /* 删除设备设置的激活手机号请求 服务器->设备 */
	SRV_RES_DEV_DEL_ACTIVE_PHONENO  = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x0018), /* 删除设备设置的激活手机号响应 设备->服务器 */

	SRV_REQ_DEV_QRY_POWERVAL        = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x0019), /* 查询设备电池电量请求 服务器->设备 */
	SRV_RES_DEV_QRY_POWERVAL        = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x001A), /* 查询设备电池电量响应 设备->服务器 */

	SRV_REQ_DEV_QRY_INPUTTYPE       = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x001B), /* 查询设备语音输入通道请求 服务器->设备 */
	SRV_RES_DEV_QRY_INPUTTYPE       = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x001C), /* 查询设备语音输入通道响应 设备->服务器 */

	SRV_REQ_DEV_SET_INPUTTYPE       = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x001D), /* 更改设备语音输入通道请求 服务器->设备 */
	SRV_RES_DEV_SET_INPUTTYPE       = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x001E), /* 更改设备语音输入通道响应 设备->服务器 */

	SRV_REQ_DEV_QRY_CLOCK           = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x001F), /* 查询定时器配置请求 服务器->设备 */
	SRV_RES_DEV_QRY_CLOCK           = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x0020), /* 查询定时器配置响应 设备->服务器 */

	SRV_REQ_DEV_SET_CLOCK           = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x0021), /* 更改设备定时器配置请求 服务器->设备 */
	SRV_RES_DEV_SET_CLOCK           = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x0022), /* 更改设备定时器配置响应 设备->服务器 */

	SRV_REQ_DEV_QRY_LED             = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x0023), /* 查询A0设备指示灯全局配置请求 服务器->设备 */
	SRV_RES_DEV_QRY_LED             = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x0024), /* 查询A0设备指示灯全局配置响应 设备->服务器 */

	SRV_REQ_DEV_SET_LED             = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x0025), /* 更改A0设备指示灯全局配置请求 服务器->设备 */
	SRV_RES_DEV_SET_LED             = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x0026), /* 更改A0设备指示灯全局配置响应 设备->服务器 */

	SRV_REQ_DEV_QRY_ALARMVAL        = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x0027), /* 查询设备报警阈值请求 服务器->设备 */
	SRV_RES_DEV_QRY_ALARMVAL        = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x0028), /* 查询设备报警阈值响应 设备->服务器 */

	SRV_REQ_DEV_SET_ALARMVAL        = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x0029), /* 更改设备报警阈值请求 服务器->设备 */
	SRV_RES_DEV_SET_ALARMVAL        = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x002A), /* 更改设备报警阈值响应 设备->服务器 */

	SRV_REQ_DEV_QRY_ALARMEVENT      = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x002B), /* 查询设备报警联动事件请求 服务器->设备 */
	SRV_RES_DEV_QRY_ALARMEVENT      = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x002C), /* 查询设备报警联动事件响应 设备->服务器 */

	SRV_REQ_DEV_SET_ALARMEVENT      = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x002D), /* 更改设备报警联动事件请求 服务器->设备 */
	SRV_RES_DEV_SET_ALARMEVENT      = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x002E), /* 更改设备报警联动事件响应 设备->服务器 */

	DEV_NFY_SRV_DEVICE_LOGOUT       = DEF_MSG_TYPE(SRV_DEV, MSG_NFY, TYPE_DEV, 0x002F), /* 设备注销,设备->设备管理             */

	SRV_REQ_DEV_CHG_AUDIO_PARAM     = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x0030), /* 更改音频采集参数请求                */
	SRV_RES_DEV_CHG_AUDIO_PARAM     = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x0031), /* 更改音频采集参数响应                */

	SRV_REQ_DEV_CHG_VIDEO_PARAM     = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x0032), /* 更改视频采集参数请求                */
	SRV_RES_DEV_CHG_VIDEO_PARAM     = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x0033), /* 更改视频采集参数响应                */

	SRV_REQ_DEV_QUERY_CONFIG        = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x0034), /* 查询设备配置请求                    */
	SRV_RES_DEV_QUERY_CONFIG        = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x0035), /* 查询设备配置响应                    */

	SRV_REQ_DEV_SAVE_CONFIG         = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x0036), /* 保存设备配置请求                    */
	SRV_RES_DEV_SAVE_CONFIG         = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x0037), /* 保存设备配置响应                    */

	SRV_REQ_DEV_STATUS_INFO         = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x0038), /* 查询设备状态信息请求                */
	SRV_RES_DEV_STATUS_INFO         = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x0039), /* 查询设备状态信息响应                */

	SRV_REQ_DEV_STREAM_INFO         = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x003A), /* 查询设备上流信息请求消息            */
	SRV_RES_DEV_STREAM_INFO         = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x003B), /* 查询设备上流信息响应消息            */

	SRV_REQ_DEV_QUERY_SUB_INFO      = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x003C), /* 查询子设备基本信息请求*/
	SRV_RES_DEV_QUERY_SUB_INFO      = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x003C), /* 查询子设备基本信息响应*/

	SRV_REQ_DEV_UPDATE_SUB_INFO     = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x003D), /* 修改子设备基本信息请求*/
	SRV_RES_DEV_UPDATE_SUB_INFO     = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x003D), /* 修改子设备基本信息响应*/

	SRV_REQ_DEV_QUERY_SUB_DETAILS   = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x003E), /* 查询子设备详细信息请求*/
	SRV_RES_DEV_QUERY_SUB_DETAILS   = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x003E), /* 查询子设备详细信息响应*/

	SRV_REQ_DEV_UPDATE_SUB_DETAILS  = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x003F), /* 修改子设备详细信息请求*/
	SRV_RES_DEV_UPDATE_SUB_DETAILS  = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x003F), /* 修改子设备详细信息响应*/

	/*0x0040-0x004F预留,VPN消息区间*/
	/*ADD_S_by_Wangly VPN二期新增消息类型定义*/
	DEV_NFY_SRV_STATUS_INFO_EX          = DEF_MSG_TYPE(SRV_DEV, MSG_NFY, TYPE_DEV, 0x0040), /* 设备状态信息上报通知消息*/
#if 0
	SRV_REQ_DEV_DISABLE_DEVICE_CHANNEL  = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x0041), /* 关闭设备通道请求*/
	SRV_RES_DEV_DISABLE_DEVICE_CHANNEL  = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x0041), /* 关闭设备通道响应*/
	SRV_REQ_DEV_ENABLE_DEVICE_CHANNEL   = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x0042), /* 启用设备通道请求*/
	SRV_RES_DEV_ENABLE_DEVICE_CHANNEL   = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x0042), /* 启用设备通道响应*/
	SRV_REQ_DEV_SAVE_CHANNEL_CONFIG     = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x0043), /* 保存设备通道配置请求*/
	SRV_RES_DEV_SAVE_CHANNEL_CONFIG     = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x0043), /* 保存设备通道配置响应*/
#endif
	/*ADD_S_by_Wangly VPN二期新增消息类型定义*/

	/*ADD_S by liangy*/
	/*A0二期新增消息定义*/
	SRV_REQ_DEV_QRY_NET_INFO        = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x0050), /*查询网络参数请求*/
	SRV_RES_DEV_QRY_NET_INFO        = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x0050), /*查询网络参数响应*/
	SRV_REQ_DEV_SET_NET_INFO        = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x0051), /*设置网络参数请求*/
	SRV_RES_DEV_SET_NET_INFO        = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x0051), /*设置网络参数响应*/
	SRV_REQ_DEV_QRY_MIC_MODE        = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x0052), /*查询麦克风模式请求*/
	SRV_RES_DEV_QRY_MIC_MODE        = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x0052), /*查询麦克风模式响应*/
	SRV_REQ_DEV_SET_MIC_MODE        = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x0053), /*设置麦克风模式请求*/
	SRV_RES_DEV_SET_MIC_MODE        = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x0053), /*设置麦克风模式响应*/
	SRV_REQ_DEV_QRY_MIC_AGC         = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x0054), /*查询当前麦克风增益请求*/
	SRV_RES_DEV_QRY_MIC_AGC         = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x0054), /*查询当前麦克风增益响应*/
	SRV_REQ_DEV_SET_MIC_AGC         = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x0055), /*设置麦克风增益请求*/
	SRV_RES_DEV_SET_MIC_AGC         = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x0055), /*设置麦克风增益响应*/
	SRV_REQ_DEV_QRY_AUDIO_BITRATE   = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x0056), /*查询当前音频码率请求*/
	SRV_RES_DEV_QRY_AUDIO_BITRATE   = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x0056), /*查询当前音频码率响应*/
	SRV_REQ_DEV_SET_AUDIO_BITRATE   = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x0057), /*设置当前音频码率请求*/
	SRV_RES_DEV_SET_AUDIO_BITRATE   = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x0057), /*设置当前音频码率响应*/
	/*ADD_E by liangy*/

	/*ADD_S by liangy*/
	/*录像管理相关业务消息*/
	SRV_REQ_DEV_DEL_FILE            = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x0061), /* 删除设备文件请求*/
	SRV_RES_DEV_DEL_FILE            = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x0061), /* 删除设备文件响应*/

	SRV_REQ_DEV_FILE_THUMBNAILS     = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x0062), /*(预留)查询文件缩略图请求*/
	SRV_RES_DEV_FILE_THUMBNAILS     = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x0062), /*(预留)查询文件缩略图响应*/

	SRV_REQ_DEV_SYNC_FILES          = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x0063), /*命令设备同步文件请求*/
	SRV_RES_DEV_SYNC_FILES          = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x0063), /*命令设备同步文件响应*/

	SRV_REQ_DEV_QRY_FILE            = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x0064), /* 查询设备文件信息请求*/
	SRV_RES_DEV_QRY_FILE            = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x0064), /* 查询设备文件信息响应*/
	/*ADD_E by liangy*/

	/*ADD_S by chenlg*/
	/*TLV形式的设备配置查询/设置处理消息*/
	SRV_REQ_DEV_PARAM_QUERY         = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x0070), /* 查询设备配置请求*/
	SRV_RES_DEV_PARAM_QUERY         = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x0070), /* 查询设备配置响应*/

	SRV_REQ_DEV_PARAM_SET           = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEV, 0x0071), /* 设备配置设置请求*/
	SRV_RES_DEV_PARAM_SET           = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEV, 0x0071), /* 设备配置设置响应*/

	/*ADD_E by chenlg*/

	/* 设备控制相关消息类型定义 */
	SRV_REQ_DEV_SET_TRANSMISSION    = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEC, 0x0001), /* 开启设备传输功能请求 ,服务器->设备 */
	SRV_RES_DEV_SET_TRANSMISSION    = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEC, 0x0002), /* 开启设备传输功能请求 ,服务器->设备 */

	SRV_REQ_DEV_POWER_OFF           = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEC, 0x0003), /* 设备关闭请求,客户端->用户管理     */
	SRV_RES_DEV_POWER_OFF           = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEC, 0x0004), /* 设备关闭响应,用户管理->客户端     */

	SRV_REQ_DEV_SLEEP               = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEC, 0x0005), /* 设备休眠请求,客户端->用户管理     */
	SRV_RES_DEV_SLEEP               = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEC, 0x0006), /* 设备休眠响应,用户管理->客户端     */

	SRV_REQ_DEV_POWER_ON            = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEC, 0x0007), /* 设备启动请求,客户端->用户管理     */
	SRV_RES_DEV_POWER_ON            = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEC, 0x0008), /* 设备启动响应,用户管理->客户端     */

	SRV_REQ_DEV_VIDEO               = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEC, 0x0009), /* 视频控制请求,客户端->用户管理     */
	SRV_RES_DEV_VIDEO               = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEC, 0x000A), /* 视频控制响应,用户管理->客户端     */

	SRV_REQ_DEV_AUDIO               = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEC, 0x000B), /* 音频控制请求,客户端->用户管理     */
	SRV_RES_DEV_AUDIO               = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEC, 0x000C), /* 音频控制响应,用户管理->客户端     */

	SRV_REQ_DEV_CTRL_PTZ            = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEC, 0x000D), /* 用户控制云台请求,客户端->用户管理 */
	SRV_RES_DEV_CTRL_PTZ            = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEC, 0x000E), /* 用户控制云台响应,用户管理->客户端 */    

	SRV_REQ_DEV_VIDEO_MODE          = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEC, 0x000F), /* 视频模式请求,客户端->服务器       */
	SRV_RES_DEV_VIDEO_MODE          = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEC, 0x0010), /* 视频模式响应,服务器->客户端       */

	SRV_REQ_DEV_AUDIO_MODE          = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEC, 0x0011), /* 音频模式请求,客户端->服务器       */
	SRV_RES_DEV_AUDIO_MODE          = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEC, 0x0012), /* 音频模式响应,服务器->客户端       */

	SRV_REQ_DEV_VIDAUD_MODE         = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEC, 0x0013), /* 音视频同步模式请求,客户端->服务器 */
	SRV_RES_DEV_VIDAUD_MODE         = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEC, 0x0014), /* 音视频同步模式响应,服务器->客户端 */

	SRV_REQ_DEV_REBOOT              = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DEC, 0x0015), /* 设备重启请求,客户端->用户管理     */
	SRV_RES_DEV_REBOOT              = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DEC, 0x0016), /* 设备重启响应,客户端->用户管理     */

	/*音视频预览相关消息类型定义*/
	/*ADD_S by liangy*/
	DEV_NFY_SRV_STREAM_CLOSE        = DEF_MSG_TYPE(SRV_DEV, MSG_NFY, TYPE_AVP, 0x0001), /*设备流关闭通知,设备->服务器*/
	/*ADD_E by liangy*/

	/* 语音对讲相关消息类型定义 */
	DEV_NFY_SRV_QUIT_VOICE_GROUP     = DEF_MSG_TYPE(SRV_DEV, MSG_NFY, TYPE_VIC_TALK, 0x0001), /* 成员退出语音集群通知*/

	DEV_REQ_SRV_VIC_KEEP_ALIVE       = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_VIC_TALK, 0x0002), /* 语音业务心跳请求*/
	DEV_RES_SRV_VIC_KEEP_ALIVE       = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_VIC_TALK, 0x0003), /* 语音业务心跳响应*/

	DEV_REQ_SRV_LOCK_VOICE_GROUP     = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_VIC_TALK, 0x0004), /* 成员锁定语音集群请求*/
	DEV_RES_SRV_LOCK_VOICE_GROUP     = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_VIC_TALK, 0x0005), /* 成员锁定语音集群响应*/

	DEV_NFY_SRV_RELEASE_VOICE_LOCK   = DEF_MSG_TYPE(SRV_DEV, MSG_NFY, TYPE_VIC_TALK, 0x0006), /* 成员释放集群锁定通知*/

	DEV_REQ_SRV_VIC_LOCK_KEEP_ALIVE  = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_VIC_TALK, 0x0007), /* 集群锁定心跳请求*/
	DEV_RES_SRV_VIC_LOCK_KEEP_ALIVE  = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_VIC_TALK, 0x0008), /* 集群锁定心跳响应*/

	/*语音对讲模块发出的消息定义*/
	SRV_REQ_DEV_JOIN_IN_VOICE_GROUP   = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_VIC_GROUP, 0x0001), /* 邀请成员加入语音集群请求*/
	SRV_RES_DEV_JOIN_IN_VOICE_GROUP   = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_VIC_GROUP, 0x0002), /* 邀请成员加入语音集群响应*/

	SRV_BRC_DEV_GROUP_MEMBER_ENTER    = DEF_MSG_TYPE(SRV_DEV, MSG_NFY, TYPE_VIC_GROUP, 0x0003), /* 成员加入集群广播*/
	SRV_BRC_DEV_GROUP_MEMBER_EXIT     = DEF_MSG_TYPE(SRV_DEV, MSG_NFY, TYPE_VIC_GROUP, 0x0004), /* 成员退出集群广播*/
	SRV_BRC_DEV_VOICE_GROUP_END       = DEF_MSG_TYPE(SRV_DEV, MSG_NFY, TYPE_VIC_GROUP, 0x0005), /* 语音集群结束广播*/
	SRV_BRC_DEV_VOICE_GROUP_LOCKED    = DEF_MSG_TYPE(SRV_DEV, MSG_NFY, TYPE_VIC_GROUP, 0x0006), /* 语音集群锁定广播*/
	SRV_BRC_DEV_VOICE_GROUP_UNLOCK    = DEF_MSG_TYPE(SRV_DEV, MSG_NFY, TYPE_VIC_GROUP, 0x0007), /* 语音集群解锁广播*/

	SRV_REQ_DEV_JOIN_IN_SOS_GROUP     = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_VIC_GROUP, 0x0008), /* 邀请同组在线成员加入SOS集群请求*/
	SRV_RES_DEV_JOIN_IN_SOS_GROUP     = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_VIC_GROUP, 0x0009), /* 邀请同组在线成员加入SOS集群响应*/

	/* 报警处理相关消息类型定义 */
	DEV_NFY_SRV_ALARM_NOTICE          = DEF_MSG_TYPE(SRV_DEV, MSG_NFY, TYPE_ALM, 0x0001), /* 设备报警、异常通知,设备->设备管理  */
	DEV_NFY_SRV_SOS_ALARM             = DEF_MSG_TYPE(SRV_DEV, MSG_NFY, TYPE_ALM, 0x0002), /* 设备SOS报警通知,现场设备->服务器*/
	DEV_NFY_SRV_SOS_REMOVED           = DEF_MSG_TYPE(SRV_DEV, MSG_NFY, TYPE_ALM, 0x0003), /* 设备结束SOS报警,现场设备->服务器*/
	DEV_NFY_SRV_STATUS_INFO           = DEF_MSG_TYPE(SRV_DEV, MSG_NFY, TYPE_ALM, 0x0004), /* 设备信息上报通知消息,现场设备->服务器*/

	SRV_REQ_DEV_END_SOS               = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_ALM, 0x0005), /* 用户强制关闭SOS时，报警模块转发给设备的停止请求, 客户端->服务器*/
	SRV_RES_DEV_END_SOS               = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_ALM, 0x0006), /* 用户强制关闭SOS时，设备给报警模块的停止响, 服务器->客户端*/

	/* 回放下载相关消息类型定义 */
#if 0
	SRV_REQ_DEV_MSLIST                = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DRP, 0x0001), /* 查询设备音视频列表请求, 客户端->服务器*/
	SRV_RES_DEV_MSLIST                = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DRP, 0x0002), /* 查询设备音视频列表响应, 服务器->客户端*/

	DEV_REQ_SRV_BUSINESS_ALIVE		  = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DRP, 0x0003), /* 上传/下载业务心跳保活请求, 客户端->服务器*/
	DEV_RES_SRV_BUSINESS_ALIVE		  = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DRP, 0x0004), /* 上传/下载业务心跳保活响应, 服务器->客户端*/

	SRV_REQ_DEV_UPLOAD_MSFILE         = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DRP, 0x0005), /* 上传音视频文件请求, 服务器->目标(设备/服务器)*/
	SRV_RES_DEV_UPLOAD_MSFILE         = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DRP, 0x0006), /* 上传音视频文件响应, 目标(设备/服务器)->服务器*/

	DEV_NFY_SRV_UPLOAD_FINISHED       = DEF_MSG_TYPE(SRV_DEV, MSG_NFY, TYPE_DRP, 0x0007), /* 设备音视频文件上传完成通知, 设备->服务器*/

	SRV_REQ_DEV_MSFILE_INFO           = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DRP, 0x0008), /* 查询设备文件信息请求，服务器->设备*/
	SRV_RES_DEV_MSFILE_INFO           = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DRP, 0x0009), /* 查询设备文件信息响应，设备->服务器*/
#endif
	/*ADD_S by liangy*/
	SRV_REQ_DEV_QRY_FILELIST       = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DRP, 0x0001), /* 查询文件列表业务请求*/
	SRV_RES_DEV_QRY_FILELIST       = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DRP, 0x0001), /* 查询文件列表业务响应*/
	SRV_REQ_DEV_FILELIST_PAGE      = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DRP, 0x0002), /* 文件列表分页请求*/
	SRV_RES_DEV_FILELIST_PAGE      = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DRP, 0x0002), /* 文件列表分页响应*/
	SRV_NFY_DEV_END_QRY_FILELIST   = DEF_MSG_TYPE(SRV_DEV, MSG_NFY, TYPE_DRP, 0x0003), /* 结束查询业务通知*/
	/*ADD_E by liangy*/

	/*ADD_S by sunwa*/
	SRV_REQ_DEV_PLAYBACK           = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DRP, 0x0020), /* 文件回放业务请求, 服务器->设备*/
	SRV_RES_DEV_PLAYBACK           = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DRP, 0x0020), /* 文件回放业务响应, 设备->服务器*/
	SRV_REQ_DEV_PLAYBACK_FILE      = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DRP, 0x0021), /* 文件回放指定文件请求, 服务器->设备*/
	SRV_RES_DEV_PLAYBACK_FILE      = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DRP, 0x0021), /* 文件回放指定文件响应, 设备->服务器*/
	SRV_REQ_DEV_FILE_CHANNEL_DATA  = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_DRP, 0x0022), /* 回放通道分块传输请求, 服务器->设备*/
	SRV_RES_DEV_FILE_CHANNEL_DATA  = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_DRP, 0x0022), /* 回放通道分块传输响应, 设备->服务器*/
	SRV_NFY_DEV_END_PLAYBACK       = DEF_MSG_TYPE(SRV_DEV, MSG_NFY, TYPE_DRP, 0x0023), /* 文件回放业务结束通知, 服务器->设备*/

	DEV_SRM_FILE_CHANNEL_FRAG_DATA  = DEF_MSG_TYPE(SRV_DEV, MSG_SRM, TYPE_DRP_SRM, 0x0001), /*回放分片传输数据流, 设备->服务器*/
	/*ADD_E by sunwa*/

	/* 流媒体相关消息类型定义 */
	SRV_NFY_DEV_START_MEDIA_STREAM   = DEF_MSG_TYPE(SRV_DEV, MSG_NFY, TYPE_SMP, 0x0001), 
	SRV_NFY_DEV_STOP_MEDIA_STREAM    = DEF_MSG_TYPE(SRV_DEV, MSG_NFY, TYPE_SMP, 0x0002), 
	DEV_SRM_SRV_MEDIA_STREAM         = DEF_MSG_TYPE(SRV_DEV, MSG_SRM, TYPE_SMP, 0x0003), 
	DEV_SRM_SRV_INTERCOM_STREAM      = DEF_MSG_TYPE(SRV_DEV, MSG_SRM, TYPE_SMP, 0x0004), 

	/*ADD_S by liangy @2015-06-01*/
	DEV_SRM_SRV_ENCRYPTED_MEDIA_STREAM = DEF_MSG_TYPE(SRV_DEV, MSG_SRM, TYPE_SMP, 0x0005), /*设备加密流数据*/
	DEV_SRM_SRV_DECRYPTED_MEDIA_STREAM = DEF_MSG_TYPE(SRV_DEV, MSG_SRM, TYPE_SMP, 0x0006), /*设备解密流数据(预留)*/
	/*ADD_E by liangy @2015-06-01*/

	/*ADD_S by youj @2016-02-29*/
	DEV_SRM_SED_ENCRYPT_RECORD_STREAM = DEF_MSG_TYPE(SRV_DEV, MSG_SRM, TYPE_SMP, 0x0007), /*设备流数据存储加密*/
	DEV_SRM_SED_DECRYPT_RECORD_STREAM = DEF_MSG_TYPE(SRV_DEV, MSG_SRM, TYPE_SMP, 0x0008), /*设备加密流数据存储解密*/
	/*ADD_E by youj @2016-02-29*/

	/* 安全认证相关消息类型定义 */
	DEV_REQ_SRV_DEVICE_AUTHEN        = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_SAC, 0x0001), /* 设备认证请求,设备->安全认证         */
	DEV_RES_SRV_DEVICE_AUTHEN        = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_SAC, 0x0002), /* 设备认证结果,安全认证->设备         */
	DEV_NFY_SRV_DH_PARAMETER         = DEF_MSG_TYPE(SRV_DEV, MSG_NFY, TYPE_SAC, 0x0003), /* 返回DH参数通知,设备->安全认证       */

	/*客户端、设备、服务器心跳消息*/
	DEV_REQ_SRV_KEEP_ALIVE           = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_COMMON, 0x0001), /*心跳请求消息,设备->服务器*/
	DEV_RES_SRV_KEEP_ALIVE           = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_COMMON, 0x0002), /*心跳响应消息,设备->服务器管理*/

	/*独立的密钥协商过程消息定义*/
	DEV_REQ_SRV_CRYPTO_EXCHANGE      = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_COMMON, 0x0010), /*交换加密能力和算法请求*/
	DEV_RES_SRV_CRYPTO_EXCHANGE      = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_COMMON, 0x0010), /*交换加密能力和算法响应*/
	DEV_REQ_SRV_DH_KEY_EXCHANGE      = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_COMMON, 0x0011), /*密钥交换请求*/
	DEV_RES_SRV_DH_KEY_EXCHANGE      = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_COMMON, 0x0011), /*密钥交换响应*/
	DEV_NFY_SRV_DH_KEY_ACK           = DEF_MSG_TYPE(SRV_DEV, MSG_NFY, TYPE_COMMON, 0x0012), /*密钥交换结果确认*/
	SRV_REQ_DEV_RECORD_KEY_NEGO      = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_COMMON, 0x0013), /*设备录像密钥协商请求*/
	SRV_RES_DEV_RECORD_KEY_NEGO      = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_COMMON, 0x0013), /*设备录像密钥协商响应*/
	SRV_REQ_DEV_RECORD_KEY_UPDATE    = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_COMMON, 0x0014), /*设备录像密钥更新请求*/
	SRV_RES_DEV_RECORD_KEY_UPDATE    = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_COMMON, 0x0014), /*设备录像密钥更新响应*/

	/*同步上传消息定义*/
	/*ADD_S by liangy*/
	DEV_REQ_SRV_SYNC                = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_SYN, 0x0001), /*同步业务请求, 设备->服务器*/
	DEV_RES_SRV_SYNC                = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_SYN, 0x0001), /*同步业务响应, 服务器->设备*/
	DEV_REQ_SRV_UPLOAD_FILE         = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_SYN, 0x0004), /*上传文件请求, 设备->服务器*/
	DEV_RES_SRV_UPLOAD_FILE         = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_SYN, 0x0004), /*上传文件响应, 服务器->设备*/
	DEV_REQ_SRV_UPLOAD_BLOCK        = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_SYN, 0x0005), /*上传分块请求, 设备->服务器*/
	DEV_RES_SRV_UPLOAD_BLOCK        = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_SYN, 0x0005), /*上传分块响应, 服务器->设备*/
#if 0
	SRV_REQ_DEV_SYNC_STATUS         = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_SYN, 0x0006), /*查询上传状态请求*/
	SRV_RES_DEV_SYNC_STATUS         = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_SYN, 0x0006), /*查询上传状态响应*/
#endif
	DEV_NFY_SRV_SYNC_STATUS         = DEF_MSG_TYPE(SRV_DEV, MSG_NFY, TYPE_SYN, 0x0007), /*上传状态通知*/
	DEV_NFY_SRV_END_SYNC            = DEF_MSG_TYPE(SRV_DEV, MSG_NFY, TYPE_SYN, 0x000F), /*结束同步业务通知, 设备->服务器*/

	/*同步上传流消息类型定义*/
	DEV_SRM_UPLOAD_FRAG     = DEF_MSG_TYPE(SRV_DEV, MSG_SRM, TYPE_SYN_SRM, 0x0001), /*上传分片传输数据流,设备->端务器*/
	/*ADD_E by liangy*/

	/*VPN业务消息定义*/
	DEV_REQ_VPNCONFIG                = DEF_MSG_TYPE(SRV_DEV, MSG_REQ, TYPE_VPN, 0x0001), /*查询VPN配置请求,设备->服务器*/
	DEV_RES_VPNCONFIG                = DEF_MSG_TYPE(SRV_DEV, MSG_RES, TYPE_VPN, 0x0001), /*查询VPN配置响应,服务器->设备*/
	SRV_NFY_VPNCONFIG                = DEF_MSG_TYPE(SRV_DEV, MSG_NFY, TYPE_VPN, 0x0002), /*VPN配置更新通知,服务器->设备*/

	/*VPN数据流定义*/
	MSG_DATA_VPN                     = DEF_MSG_TYPE(SRV_DEV, MSG_SRM, TYPE_VPN_SRM, 0x0001), /* VPN数据流,设备<->服务器*/

	MAX_SRV_DEV_MSGTYPE ,
	//  SRV_REQ_DEV_REBOOT					         = 0x10480015,                                         //设备重启请求
	//  SRV_RES_DEV_REBOOT					         = 0x11480016,                                         //设备重启响应
	// SRV_REQ_DEV_CHG_VIDEO_PARAM				     = 0x10460032,                                         //更改视频采集参数请求
	//  SRV_RES_DEV_CHG_VIDEO_PARAM				     = 0x11460033,                                         //更改视频采集参数响应
	SRV_REQ_DEV_GET_VIDEO_COLOR_ATTR			 = 0x10460080, 	                                       //获取设备图像属性请求
	SRV_RES_DEV_GET_VIDEO_COLOR_ATTR			 = 0x11460080, 		                                   //获取设备图像属性响应
	SRV_REQ_DEV_SET_VIDEO_COLOR_ATTR			 = 0x10460081, 	                                       //设置设备图像属性请求
	SRV_RES_DEV_SET_VIDEO_COLOR_ATTR			 = 0x11460081, 		                                   //设置设备图像属性响应
	SRV_REQ_DEV_GET_CAMERA_CFG					 = 0x10460082,                                         //获取设备镜头参数请求
	SRV_RES_DEV_GET_CAMERA_CFG					 = 0x11460082,                                         //获取设备镜头参数响应
	SRV_REQ_DEV_SET_CAMERA_CFG					 = 0x10460083,                                         //设置设备镜头参数请求
	SRV_RES_DEV_SET_CAMERA_CFG					 = 0x11460083,                                         //设置设备镜头参数响应
	SRV_REQ_DEV_GET_MOTTON_DETECT_ATTR			 = 0x10460084, 		                                   //获取设备移动侦测属性请求
	SRV_RES_DEV_GET_MOTTON_DETECT_ATTR			 = 0x11460084,                                         //获取设备移动侦测属性响应
	SRV_REQ_DEV_SET_MOTTON_DETECT_ATTR			 = 0x10460085, 	                                       //设置设备移动侦测属性请求
	SRV_RES_DEV_SET_MOTTON_DETECT_ATTR			 = 0x11460085, 	                                       //设置设备移动侦测属性响应
	DEV_NFY_SRV_MOTTON_DETECT_ALARM				 = 0x124D0007, 	                                       //设备移动侦测报警通知
	SRV_REQ_DEV_RESET_VIDEO_COLOR_ATTR			 = 0x10460086, 		                                   //恢复设备图像默认属性请求
	SRV_RES_DEV_RESET_VIDEO_COLOR_ATTR			 = 0x11460086, 		                                   //恢复设备图像默认属性响应
	SRV_REQ_DEV_GET_VIDEO_COLOR_SCENE			 = 0x10460087, 		                                   //获取设备图像场景请求消息
	SRV_RES_DEV_GET_VIDEO_COLOR_SCENE			 = 0x11460087, 		                                   //获取设备图像默认属性响应
	SRV_REQ_DEV_SET_VIDEO_COLOR_SCENE			 = 0x10460088, 		                                   //保存设备图像场景请求
	SRV_RES_DEV_SET_VIDEO_COLOR_SCENE			 = 0x11460088, 		                                   //保存设备图像场景响应
	SRV_REQ_DEV_USE_VIDEO_COLOR_SCENE			 = 0x10460089, 	                                       //应用设备图像场景请求
	SRV_RES_DEV_USE_VIDEO_COLOR_SCENE			 = 0x11460089		                                   //应用设备图像场景响应


} EN_SRV_DEV_MSGTYPE_T;

#endif
