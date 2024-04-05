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
#include "task_webrtc.h"
#include "rtc/rtc.hpp"
#include "h26xsource.hpp"
#include "audiosource.hpp"


using namespace rtc;
using namespace std;
using namespace chrono_literals;
using namespace chrono;

using json = nlohmann::json;
std::optional<std::shared_ptr<Stream>> avStream = std::nullopt;
std::mutex clientsMutex;
ClientsGroup_t clients;

static rtcServersConfig_t rtcServerCfg;
void safe_ak_msg_free(ak_msg_t *msg);

q_msg_t gw_task_webrtc_mailbox;

void *gw_task_webrtc_entry(void *) {

	ak_msg_t *msg = AK_MSG_NULL;

	wait_all_tasks_started();

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
	config.disableAutoNegotiation = false;	  

	APP_DBG("[STARTED] gw_task_webrtc_entry\n");
	
	/* init websocket */
	auto ws = make_shared<WebSocket>();	   
	ws->onOpen([]() { APP_PRINT("WebSocket connected, signaling ready\n"); });

	ws->onClosed([]() { APP_PRINT("WebSocket closed\n"); });

	ws->onError([](const string &error) { APP_PRINT("WebSocket failed: %s\n", error.c_str()); });

	ws->onMessage([&](variant<binary, string> data) {
		if (!holds_alternative<string>(data))
			return;
		string msg = get<string>(data);
		task_post_dynamic_msg(GW_TASK_WEBRTC_ID, GW_WEBRTC_SIGNALING_SOCKET_REQ, (uint8_t *)msg.data(), msg.length() + 1);
	});

	const string url = "ws://42.116.138.38:8089/" + mtce_getSerial();
	APP_PRINT("WebSocket URL is %s\n", url.c_str());
	ws->open(url);
	
	while (1) {
		/* get messge */
 		switch (msg->header->sig) {
		


		default:
		break;
		}

		/* free message */
		safe_ak_msg_free(msg);
		// ak_msg_free(msg);
	}

	return (void *)0;
}

 void safe_ak_msg_free(ak_msg_t *msg) {
	if (msg) {
		AK_MSG_DBG("Freeing message: %p\n", msg);
		ak_msg_free(msg);
	} else {
		AK_MSG_DBG("Attempted to free NULL message\n");
	}
}
