
#ifndef STAGE0_BOARD_H
#define STAGE0_BOARD_H

#include "types.h"

#define PKG_UNKNOWN			0b000
#define PKG_QFN88			0b001
#define PKG_BGA205			0b010
#define PKG_QFN68			0b011

int parse_board_info(unsigned int pkg_type);

extern uint8_t package_type;

void rtc_init_stage0(void);

#endif /* STAGE0_BOARD_H */
