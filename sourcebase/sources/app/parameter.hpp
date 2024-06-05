/**
****************************************************************************************
* @author: PND
* @ Modified by: Your name
* @ Modified time: 2023-05-16 14:54:16
* @brief: mtce
*****************************************************************************************
**/
#ifndef __MTCE_PARAMETER_H__
#define __MTCE_PARAMETER_H__

#include <stdbool.h>


#define MTCE_CLIENT_ID_LEN 128
#define MTCE_NAME_LEN 128
#define MTCE_NAME_PASSWORD_LEN 128

typedef struct {
    char clientID[MTCE_CLIENT_ID_LEN];
    char username[MTCE_NAME_LEN];
    char password[MTCE_NAME_PASSWORD_LEN];
    char host[MTCE_NAME_LEN];
    int port;
    int keepAlive;
    int QOS;
} netMQTT_t;

typedef struct {
    char topicStatus[128];
    char topicRequest[128];
    char topicResponse[128]; 
} mqttTopicCfg_t;

 



#endif	  // __MTCE_PARAMETER_H__