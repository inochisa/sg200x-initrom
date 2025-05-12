
#include "common.h"
#include "csr.h"

#define SYS_COUNTER_FREQ_US (SYS_COUNTER_FREQ / 1000000)

static uint64_t get_timer_value(void)
{
	return csr_read(CSR_CYCLE);
}

void udelay(uint32_t usec)
{
	uint64_t start, delta, total;

	start = get_timer_value();

	total = (usec * SYS_COUNTER_FREQ_US);

	do {
		delta = start - get_timer_value();

	} while (delta < total);
}
