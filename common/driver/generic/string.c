
#include "string.h"

size_t strlen(const char *str)
{
	unsigned long ret = 0;

	while (*str != '\0') {
		ret++;
		str++;
	}

	return ret;
}
