#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <regex>
#include <string.h>

#include "utils.h"

#include "app.h"
#include "app_dbg.h"
#include "app_config.h"
#include "parser_json.h"

/******************************************************************************
 * APP CONFIGURE CLASS (BASE)
 ******************************************************************************/
app_config::app_config() {}


int mtce_configGetRtcServers(rtcServersConfig_t *rtcServerCfg) {
	json cfgJs;
	string file = MTCE_USER_CONF_PATH "/" MTCE_RTC_SERVERS_FILE;
	if (!read_json_file(cfgJs, file)) {
		APP_DBG("Can not read: %s\n", file.data());
		file = MTCE_DFAUL_CONF_PATH "/" MTCE_RTC_SERVERS_FILE;
		cfgJs.clear();
		if (!read_json_file(cfgJs, file)) {
			APP_DBG("Can not read: %s\n", file.data());
			return APP_CONFIG_ERROR_FILE_OPEN;
		}
	}

	if (mtce_jsonGetRtcServers(cfgJs, rtcServerCfg)) {
		APP_DBG("rtc server config: %s\n", cfgJs.dump().data());
		return APP_CONFIG_SUCCESS;
	}
	else {
		APP_DBG("Can not convert: %s\n", file.data());
		return APP_CONFIG_ERROR_DATA_INVALID;
	}
}