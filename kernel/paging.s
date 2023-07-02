[BITS 32]

PE_PRESENT equ 1 << 0
PE_WRITABLE equ 1 << 1
PE_USERMODE equ 1 << 2
PE_ACCESSED equ 1 << 5
PE_DIRTY equ 1 << 6

section .bss
align 4096
global page_directory
page_directory:
	resb 4096
global boot_page_table
boot_page_table:
	resb 4096
