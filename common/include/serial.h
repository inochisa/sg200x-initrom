
#ifndef _SERIAL_H
#define _SERIAL_H

#include "common.h"

void fsbl_init_console(unsigned long base, unsigned int uart_clk, unsigned int baud_rate);
void fsbl_putc(uint8_t ch);
int fsbl_getc(void);
void fsbl_flush(void);
int fsbl_tstc(void);
void fsbl_puts(char *str);
int fsbl_snprintf(char *out, unsigned int out_len, const char *format, ...)
	__attribute__ ((format(printf, 3, 4)));
int fsbl_printf(const char *format, ...)  __attribute__ ((format(printf, 1, 2)));

#endif /* _SERIAL_H */
