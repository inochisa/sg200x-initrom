
#ifndef _COMMON_H
#define _COMMON_H

#include "types.h"
#include "romapi.h"
#include "mapping.h"

static inline void __raw_writeb(uint8_t val, volatile void *addr)
{
	asm volatile("sb %0, 0(%1)" : : "r"(val), "r"(addr));
}

static inline void __raw_writew(uint16_t val, volatile void *addr)
{
	asm volatile("sh %0, 0(%1)" : : "r"(val), "r"(addr));
}

static inline void __raw_writel(uint32_t val, volatile void *addr)
{
	asm volatile("sw %0, 0(%1)" : : "r"(val), "r"(addr));
}

static inline uint8_t __raw_readb(const volatile void *addr)
{
	uint8_t val;

	asm volatile("lb %0, 0(%1)" : "=r"(val) : "r"(addr));
	return val;
}

static inline uint16_t __raw_readw(const volatile void *addr)
{
	uint16_t val;

	asm volatile("lh %0, 0(%1)" : "=r"(val) : "r"(addr));
	return val;
}

static inline uint32_t __raw_readl(const volatile void *addr)
{
	uint32_t val;

	asm volatile("lw %0, 0(%1)" : "=r"(val) : "r"(addr));
	return val;
}

#define __io_br()	do {} while (0)
#define __io_ar()	__asm__ __volatile__ ("fence i,r" : : : "memory");
#define __io_bw()	__asm__ __volatile__ ("fence w,o" : : : "memory");
#define __io_aw()	do {} while (0)

#define readb(c)	({ uint8_t  __v; __io_br(); __v = __raw_readb(c); __io_ar(); __v; })
#define readw(c)	({ uint16_t __v; __io_br(); __v = __raw_readw(c); __io_ar(); __v; })
#define readl(c)	({ uint32_t __v; __io_br(); __v = __raw_readl(c); __io_ar(); __v; })

#define writeb(v,c)	({ __io_bw(); __raw_writeb((v),(c)); __io_aw(); })
#define writew(v,c)	({ __io_bw(); __raw_writew((v),(c)); __io_aw(); })
#define writel(v,c)	({ __io_bw(); __raw_writel((v),(c)); __io_aw(); })

#endif /* _COMMON_H */
