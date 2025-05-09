
#ifndef _TYPE_H
#define _TYPE_H

#include <stdint.h>
#include <stdbool.h>

typedef unsigned long		size_t;

#define NULL			((void *)0)


#define va_start(v, l)		__builtin_va_start((v), l)
#define va_end			__builtin_va_end
#define va_arg			__builtin_va_arg
typedef __builtin_va_list	va_list;

#endif /* _TYPE_H */
