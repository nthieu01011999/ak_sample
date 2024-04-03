#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "app_config.h"

mtce_context_t mtce_context;

string mtce_getSerial() {
	return string(mtce_context.serial);
}