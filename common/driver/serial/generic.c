
#include "common.h"
#include "serial.h"


void fsbl_puts(char *str)
{
	if (!str)
		return;

	while (*str) {
		fsbl_putc(*str++);
	}
}
