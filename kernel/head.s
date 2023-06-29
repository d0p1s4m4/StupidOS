; file: head.s
;
[BITS 32]

%include "base.inc"
%include "multiboot.inc"

; Define: MB_HDR_FLAGS 
MB_HDR_FLAGS equ MB_HDR_ALIGN | MB_HDR_MEMINFO | MB_HDR_VIDEO

section .multiboot
align 4
	istruc mb_header
		at mb_header.magic,    dd MB_HDR_MAGIC
		at mb_header.flags,    dd MB_HDR_FLAGS
		at mb_header.checksum, dd -(MB_HDR_MAGIC + MB_HDR_FLAGS)

		; video mode info
		at mb_header.mode_type, dd 0
		at mb_header.width,     dd 1024
		at mb_header.height,    dd 768
		at mb_header.depth,     dd 32
	iend

section .bss
align 16
stack_bottom:
	resb 16384
stack_top:

section .text

	; Function: entry
	; setup cpu and paging before calling <kmain>
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
	cli ; disable interrupt

	mov edi, eax ; save multiboot magic in EDI
	mov esi, ebx ; save multiboot structure in ESI

	;; initialize serial (mostly used for debug)
	extern serial_init
	call serial_init

	;; make sure we use a multiboot compliant bootloader
	cmp edi, MB_MAGIC
	je .multiboot_valid

	LOG err_invalid_boot_magic, edi
	jmp hang
	
.multiboot_valid:
	
	LOG msg_hello_world
	mov eax, [esi + mb_info.bootloader_name]
	LOG msg_boot_info, eax
	
	extern setup_gdt
	call setup_gdt

	;extern setup_pic
	;call setup_pic
	mov al, 0xff
	out 0xa1, al
	out 0x21, al

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

msg_hello_world db "StupidOS v", STUPID_VERSION, " (built with ", __NASM_VER__, " on ",  __DATE__, " ", __TIME__, ")", 0
msg_boot_info db "Bootloader: %s", 0
msg_pmm_initialized db "PMM initialized", 0
err_invalid_boot_magic db "[ERROR] Invalid boot magic (got: %x, expected: 0x2BADB002)", 0
err_cannot_map_memory db "[ERROR] Can't map memory", 0

file db __FILE__, 0
