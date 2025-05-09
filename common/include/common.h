
#ifndef _COMMON_H
#define _COMMON_H

#include "types.h"
#include "romapi.h"
#include "mapping.h"
#include "io.h"

#define readb(c)	({ uint8_t  __v; __io_br(); __v = __raw_readb((volatile void *)c); __io_ar(); __v; })
#define readw(c)	({ uint16_t __v; __io_br(); __v = __raw_readw((volatile void *)c); __io_ar(); __v; })
#define readl(c)	({ uint32_t __v; __io_br(); __v = __raw_readl((volatile void *)c); __io_ar(); __v; })

#define writeb(v,c)	({ __io_bw(); __raw_writeb((v),(volatile void *)(c)); __io_aw(); })
#define writew(v,c)	({ __io_bw(); __raw_writew((v),(volatile void *)(c)); __io_aw(); })
#define writel(v,c)	({ __io_bw(); __raw_writel((v),(volatile void *)(c)); __io_aw(); })

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

#define GENMASK(h, l)			((1UL << ((h) + 1)) - (1UL << (l)))
#define FIELD_GET(_mask, _reg)		(((_reg) & (_mask)) >> __ffs(_mask))
#define FIELD_PREP(_mask, _value)	(((_value) << __ffs(_mask)) & (_mask))

#endif /* _COMMON_H */
