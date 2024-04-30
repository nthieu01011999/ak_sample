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


#endif	  //__APP_CONFIG_H__
