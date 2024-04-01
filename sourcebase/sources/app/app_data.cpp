#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "app_config.h"

mtce_context_t mtce_context;

optional<shared_ptr<Stream>> avStream;

// static int mtce_contextStatusSet(char *status);

// int mtce_contextInit() {
// 	int ret = 0;
// 	memset(&mtce_context, 0, sizeof(mtce_context));
// 	mtce_configGetSerial(mtce_context.serial);
// 	// mtce_context.startTime = ipc_system_get_timestamp(); //TODO add timestamp

// 	mtce_contextStatusSet("Disconnected");
// 	return ret;
// }

// int mtce_contextStatusSet(char *status) {
// 	memset(mtce_context.status, 0, sizeof(mtce_context.status));
// 	strcpy(mtce_context.status, status);
// 	return 0;
// }

// string mtce_getSerial() {
// 	return string(mtce_context.serial);
// }

// bool checkFileExist(const char *url) {
// 	int file_obj = -1;
// 	file_obj	 = open(url, O_RDONLY);
// 	if (file_obj < 0) {
// 		return false;
// 	}
// 	else {
// 		close(file_obj);
// 		return true;
// 	}
// }

// string getFileName(const string &s) {
// 	char sep = '/';
// 	size_t i = s.rfind(sep, s.length());
// 	if (i != string::npos) {
// 		return (s.substr(i + 1, s.length() - i));
// 	}

// 	return ("");
// }