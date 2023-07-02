OUTPUT_ARCH(i386)
ENTRY(entry)

SECTIONS
{
	. = 1M;
	
	kernel_start = .;

	.multiboot : {
		KEEP(*(.multiboot.data))
		KEEP(*(.multiboot.text))
	}

	. += 0xC0100000;

	.text ALIGN(0x1000) : AT(ADDR(.text) - 0xC0000000) {
		*(.text)
	}

	.rodata ALIGN(0x1000) : AT(ADDR(.rodata) - 0xC0000000) {
		*(.rodata)
	}

	.data ALIGN(0x1000) : AT(ADDR(.data) - 0xC0000000) {
		*(.data)
	}

	.bss ALIGN(0x1000) : AT(ADDR(.bss) - 0xC0000000) {
		bss_start = .;
		*(COMMON)
		*(.bss)
		bss_end = .;
	}

	kernel_end = .;
}

kernel_size = kernel_end - kernel_start;
