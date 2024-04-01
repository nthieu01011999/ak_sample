#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

#include <stdint.h>
#include <string.h>

#include "ak.h"
#include "app.h"
#include "app_data.hpp"

#include "ak_dbg.h"

#include "sys_dbg.h"

#include "json.hpp"

using namespace std;
using json = nlohmann::json;

#define APP_CONFIG_SUCCESS			  (0)
#define APP_CONFIG_ERROR_FILE_OPEN	  (-1)
#define APP_CONFIG_ERROR_DATA_MALLOC  (-2)
#define APP_CONFIG_ERROR_DATA_INVALID (-3)
#define APP_CONFIG_ERROR_DATA_DIFF	  (-4)
#define APP_CONFIG_ERROR_TIMEOUT	  (-5)
#define APP_CONFIG_ERROR_BUSY		  (-6)
#define APP_CONFIG_ERROR_ANOTHER	  (-7)

/******************************************************************************
 * APP CONFIGURE CLASS (BASE)
 ******************************************************************************/
class app_config {
public:
	app_config();

	void initializer(char *);
	/* configure file */
	void set_config_file_path(char *);
	void set_config_file_path(string);
	void get_config_file_path(char *);
	void get_config_file_path(string &);

	int read_config_file_to_str(string &cfg);
	bool read_config_file_to_js(json &);
	int write_config_file_to_str(string &cfg);
	bool write_config_file_from_js(json &);

private:
	string m_config_path;
};

/******************************************************************************
 * global config
 ******************************************************************************/
#define MESSAGE_TYPE_DEVINFO		"DeviceInfo"
#define MESSAGE_TYPE_UPGRADE		"UpgradeFirmware"
#define MESSAGE_TYPE_ALARM			"AlarmInfo"
#define MESSAGE_TYPE_MQTT			"MQTTSetting"
#define MESSAGE_TYPE_FTP			"FTPSetting"
#define MESSAGE_TYPE_RTMP			"RTMPSetting"
#define MESSAGE_TYPE_MOTION			"MotionSetting"
#define MESSAGE_TYPE_MOTION_DETECT	"MotionDetect"
#define MESSAGE_TYPE_ENCODE			"EncodeSetting"
#define MESSAGE_TYPE_REBOOT			"Reboot"
#define MESSAGE_TYPE_RESET			"ResetSetting"
#define MESSAGE_TYPE_STORAGE_FORMAT "StorageFormat"
#define MESSAGE_TYPE_STORAGE_INFO	"StorageInfo"
#define MESSAGE_TYPE_CAMERA_PARAM	"ParamSetting"
#define MESSAGE_TYPE_WIFI			"WifiSetting"
#define MESSAGE_TYPE_WATERMARK		"WatermarkSetting"
#define MESSAGE_TYPE_S3				"S3Setting"
#define MESSAGE_TYPE_NETWORK_INFO	"NetworkInfo"
#define MESSAGE_TYPE_SYSTEM_INFO	"SystemInfo"
#define MESSAGE_TYPE_ACCOUNT		"AccountSetting"
#define MESSAGE_TYPE_NETWORKAP		"NetworkAp"
#define MESSAGE_TYPE_P2P			"P2PSetting"
#define MESSAGE_TYPE_ERROR_INFO		"ErrorInfo"
#define MESSAGE_TYPE_PTZ			"PTZ"
#define MESSAGE_TYPE_UPLOAD_FILE	"UploadFile"
#define MESSAGE_TYPE_SIGNALING		"Signaling"
#define MESSAGE_TYPE_RECORD			"RecordSetting"

extern int mtce_configGetSerial(char *serial);
extern int mtce_configGetDeviceInfoStr(json &devInfoJs);

extern int mtce_configSetMQTT(mtce_netMQTT_t *mqttCfg);
extern int mtce_configGetMQTT(mtce_netMQTT_t *mqttCfg);

extern int mtce_configGetRtcServers(rtcServersConfig_t *rtcServerCfg);

extern int mtce_configSetMotion(mtce_motionSetting_t *motionCfg);
extern int mtce_configGetMotion(mtce_motionSetting_t *motionCfg);

extern int mtce_configSetEncode(mtce_encode_t *encodeCfg);
extern int mtce_configGetEncode(mtce_encode_t *encodeCfg);

extern int mtce_configSetParam(mtce_cameraParam_t *paramCfg);
extern int mtce_configGetParam(mtce_cameraParam_t *paramCfg);

extern int mtce_configSetWifi(mtce_netWifi_t *wifiCfg);
extern int mtce_configGetWifi(mtce_netWifi_t *wifiCfg);

extern int mtce_configSetRTMP(mtce_rtmp_t *rtmpCfg);
extern int mtce_configGetRTMP(mtce_rtmp_t *rtmpCfg);

extern int mtce_configSetWatermark(mtce_watermark_t *watermarkCfg);
extern int mtce_configGetWatermark(mtce_watermark_t *watermarkCfg);

extern int mtce_configSetStorage(mtce_storage_t *storageCfg);
extern int mtce_configGetStorage(mtce_storage_t *storageCfg);

extern int mtce_configSetS3(mtce_s3_t *s3Cfg);
extern int mtce_configGetS3(mtce_s3_t *s3Cfg);

extern int mtce_configSetAccount(mtce_account_t *accountCfg);
extern int mtce_configGetAccount(mtce_account_t *accountCfg);

extern int mtce_configSetRecord(mtce_record_t *recordCfg);
extern int mtce_configGetRecord(mtce_record_t *recordCfg);

#endif	  //__APP_CONFIG_H__
