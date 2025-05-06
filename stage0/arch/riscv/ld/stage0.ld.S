
OUTPUT_FORMAT("elf64-littleriscv")
OUTPUT_ARCH(riscv)
ENTRY(_start)

#define BL2_BASE	0x05200200
#define BL2_SIZE	0x00006000

/*
MEMORY {
	RAM: ORIGIN = BL2_BASE, LENGTH = BL2_SIZE
}
*/

SECTIONS
{
	. = 0x0c000000;
	PROVIDE(_fw_start = .);

	. = ALIGN(0x1000); /* Need this to create proper sections */


	/* Beginning of the code section */

	.text :
 	{
		PROVIDE(_text_start = .);
		*(.entry)
		*(.text)
		*(.text.*)
		. = ALIGN(8);
		PROVIDE(_text_end = .);
	}

	/* End of the code sections */

	. = ALIGN(0x1000); /* Ensure next section is page aligned */

	/* Beginning of the read-only data sections */

	PROVIDE(_rodata_start = .);

	.rodata :
	{
		*(.rodata .rodata.*)
		. = ALIGN(8);
	}

	.dynsym :
	{
		*(.dynsym)
	}

	. = ALIGN(0x1000); /* Ensure next section is page aligned */

	PROVIDE(_rodata_end = .);


	.data :
	{
		PROVIDE(_data_start = .);

		*(.sdata)
		*(.sdata.*)
		*(.data)
		*(.data.*)
		*(.readmostly.data)
		*(*.data)
		. = ALIGN(8);

		PROVIDE(_data_end = .);
	}

	. = ALIGN(0x1000); /* Ensure next section is page aligned */

	.bss :
	{
		PROVIDE(_bss_start = .);
		*(.sbss)
		*(.sbss.*)
		*(.bss)
		*(.bss.*)
		. = ALIGN(8);
		PROVIDE(_bss_end = .);
	}

	/* End of the read-write data sections */

	. = ALIGN(0x1000); /* Need this to create proper sections */

	PROVIDE(_fw_end = .);
}
