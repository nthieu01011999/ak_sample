#ifndef __APP_H__
#define __APP_H__

#include <string>

#include "app_config.h"
#include "cpptime.h"

using namespace std;

/*****************************************************************************/
/* task GW_SYS define.
 */
/*****************************************************************************/
/* define timer */
#define GW_SYS_WATCH_DOG_TIMEOUT_INTERVAL (10000) /* 30s */ // TODO: Release change to 5s 
/* define signal */
enum {
	GW_SYS_WATCH_DOG_REPORT_REQ = AK_USER_DEFINE_SIG,
	GW_SYS_WATCH_DOG_DBG_1,
	GW_SYS_WATCH_DOG_DBG_2,
	GW_SYS_WATCH_DOG_DBG_3,
	GW_SYS_WATCH_DOG_DBG_4,
	GW_SYS_WATCH_DOG_DBG_5,
	GW_SYS_WATCH_DOG_DBG_6,
};

/*****************************************************************************/
/*  task GW_CONSOLE define
 */
/*****************************************************************************/
/* define timer */

/* define signal */
enum {
	GW_CONSOLE_INTERNAL_LOGIN_CMD = AK_USER_DEFINE_SIG,
	GW_HELLO_WORLD,
};

/*****************************************************************************/
/*  task GW_CLOUD define
 */
/*****************************************************************************/
/* define timer */
#define GW_WEB_MQTT_TRY_CONNECT_TIMEOUT_INTERVAL		  (5000)
#define GW_WEB_MQTT_CHECK_CONNECT_STATUS_INTERVAL		  (10000)
#define GW_WEB_MQTT_TRY_CONNECT_AFTER_DISCONNECT_INTERVAL (20000)

/* define signal */
enum {
	GW_CLOUD_WATCHDOG_PING_REQ = AK_USER_WATCHDOG_SIG,
	GW_CLOUD_MQTT_INIT_REQ	   = AK_USER_DEFINE_SIG,
	GW_CLOUD_MQTT_TRY_CONNECT_REQ,
	GW_CLOUD_MQTT_CHECK_CONNECT_STATUS_REQ,
	GW_CLOUD_DATA_COMMUNICATION,
	GW_CLOUD_SET_MQTT_CONFIG_REQ,
	GW_CLOUD_GET_MQTT_CONFIG_REQ,
	GW_CLOUD_CAMERA_STATUS_RES,
	GW_CLOUD_CAMERA_CONFIG_RES,
	GW_CLOUD_CAMERA_ALARM_RES,
	GW_CLOUD_SIGNALING_MQTT_RES,
	GW_CLOUD_MESSAGE_LENGTH_OUT_OF_RANGE_REP,
};


/*****************************************************************************/
/* task GW_AV define
 */
/*****************************************************************************/
/* define timer */
/* define signal */

enum {
	GW_AV_SET_ENCODE_REQ,
	GW_AV_SET_WATERMARK_REQ,
	GW_AV_SET_CAMERAPARAM_REQ,
	GW_AV_SNAP_JPEG_REQ,
};

/*****************************************************************************/
/* task GW_CONFIG define
 */
/*****************************************************************************/
/* define timer */
/* define signal */

enum {
	// SET
	GW_CONFIG_SET_CONFIG,
	GW_CONFIG_SET_RTMP_CONFIG_REQ,
	GW_CONFIG_SET_MOTION_CONFIG_REQ,
	GW_CONFIG_SET_ENCODE_CONFIG_REQ,
	GW_CONFIG_SET_WATERMARK_CONFIG_REQ,
	GW_CONFIG_SET_RESET_CONFIG_REQ,
	GW_CONFIG_SET_STORAGE_FORMAT_CONFIG_REQ,
	GW_CONFIG_SET_CAMERA_PARAM_CONFIG_REQ,
	GW_CONFIG_SET_WIFI_CONFIG_REQ,
	GW_CONFIG_SET_S3_CONFIG_REQ,
	GW_CONFIG_SET_UPGRADE_REQ,
	GW_CONFIG_SET_REBOOT_REQ,
	GW_CONFIG_SET_RECORD_CONFIG_REQ,
	// GET
	GW_CONFIG_GET_RTMP_CONFIG_REQ,
	GW_CONFIG_GET_MOTION_CONFIG_REQ,
	GW_CONFIG_GET_ENCODE_CONFIG_REQ,
	GW_CONFIG_GET_WATERMARK_CONFIG_REQ,
	GW_CONFIG_GET_RESET_CONFIG_REQ,
	GW_CONFIG_GET_STORAGE_INFO_CONFIG_REQ,
	GW_CONFIG_GET_CAMERA_PARAM_CONFIG_REQ,
	GW_CONFIG_GET_WIFI_CONFIG_REQ,
	GW_CONFIG_GET_S3_CONFIG_REQ,
	GW_CONFIG_GET_NETINFO_CONFIG_REQ,
	GW_CONFIG_GET_SYSINFO_CONFIG_REQ,
	GW_CONFIG_GET_RECORD_CONFIG_REQ,
	// ACT
	GW_CONFIG_ACT_UPLOAD_FILE_REQ,
};

/*****************************************************************************/
/* task GW_DETECT define
 */
/*****************************************************************************/
/* define timer */
/* define signal */

enum {
	GW_AV_SET_MOTION_REQ,
	GW_DETECT_INTERVAL_REQ,
	GW_DETECT_DURATION_REQ,
};

/*****************************************************************************/
/* task GW_LED define
 */
/*****************************************************************************/
/* define timer */
#define GW_LED_BLINK_1S_INTERVAL	(10000)
#define GW_LED_BLINK_500MS_INTERVAL (5000)
/* define signal */

enum {
	GW_LED_WHITE_BLINK,
	GW_LED_ORANGE_BLINK,
};

/*****************************************************************************/
/* task GW_UPLOAD define
 */
/*****************************************************************************/
/* define timer */
/* define signal */

enum {
	GW_UPLOAD_FILE_JPEG,
};

/*****************************************************************************/
/* define timer */
#define GW_HELLO_TRIGGERED_1S_INTERVAL				  (1000)
/* define signal */

enum {
	GW_HELLO_TRIGGERED,
};

/*****************************************************************************/
/* task GW_NETWORK define
 */
/*****************************************************************************/
/* define timer */
#define GW_NET_CHECK_CONNECT_ROUTER_INTERVAL (1000) /* 1s */

/* define signal */
enum {
	/* wifi station interface */
	GW_NET_WIFI_STA_START = AK_USER_DEFINE_SIG,
	GW_NET_WIFI_STA_SET_CONF,
	GW_NET_WIFI_STA_GET_CONF,

	GW_NET_WIFI_MQTT_SET_CONF,
	GW_NET_WIFI_MQTT_GET_CONF,

	GW_NET_WIFI_STA_CONNECT_TO_ROUTER,
	GW_NET_WIFI_STA_CONNECTED_TO_ROUTER,

	GW_NET_WIFI_AP_START,

	GW_NET_WIFI_GET_INFO,

	GW_NET_INTERNET_STATUS,
};

/*****************************************************************************/
/*  task GW_TASK_WEBRTC define
 */
/*****************************************************************************/
/* define timer */
#define GW_WEBRTC_ERASE_CLIENT_NO_ANSWER_TIMEOUT_INTERVAL (50000) /* 30s */
#define GW_WEBRTC_WAIT_REQUEST_TIMEOUT_INTERVAL			  (20000) /* 20s */

/* define signal */
enum {
	GW_WEBRTC_SIGNALING_MQTT_REQ = AK_USER_DEFINE_SIG,
#ifdef TEST_USE_WEB_SOCKET
	GW_WEBRTC_SIGNALING_SOCKET_REQ,
#endif
	GW_WEBRTC_CHECK_CLIENT_CONNECTED_REQ,
	GW_WEBRTC_ERASE_CLIENT_REQ,
	GW_WEBRTC_DBG_IPC_SEND_MESSAGE_REQ,
	GW_WEBRTC_ON_MESSAGE_CONTROL_DATACHANNEL_REQ,
	GW_WEBRTC_DATACHANNEL_DOWNLOAD_RELEASE_REQ,
};

/*****************************************************************************/
/*  task GW_TASK_RTMP define
 */
/*****************************************************************************/
/* define timer */
#define GW_RTMP_RECONNECT_INTERVAL (30000) /* 30s */

/* define signal */
enum {
	GW_RTMP_START = AK_USER_DEFINE_SIG,
	GW_RTMP_STOP,
	GW_RTMP_SET_CONFIG,
	GW_RTMP_GET_CONFIG,
};

/*****************************************************************************/
/*  global define variable
 */
/*****************************************************************************/
#define APP_OK (0x00)
#define APP_NG (0x01)

#define APP_FLAG_OFF (0x00)
#define APP_FLAG_ON	 (0x01)

#define APP_ROOT_PATH_DISK "/opt/p2p"
#define APP_ROOT_PATH_RAM  "/tmp/p2p"

/* define file name */
#define MTCE_SERIAL_FILE	   "Serial"
#define MTCE_ACCOUNT_FILE	   "Account"
#define MTCE_P2P_FILE		   "P2P"
#define MTCE_ENCODE_FILE	   "Encode"
#define MTCE_DEVINFO_FILE	   "DeviceInfo"
#define MTCE_MOTION_FILE	   "Motion"
#define MTCE_WIFI_FILE		   "Wifi"
#define MTCE_WATERMARK_FILE	   "Watermark"
#define MTCE_S3_FILE		   "S3"
#define MTCE_RTMP_FILE		   "RTMP"
#define MTCE_NETWORK_CA_FILE   "Bundle_RapidSSL_2023.cert"
#define MTCE_NETWORK_MQTT_FILE "MQTTService"
#define MTCE_NETWORK_WPA_FILE  "wpa_supplicant.conf"
#define MTCE_CAMERA_PARAM_FILE "CameraParam"
#define MTCE_PTZ_FILE		   "PTZ"
#define MTCE_LOG_FILE		   "app.log"
#define MTCE_RTC_SERVERS_FILE  "RtcServersConfig"
#define MTCE_STORAGE_FILE	   "Storage"
#define MTCE_RECORD_FILE	   "Record"

/* define path name */

#define MTCE_DEVICE_INFO_PATH "/app/init/version"

#define MTCE_USER_CONF_PATH	 "/usr/conf"	 // TODO change
// #define MTCE_DFAUL_CONF_PATH "/app/default"
#define MTCE_DFAUL_CONF_PATH "/app"

#define MTCE_MEDIA_VIDEO_PATH "/media/video"
#define MTCE_MEDIA_AUDIO_PATH "/media/audio"
#define MTCE_MEDIA_JPEG_PATH  "/media/jpeg"
#define MTCE_LOG_FILE_PATH	  "/var/log"

#define MTCE_AUDIO_AAC_EXT	".aac"
#define MTCE_AUDIO_G711_EXT ".g711"

#define MTCE_MQTT_KEEPALIVE	 90
#define MTCE_SSL_VERIFY_NONE 0
#define MTCE_SSL_VERIFY_PEER 1

// TODO test download path
#ifdef BUILD_ARM_RTS
#define TEST_SDCARD_URL "/tmp/demo_nfs/records"	   // TODO: test
// #define TEST_SDCARD_URL "/tmp/hung_nfs/records"
// #define TEST_SDCARD_URL "/tmp/hung_nfs/records"
#else
#define TEST_SDCARD_URL "/home/huy/Workspace/FPT/Github/Camera/huynt_nfs/records"
#endif
#define MTCE_MEDIA_VIDEO_PATH_TEST TEST_SDCARD_URL "/video"
#define MTCE_MEDIA_AUDIO_PATH_TEST TEST_SDCARD_URL "/audio"

extern CppTime::Timer systemTimer;

#endif	  // __APP_H__
