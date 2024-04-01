#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#include "app.h"
#include "app_data.hpp"
#include "app_dbg.h"
#include "parser_json.h"

using namespace std;

bool mtce_jsonGetRtcServers(json &json_in, rtcServersConfig_t *rtcSvCfg) {
	try {
		rtcSvCfg->clear();
		json arrJs = json_in["Connections"];
		for (auto &server : arrJs) {
			string type = server["Type"].get<string>();
			if (type == "stun") {
				rtcSvCfg->stunServerCfg = server["StunUrl"].get<string>();
			}
			else if (type == "turn") {
				rtcSvCfg->turnServerCfg = server["TurnUrl"].get<string>();
			}
		}
		return true;
	}
	catch (const exception &e) {
		APP_DBG("json error: %s\n", e.what());
	}
	return false;
}
