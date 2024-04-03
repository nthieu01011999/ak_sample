#ifndef __APP_DATA_H__
#define __APP_DATA_H__
#include <string>
#include <stdint.h>
#include <stdbool.h>
#include <iostream>
#include <memory>
#include <unordered_map>

#include "sys_boot.h"
#include "helpers.hpp"
// #include "SDCard.h"
#include "app_data.h"
#include "stream.hpp"


/******************************************************************************
 * interface define
 *******************************************************************************/
/* RF24 interface for modules */
#define IF_TYPE_RF24_MT (0)

/* Data type of RF24Network */
#define RF24_DATA_COMMON_MSG_TYPE (1)
#define RF24_DATA_PURE_MSG_TYPE	  (2)
#define RF24_DATA_REMOTE_CMD_TYPE (3)

/* APP interface, communication via socket interface */
#define IF_TYPE_APP_START (100)
#define IF_TYPE_APP_GMNG  (100)
#define IF_TYPE_APP_GW	  (101)
#define IF_TYPE_APP_GU	  (102)
#define IF_TYPE_APP_SVN	  (103)

/******************************************************************************
 * common define
 *******************************************************************************/
typedef struct {
	uint32_t bin_len;
	std::string checksum;
} fileInfo_t;

extern bool checkFileExist(const char *url);
extern std::string mtce_getSerial();
extern mutex clientsMutex;
/******************************************************************************
 * task shell define
 *******************************************************************************/
typedef struct {
	char clientId[30];
	char msg[200];
} msgDatachanel_t;

/******************************************************************************
 * task av define
 *******************************************************************************/
extern void audio_streamStop();

/******************************************************************************
 * webrtc define
 *******************************************************************************/
#define TEST_USE_WEB_SOCKET

template<class T>
std::weak_ptr<T> make_weak_ptr(std::shared_ptr<T> ptr) {
	return ptr;
}

typedef struct {
	std::string stunServerCfg;
	std::string turnServerCfg;

	void clear() {
		stunServerCfg.clear();
		turnServerCfg.clear();
	}
} rtcServersConfig_t;

// extern SDCard SDRecords;
// extern mutex clientsMutex;
// extern ClientsGroup_t clients;
// extern DispatchQueue rtcThread;
// extern optional<shared_ptr<Stream>> avStream;

// extern string getFileName(const string &s);
// extern void sendMsgControlDatachannel(const string &id, const string &msg);

#endif	  //__APP_DATA_H__
