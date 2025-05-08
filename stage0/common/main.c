
#include "common.h"
#include "serial.h"

int main(void)
{
	fsbl_init_console(UART0_BASE, 25000000, 115200);
	fsbl_puts("\n");
	fsbl_puts("hello fip\n");
	fsbl_puts("\n");

	while (1)
		;

	return 0;
}
