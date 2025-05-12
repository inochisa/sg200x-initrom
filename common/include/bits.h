
#ifndef _COMMON_BITS_H
#define _COMMON_BITS_H

static inline int __ffs(unsigned long word)
{
	int num = 0;

	if ((word & 0xffffffff) == 0) {
		num += 32;
		word >>= 32;
	}

	if ((word & 0xffff) == 0) {
		num += 16;
		word >>= 16;
	}

	if ((word & 0xff) == 0) {
		num += 8;
		word >>= 8;
	}

	if ((word & 0xf) == 0) {
		num += 4;
		word >>= 4;
	}

	if ((word & 0x3) == 0) {
		num += 2;
		word >>= 2;
	}

	if ((word & 0x1) == 0)
		num += 1;

	return num;
}

#define BIT(n)				(1UL << (n))
#define GENMASK(h, l)			(BIT((h) + 1) - BIT((l)))
#define FIELD_GET(_mask, _reg)		(((_reg) & (_mask)) >> __ffs(_mask))
#define FIELD_PREP(_mask, _value)	(((_value) << __ffs(_mask)) & (_mask))

#endif /* _COMMON_BITS_H */
