#ifndef __MTCE_APP_JSON_H__
#define __MTCE_APP_JSON_H__

#include "app_data.h"

extern bool mtce_jsonSetConfigSetRes(json &json_in, const char *mess_type, int ret);
extern bool mtce_jsonSetConfigGetRes(json &json_in, json &json_data, const char *mess_type, int ret);
extern bool mtce_jsonAlarmMotion(json &json_in, int channel, const char *startTime);
extern bool mtce_jsonAlarmUploadFile(json &json_in, json &json_data);

extern bool mtce_jsonSetNetMQTT(json &json_in, mtce_netMQTT_t *mqtt);
extern bool mtce_jsonGetNetMQTT(json &json_in, mtce_netMQTT_t *param);

// extern bool mtce_jsonGetRtcServers(json &json_in, rtcServersConfig_t *rtcSvCfg);

extern bool mtce_jsonGetRtcServers(const json &json_in, rtcServersConfig_t *rtcSvCfg);

extern bool mtce_jsonSetMotion(json &json_in, mtce_motionSetting_t *param);
extern bool mtce_jsonGetMotion(json &json_in, mtce_motionSetting_t *param);

extern bool mtce_jsonSetEncode(json &json_in, mtce_encode_t *param);
extern bool mtce_jsonGetEncode(json &json_in, mtce_encode_t *param);

extern bool mtce_jsonSetParam(json &json_in, mtce_cameraParam_t *param);
extern bool mtce_jsonGetParam(json &json_in, mtce_cameraParam_t *param);

extern bool mtce_jsonSetWifi(json &json_in, mtce_netWifi_t *param);
extern bool mtce_jsonGetWifi(json &json_in, mtce_netWifi_t *param);

extern bool mtce_jsonSetRTMP(json &json_in, mtce_rtmp_t *param);
extern bool mtce_jsonGetRTMP(json &json_in, mtce_rtmp_t *param);

extern bool mtce_jsonSetWatermark(json &json_in, mtce_watermark_t *param);
extern bool mtce_jsonGetWatermark(json &json_in, mtce_watermark_t *param);

extern bool mtce_jsonGetReset(json &json_in, mtce_reset_t *param);
extern bool mtce_jsonGetUpgrade(json &json_in, mtce_upgrade_t *param);
extern bool mtce_jsonGetUpoadFile(json &json_in, mtce_upload_file_t *param);

extern bool mtce_jsonSetStorage(json &json_in, mtce_storage_t *param);
extern bool mtce_jsonGetStorage(json &json_in, mtce_storage_t *param);

extern bool mtce_jsonSetS3(json &json_in, mtce_s3_t *param);
extern bool mtce_jsonGetS3(json &json_in, mtce_s3_t *param);

extern bool mtce_jsonSetAccount(json &json_in, mtce_account_t *param);
extern bool mtce_jsonGetAccount(json &json_in, mtce_account_t *param);

extern bool mtce_jsonSetNetInfo(json &json_in, mtce_netWifi_t *param);
extern bool mtce_jsonSetSysInfo(json &json_in);

extern bool mtce_jsonSetRecord(json &json_in, mtce_record_t *param);
extern bool mtce_jsonGetRecord(json &json_in, mtce_record_t *param);

#endif	  // __MTCE_JSON_H__
