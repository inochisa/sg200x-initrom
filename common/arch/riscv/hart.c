
#include "common.h"

__attribute__((noreturn)) void initrom_hang(void)
{
	while (1)
		__asm__ __volatile__("wfi" ::: "memory");
	__builtin_unreachable();
}
