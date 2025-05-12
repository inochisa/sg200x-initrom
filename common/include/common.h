
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

#define SYS_COUNTER_FREQ		25000000

void udelay(uint32_t usec);

#endif /* _COMMON_H */
