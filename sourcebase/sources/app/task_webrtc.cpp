#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <vector>

#include <algorithm>
#include <future>
#include <iostream>
#include <memory>
#include <random>
#include <stdexcept>
#include <thread>
#include <unordered_map>
#include <chrono>
#include <pthread.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <atomic>

#include "app.h"
#include "app_dbg.h"
#include "task_list.h"
#include "rtc/rtc.hpp"

using namespace rtc;
using namespace std;
using namespace chrono_literals;
using namespace chrono;

using json = nlohmann::json;

static rtcServersConfig_t rtcServerCfg;

q_msg_t gw_task_webrtc_mailbox;

void *gw_task_webrtc_entry(void *) {
	ak_msg_t *msg = AK_MSG_NULL;

	wait_all_tasks_started();

	APP_DBG("[STARTED] gw_task_webrtc_entry\n");

	Configuration config;
	mtce_configGetRtcServers(&rtcServerCfg);
	APP_PRINT("STUN server is: %s\n", rtcServerCfg.stunServerCfg.c_str());
	APP_PRINT("TURN server is: %s\n", rtcServerCfg.turnServerCfg.c_str());
	if (rtcServerCfg.stunServerCfg != "") {
		config.iceServers.emplace_back(rtcServerCfg.stunServerCfg);
	}
	if (rtcServerCfg.turnServerCfg != "") {
		config.iceServers.emplace_back(rtcServerCfg.turnServerCfg);
	}
	config.disableAutoNegotiation = false;	  // NOTE call setLocalDescription auto

	while (1) {
		/* get messge */
		msg = ak_msg_rev(GW_TASK_WEBRTC_ID);

		switch (msg->header->sig) {
			
		default:
		break;
		}

		/* free message */
		ak_msg_free(msg);
	}

	return (void *)0;
}
