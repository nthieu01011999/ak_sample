#ifndef __TASK_WEBRTC_H__
#define __TASK_WEBRTC_H__

#include "message.h"
#include <optional> // Include for std::optional
#include <memory>   // Include for std::shared_ptr
#include <string>   // Include for std::string

#define USER_FPS    13

extern q_msg_t gw_task_webrtc_mailbox;
extern void *gw_task_webrtc_entry(void *);



#endif    //__TASK_WEBRTC_H__
