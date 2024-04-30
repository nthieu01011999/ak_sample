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

 void safe_ak_msg_free(ak_msg_t *msg);

q_msg_t gw_task_webrtc_mailbox;

void *gw_task_webrtc_entry(void *) {

	ak_msg_t *msg = AK_MSG_NULL;

	wait_all_tasks_started();

  
	APP_DBG("[STARTED] gw_task_webrtc_entry\n");
	 
	
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
