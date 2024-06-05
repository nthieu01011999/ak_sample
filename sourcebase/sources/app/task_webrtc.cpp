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
#include "parameter.hpp"
#include "mqtt.hpp"

mtce_netMQTT_t mqttConfig;
mqttTopicCfg_t topicConfig;

void safe_ak_msg_free(ak_msg_t *msg);

q_msg_t gw_task_webrtc_mailbox;
unique_ptr<mqtt> mospp;

static bool mqttInitialized = false;
static mtce_netMQTT_t mqttService = {0};
static mqttTopicCfg_t mqttTopics;


void initializeMQTTTopicConfig(mqttTopicCfg_t *topicConfig);
void initializeMQTTConfig(mtce_netMQTT_t *mqttConfig);

void printMQTTTopicConfig(const mqttTopicCfg_t *topicConfig) {
    printf("MQTT Topic Configuration:\n");
    printf("Status Topic: %s\n", topicConfig->topicStatus);
    printf("Request Topic: %s\n", topicConfig->topicRequest);
    printf("Response Topic: %s\n", topicConfig->topicResponse);
}

// Function to Print MQTT Configuration
void printMQTTConfig(const mtce_netMQTT_t *mqttConfig) {
    printf("MQTT Configuration:\n");
    printf("Client ID: %s\n", mqttConfig->clientID);
    printf("Host: %s\n", mqttConfig->host);
    printf("Port: %d\n", mqttConfig->port);
    printf("Keep Alive: %d\n", mqttConfig->keepAlive);
    printf("QOS: %d\n", mqttConfig->QOS);
    // Uncomment if username and password are used
    // printf("Username: %s\n", mqttConfig->username);
    // printf("Password: %s\n", mqttConfig->password);
}

void *gw_task_webrtc_entry(void *) {

	wait_all_tasks_started();
	APP_DBG("[STARTED] gw_task_webrtc_entry\n");

	ak_msg_t *msg = AK_MSG_NULL;



	task_post_pure_msg(GW_TASK_WEBRTC_ID, GW_CLOUD_MQTT_INIT_REQ);
	
    while (1) {
        // Retrieve message from mailbox
        // ak_msg_t *msg = ak_msg_rev(GW_TASK_WEBRTC_ID);
        msg = ak_msg_rev(GW_TASK_WEBRTC_ID);

        // Check if message is not NULL before processing
        if (msg != NULL) {
            /* get message */
            switch (msg->header->sig) {
            case GW_CLOUD_MQTT_INIT_REQ: {
                APP_DBG_SIG("GW_CLOUD_MQTT_INIT_REQ\n");
				initializeMQTTConfig(&mqttConfig);
				initializeMQTTTopicConfig(&topicConfig);
				printMQTTConfig(&mqttConfig);
				printMQTTTopicConfig(&topicConfig);
				mospp.reset(new mqtt(&topicConfig, &mqttConfig));
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

void initializeMQTTTopicConfig(mqttTopicCfg_t *topicConfig) {
    strcpy(topicConfig->topicStatus, "example/status");
    strcpy(topicConfig->topicRequest, "example/request");
    strcpy(topicConfig->topicResponse, "example/response");
}

void initializeMQTTConfig(mtce_netMQTT_t *mqttConfig) {
    strcpy(mqttConfig->clientID, "c02i24010000008");
    // strcpy(mqttConfig->username, "exampleUser");
    // strcpy(mqttConfig->password, "examplePass");
    strcpy(mqttConfig->host, "127.0.0.1");  
    mqttConfig->port = 1883;  
    mqttConfig->keepAlive = 60;  
    mqttConfig->QOS = 1;  
}