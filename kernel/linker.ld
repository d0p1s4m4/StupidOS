OUTPUT_ARCH(i386)
ENTRY(entry)

SECTIONS
{
	. = 1M;
	kernel_start = .;

	.text : ALIGN(4) {
		KEEP(*(.multiboot))
		*(.text)
	}

	.rodata : ALIGN(4) {
		*(.rodata)
	}

	.data : ALIGN(4) {
		*(.data)
	}

	.bss : ALIGN(4) {
		*(COMMON)
		*(.bss)
	}

	kernel_end = .;
}
