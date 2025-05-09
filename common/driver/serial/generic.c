
#include "common.h"
#include "serial.h"
#include "string.h"

void fsbl_puts(char *str)
{
	if (!str)
		return;

	while (*str) {
		fsbl_putc(*str++);
	}
}

#define PAD_RIGHT		(1 << 0)
#define PAD_ZERO		(1 << 1)
#define PAD_ALTERNATE		(1 << 2)
#define PAD_SIGN		(1 << 3)

static void printc(char **out, unsigned int *out_len, char ch, int flags)
{
	if (!out) {
		fsbl_putc(ch);
		return;
	}

	if (!out_len || *out_len > 1) {
		*(*out)++ = ch;
		**out = '\0';
		if (out_len) {
			--(*out_len);
		}
	}
}

static int prints(char **out, unsigned int *out_len, const char *string, int width,
		  int flags)
{
	int pc = 0;
	width -= strlen(string);
	if (!(flags & PAD_RIGHT)) {
		for (; width > 0; --width) {
			printc(out, out_len, flags & PAD_ZERO ? '0' : ' ', flags);
			++pc;
		}
	}
	for (; *string; ++string) {
		printc(out, out_len, *string, flags);
		++pc;
	}
	for (; width > 0; --width) {
		printc(out, out_len, ' ', flags);
		++pc;
	}

	return pc;
}

#define PRINT_BUF_LEN 64

static int printi(char **out, unsigned int *out_len, long long i,
		  int width, int flags, int type)
{
	int pc = 0;
	char *s, sign = 0, letbase, print_buf[PRINT_BUF_LEN];
	unsigned long long u, b, t;

	b = 10;
	letbase = 'a';
	if (type == 'o')
		b = 8;
	else if (type == 'x' || type == 'X' || type == 'p' || type == 'P') {
		b = 16;
		letbase &= ~0x20;
		letbase |= type & 0x20;
	}

	u = i;
	sign = 0;
	if (type == 'i' || type == 'd') {
		if ((flags & PAD_SIGN) && i > 0)
			sign = '+';
		if (i < 0) {
			sign = '-';
			u = -i;
		}
	}

	s  = print_buf + PRINT_BUF_LEN - 1;
	*s = '\0';

	if (!u) {
		*--s = '0';
	} else {
		while (u) {
			t = u % b;
			u = u / b;
			if (t >= 10)
				t += letbase - '0' - 10;
			*--s = t + '0';
		}
	}

	if (flags & PAD_ZERO) {
		if (sign) {
			printc(out, out_len, sign, flags);
			++pc;
			--width;
		}
		if (i && (flags & PAD_ALTERNATE)) {
			if (b == 16 || b == 8) {
				printc(out, out_len, '0', flags);
				++pc;
				--width;
			}
			if (b == 16) {
				printc(out, out_len, 'x' - 'a' + letbase, flags);
				++pc;
				--width;
			}
		}
	} else {
		if (i && (flags & PAD_ALTERNATE)) {
			if (b == 16)
				*--s = 'x' - 'a' + letbase;
			if (b == 16 || b == 8)
				*--s = '0';
		}
		if (sign)
			*--s = sign;
	}

	return pc + prints(out, out_len, s, width, flags);
}

static int print(char **out, unsigned int *out_len, const char *format, va_list args)
{
	bool flags_done;
	int width, flags, pc = 0;
	char type, scr[2], *tout;
	uintptr_t num;

	/* handle special case: *out_len == 1*/
	if (out) {
		if(!out_len || *out_len)
			**out = '\0';
	}

	for (; *format != 0; ++format) {
		width = flags = 0;

		if (*format != '%') {
raw:
			printc(out, out_len, *format, flags);
			++pc;
			continue;
		}

		++format;

		if (*format == '\0')
			break;
		if (*format == '%')
			goto raw;

		/* Get flags */
		flags_done = false;
		while (!flags_done) {
			switch (*format) {
			case '-':
				flags |= PAD_RIGHT;
				break;
			case '+':
				flags |= PAD_SIGN;
				break;
			case '#':
				flags |= PAD_ALTERNATE;
				break;
			case '0':
				flags |= PAD_ZERO;
				break;
			case ' ':
			case '\'':
				/* Ignored flags, do nothing */
				break;
			default:
				flags_done = true;
				break;
			}
			if (!flags_done)
				++format;
		}

		if (flags & PAD_RIGHT)
			flags &= ~PAD_ZERO;

		/* Get width */
		for (; *format >= '0' && *format <= '9'; ++format) {
			width *= 10;
			width += *format - '0';
		}

		if (*format == 's') {
			char *s = va_arg(args, char *);
			pc += prints(out, out_len, s ? s : "(null)",
					width, flags);
			continue;
		}

		if (*format == 'c') {
			/* char are converted to int then pushed on the stack */
			scr[0] = va_arg(args, int);
			scr[1] = '\0';
			pc += prints(out, out_len, scr, width, flags);
			continue;
		}

		type = '\0';

		if ((*format == 'p') || (*format == 'P')) {
			num = (uintptr_t)va_arg(args, void *);
			type = *format;
			continue;
		} else if ((*format == 'd') || (*format == 'i')) {
			num = (uintptr_t)va_arg(args, int);
			type = *format;
			continue;
		} else if ((*format == 'u') || (*format == 'o') ||
			   (*format == 'x') || (*format == 'X')) {
			num = (uintptr_t)va_arg(args, unsigned int);
			type = *format;
			continue;
		} else if (*format == 'l') {
			if (format[1] == 'l') {
				++format;
				if ((format[1] == 'd') || (format[1] == 'i') ||
				    (format[1] == 'u') || (format[1] == 'o') ||
				    (format[1] == 'x') || (format[1] == 'X')) {
					num = va_arg(args, long long);
					++format;
					type = *format;
				}
			} else if ((format[1] == 'd') || (format[1] == 'i')) {
				num = va_arg(args, long);
				type = *format;
			} else if ((format[1] == 'u') || (format[1] == 'o') ||
				   (format[1] == 'x') || (format[1] == 'X')) {
				num = va_arg(args, unsigned long);
				type = *format;
			}
		}

		if (type != '\0')
			pc += printi(out, out_len, num, width, flags, type);
	}

	return pc;
}

int fsbl_snprintf(char *out, unsigned int out_len, const char *format, ...)
{
	va_list args;
	int retval;

	va_start(args, format);
	retval = print(&out, &out_len, format, args);
	va_end(args);

	return retval;
}

int fsbl_printf(const char *format, ...)
{
	va_list args;
	int retval;

	va_start(args, format);
	retval = print(NULL, NULL, format, args);
	va_end(args);

	return retval;
}
