[BITS 32]

%include "base.inc"

MULTIBOOT_MAGIC equ 0x1BADB002
MULTIBOOT_ALIGN equ 1 << 0
MULTIBOOT_MEMINFO equ 1 << 1
MULTIBOOT_FLAGS equ MULTIBOOT_ALIGN | MULTIBOOT_MEMINFO

section .multiboot
align 4
	dd MULTIBOOT_MAGIC
	dd MULTIBOOT_FLAGS
	dd -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

section .bss
align 16
stack_bottom:
	resb 16384
stack_top:

section .text
global entry
entry:
	mov esp, stack_top

	cli

	extern serial_init
	call serial_init

	LOG msg_hello_world

	extern setup_gdt
	call setup_gdt

	extern setup_pic
	call setup_pic

	extern setup_idt
	call setup_idt

	;extern setup_paging
	;call setup_paging

	int3

	LOG file

	cli
hang:
	hlt
	jmp hang


section .rodata

msg_hello_world db "StupidOS ", STUPID_VERSION, 0
file db __FILE__, 0
