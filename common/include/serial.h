
#ifndef _SERIAL_H
#define _SERIAL_H

int console_init(unsigned int uart_clk, unsigned int baud_rate);
int console_putc(int c);
int console_getc(void);
int console_flush(void);
int console_tstc(void);
void console_puts(char *str);

#endif /* _SERIAL_H */
