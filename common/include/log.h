
#ifndef _LOG_H
#define _LOG_H

#include "serial.h"

#ifndef FSBL_LOG_LEVEL
#define FSBL_LOG_LEVEL	3
#endif

#define LOG_ERR_ID		0
#define LOG_WARN_ID		1
#define LOG_INFO_ID		2
#define LOG_DEBUG_ID		3

extern const char *fsbl_color[];
extern const char *fsbl_color_break;

#ifndef FSBL_NO_COLOR_LOG
#define FSBL_LOG_SET_COLOR(id)		fsbl_puts(fsbl_color[id])
#define FSBL_LOG_RESET_COLOR()		fsbl_puts(fsbl_color_break)
#else
#endif


#if FSBL_LOG_LEVEL > 0
#define ERR(format, ...) 				\
	do {						\
		FSBL_LOG_SET_COLOR(LOG_ERR_ID);		\
		fsbl_printf(format,## __VA_ARGS__);	\
		FSBL_LOG_RESET_COLOR();			\
	} while (0)
#else
#define ERR(format, ...)
#endif

#if FSBL_LOG_LEVEL > 1
#define WARN(format, ...) 				\
	do {						\
		FSBL_LOG_SET_COLOR(LOG_WARN_ID);	\
		fsbl_printf(format,## __VA_ARGS__);	\
		FSBL_LOG_RESET_COLOR();			\
	} while (0)
#else
#define WARN(format, ...)
#endif

#if FSBL_LOG_LEVEL > 2
#define INFO(format, ...) 				\
	do {						\
		FSBL_LOG_SET_COLOR(LOG_INFO_ID);	\
		fsbl_printf(format,## __VA_ARGS__);	\
		FSBL_LOG_RESET_COLOR();			\
	} while (0)
#else
#define INFO(format, ...)
#endif

#if FSBL_LOG_LEVEL > 4
#define DEBUG(format, ...) 				\
	do {						\
		FSBL_LOG_SET_COLOR(LOG_DEBUG_ID);	\
		fsbl_printf(format,## __VA_ARGS__);	\
		FSBL_LOG_RESET_COLOR();			\
	} while (0)
#else
#define DEBUG(format, ...)
#endif

#endif /* _LOG_H */
