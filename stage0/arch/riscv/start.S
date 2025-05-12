#include "mapping.h"
#include "csr.h"

	.section .text, "ax", %progbits
	.align 3
	.globl _start
_start:
	li	x1, 0
	li	x2, 0
	li	x3, 0
	li	x4, 0
	li	x5, 0
	li	x6, 0
	li	x7, 0
	li	x8, 0
	li	x9, 0
	li	x10, 0
	li	x11, 0
	li	x12, 0
	li	x13, 0
	li	x14, 0
	li	x15, 0
	li	x16, 0
	li	x17, 0
	li	x18, 0
	li	x19, 0
	li	x20, 0
	li	x21, 0
	li	x22, 0
	li	x23, 0
	li	x24, 0
	li	x25, 0
	li	x26, 0
	li	x27, 0
	li	x28, 0
	li	x29, 0
	li	x30, 0
	li	x31, 0

	csrw	CSR_MSCRATCH, x0

	# write mtvec and make sure it sticks
	la	t0, _trap
	csrw	CSR_MTVEC, t0

	# set mxstatus to init value
	li	t0, 0xc0638000
	csrw	0x7c0, t0

	# invalid I-cache
	li	t0, 0x33
	csrc	0x7c2, t0
	li	t0, 0x11
	csrs	0x7c2, t0
	# enable I-cache
	li	t0, 0x1
	csrs	0x7c1, t0
	# invalid D-cache
	li	t0, 0x33
	csrc	0x7c2, t0
	li	t0, 0x12
	csrs	0x7c2, t0
	# enable D-cache
	li	t0, 0x2
	csrs	0x7c1, t0

	li	sp, 0x05200400
	// la	sp, _fw_end

	la	a3, _bss_start
	la	a4, _bss_end
	sub	a4, a4, a3

bss_clear:
	sd	x0, 0(a3)
	addi	a3, a3, 8
	addi	a4, a4, -8
	bnez	a4, bss_clear

	call	main

_trap:
_die:
	j	_die
