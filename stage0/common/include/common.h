
#ifndef _COMMON_H
#define _COMMON_H

#include "types.h"
#include "romapi.h"

int console_init(unsigned int uart_clk, unsigned int baud_rate);
int console_putc(int c);
int console_getc(void);
int console_flush(void);
int console_tstc(void);
void console_puts(char *str);

#endif /* _COMMON_H */
