#ifndef __MQTT__H__
#define __MQTT__H__

#include <stdint.h>
#include <string.h>
#include <atomic>
#include <mosquittopp.h>

#include "ak.h"

#include "app.h"
#include "app_dbg.h"

class mqtt : public mosqpp::mosquittopp {
public:
	mqtt(mqttTopicCfg_t *mqtt_parameter, mtce_netMQTT_t *mqtt_config);
	~mqtt() {
		loop_stop(true);	// Force stop thread (true)
		if (lastMsgPtr != NULL) {
			free(lastMsgPtr);
		}
		// disconnect();
		mosqpp::lib_cleanup();
		APP_DBG("~mqtt() called\n");
	}
private:
	// store topic: default it will sub 1 topic when MQTT is connected
	mtce_netMQTT_t m_cfg;
	mqttTopicCfg_t m_topics;
	int m_mid;
	std::atomic<bool> m_connected;
	int lenLastMsg;
	uint8_t *lastMsgPtr;    
};

#endif	  //__MQTT__H__