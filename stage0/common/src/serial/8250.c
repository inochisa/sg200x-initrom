
#include "common.h"

#define UART_LCR_WLS_MSK 0x03       /* character length select mask */
#define UART_LCR_WLS_5  0x00        /* 5 bit character length */
#define UART_LCR_WLS_6  0x01        /* 6 bit character length */
#define UART_LCR_WLS_7  0x02        /* 7 bit character length */
#define UART_LCR_WLS_8  0x03        /* 8 bit character length */
#define UART_LCR_STB    0x04        /* # stop Bits, off=1, on=1.5 or 2) */
#define UART_LCR_PEN    0x08        /* Parity eneble */
#define UART_LCR_EPS    0x10        /* Even Parity Select */
#define UART_LCR_STKP   0x20        /* Stick Parity */
#define UART_LCR_SBRK   0x40        /* Set Break */
#define UART_LCR_BKSE   0x80        /* Bank select enable */
#define UART_LCR_DLAB   0x80        /* Divisor latch access bit */

#define UART_MCR_DTR    0x01        /* DTR   */
#define UART_MCR_RTS    0x02        /* RTS   */

#define UART_LSR_THRE   0x20 /* Transmit-hold-register empty */
#define UART_LSR_DR	    0x01 /* Receiver data ready */
#define UART_LSR_TEMT   0x40        /* Xmitter empty */

#define UART_FCR_FIFO_EN    0x01 /* Fifo enable */
#define UART_FCR_RXSR       0x02 /* Receiver soft reset */
#define UART_FCR_TXSR       0x04 /* Transmitter soft reset */

#define UART_MCRVAL (UART_MCR_DTR | UART_MCR_RTS)      /* RTS/DTR */
#define UART_FCR_DEFVAL	(UART_FCR_FIFO_EN | UART_FCR_RXSR | UART_FCR_TXSR)
#define UART_LCR_8N1    0x03

static struct dw_regs *uart = (struct dw_regs *)UART0_BASE;

#define UART_RBR_OFFSET		0x00	/* In:  Recieve Buffer Register */
#define UART_THR_OFFSET		0x00	/* Out: Transmitter Holding Register */
#define UART_DLL_OFFSET		0x00	/* Out: Divisor Latch Low */
#define UART_IER_OFFSET		0x04	/* I/O: Interrupt Enable Register */
#define UART_DLM_OFFSET		0x04	/* Out: Divisor Latch High */
#define UART_FCR_OFFSET		0x08	/* Out: FIFO Control Register */
#define UART_IIR_OFFSET		0x08	/* I/O: Interrupt Identification Register */
#define UART_LCR_OFFSET		0x0c	/* Out: Line Control Register */
#define UART_MCR_OFFSET		0x10	/* Out: Modem Control Register */
#define UART_LSR_OFFSET		0x14	/* In:  Line Status Register */
#define UART_MSR_OFFSET		0x18	/* In:  Modem Status Register */
#define UART_SCR_OFFSET		0x1c	/* I/O: Scratch Register */
#define UART_MDR1_OFFSET	0x20	/* I/O:  Mode Register */

static void *uart_base = (void *)UART0_BASE;

void console_init(unsigned int uart_clk, unsigned int baud_rate)
{
	uint32_t val;
	int baudrate = baud_rate;
	int uart_clock = uart_clk;

	int divisor = uart_clock / (16 * baudrate);

	val = readl(uart_base + UART_LCR_OFFSET);
	writel(val | UART_LCR_DLAB | UART_LCR_8N1, uart_base + UART_LCR_OFFSET);

	writel(divisor & 0xff, uart_base + UART_DLL_OFFSET);
	writel((divisor >> 8) & 0xff, uart_base + UART_DLM_OFFSET);

	val = readl(uart_base + UART_LCR_OFFSET);
	writel(val & (~UART_LCR_DLAB), uart_base + UART_LCR_OFFSET);

	writel(0x00, uart_base + UART_IER_OFFSET);
	writel(UART_MCRVAL, uart_base + UART_MCR_OFFSET);
	writel(UART_FCR_DEFVAL, uart_base + UART_FCR_OFFSET);

	writel(0x03, uart_base + UART_LCR_OFFSET);
}

void _uart_putc(uint8_t ch)
{
	while ((readl(uart_base + UART_LSR_OFFSET) & UART_LSR_THRE) == 0)
		;

	writel(ch, uart_base + UART_RBR_OFFSET);
}

void console_putc(uint8_t ch)
{
	if (ch == '\n') {
		_uart_putc('\r');
	}
	_uart_putc(ch);
}

void console_puts(char *str)
{
	if (!str)
		return;

	while (*str) {
		console_putc(*str++);
	}
}

void console_flush(void)
{
	uint32_t mask = UART_LSR_THRE | UART_LSR_TEMT;

	while ((readl(uart_base + UART_LSR_OFFSET) & mask) != mask)
		;
}

int console_getc(void)
{
	while ((readl(uart_base + UART_LSR_OFFSET) & UART_LSR_DR) == 0)
		;

	return readl(uart_base + UART_RBR_OFFSET);
}

int uart_tstc(void)
{
	return (!!(readl(uart_base + UART_LSR_OFFSET) & UART_LSR_DR));
}

int console_tstc(void)
{
	return uart_tstc();
}
