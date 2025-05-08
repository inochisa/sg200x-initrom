
#ifndef _SERIAL_H
#define _SERIAL_H

#include "common.h"

void fsbl_init_console(unsigned long base, unsigned int uart_clk, unsigned int baud_rate);
void fsbl_putc(uint8_t ch);
int fsbl_getc(void);
void fsbl_flush(void);
int fsbl_tstc(void);
void fsbl_puts(char *str);

#endif /* _SERIAL_H */
