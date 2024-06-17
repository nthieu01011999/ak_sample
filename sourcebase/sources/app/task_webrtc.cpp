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
// #include "parameter.hpp"
#include "mqtt.hpp"
 

void safe_ak_msg_free(ak_msg_t *msg);

q_msg_t gw_task_webrtc_mailbox;
 


 
void *gw_task_webrtc_entry(void *) {

	wait_all_tasks_started();
	APP_DBG("[STARTED] gw_task_webrtc_entry\n");

	ak_msg_t *msg = AK_MSG_NULL;



	task_post_pure_msg(GW_TASK_WEBRTC_ID, GW_CLOUD_MQTT_INIT_REQ);
	
    while (1) {
 
        msg = ak_msg_rev(GW_TASK_WEBRTC_ID);

        // Check if message is not NULL before processing
        if (msg != NULL) {
            /* get message */
            switch (msg->header->sig) {
            case GW_CLOUD_MQTT_INIT_REQ: {
                APP_DBG_SIG("GW_CLOUD_MQTT_INIT_REQ\n");
 
				APP_DBG("[YOU ARE HERE]\n");
				
            } break;

            default:
                break;
            }

            /* free message */
            safe_ak_msg_free(msg);
        }
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

 