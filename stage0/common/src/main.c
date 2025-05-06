
#include "common.h"

int main(void)
{
	console_init(25000000, 115200);
	console_puts("\n");
	console_puts("hello fip\n");
	console_puts("\n");
	while (1)
		;

	return 0;
}
