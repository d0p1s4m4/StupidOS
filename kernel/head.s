; file: head.s
; 
[BITS 32]

%include "base.inc"
%include "multiboot.inc"

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

	; Function: entry
	; 
	; in:
	;     EAX - Multiboot magic
	;     EBX - Multiboot structure
	;
	; out:
	;     none
	;
global entry
entry:
	mov esp, stack_top
	cli

	mov edi, eax
	mov esi, ebx

	;; initialize serial (mostly used for debug)
	extern serial_init
	call serial_init

	;; eax <- magic
	;; ebx <- multiboot struct
	cmp edi, 0x2BADB002
	je .multiboot_valid

	LOG err_invalid_boot_magic, edi
	jmp hang
	
.multiboot_valid:
	
	LOG msg_hello_world
	mov eax, [esi + mb_info.bootloader_name]
	LOG msg_boot_info, eax
	
	extern setup_gdt
	call setup_gdt

	extern setup_pic
	call setup_pic

	extern setup_idt
	call setup_idt

	push esi
	extern setup_pmm
	call setup_pmm
	add esp, 4
	test eax, 0
	jz .mem_ok

	LOG err_cannot_map_memory

	jmp hang

.mem_ok:
	LOG msg_pmm_initialized

	extern setup_paging
	call setup_paging

hang:
	cli
	hlt
	jmp hang
	
section .rodata

msg_hello_world db "StupidOS v", STUPID_VERSION, " (built with ", NASM_VERSION, " on ", BUILD_DATE, ")", 0
msg_boot_info db "Bootloader: %s", 0
msg_pmm_initialized db "PMM initialized", 0
err_invalid_boot_magic db "[ERROR] Invalid boot magic (got: %x, expected: 0x2BADB002)", 0
err_cannot_map_memory db "[ERROR] Can't map memory", 0

file db __FILE__, 0
