
#include "log.h"

const char *fsbl_color_break = "\033[0m";

const char *fsbl_color[] = {
	[LOG_ERR_ID]	= "\033[1;31m",
	[LOG_WARN_ID]	= "\033[1;33m",
	[LOG_INFO_ID]	= "\033[1;34m",
	[LOG_DEBUG_ID]	= "\033[1m",
};
