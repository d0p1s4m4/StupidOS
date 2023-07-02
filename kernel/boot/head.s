	;; File: head.s

[BITS 32]

%include "machdep.inc"
%include "sys/multiboot.inc"
%include "sys/i386/registers.inc"
%include "sys/i386/mmu.inc"

	;; Define: MB_HDR_FLAGS
MB_HDR_FLAGS equ MB_HDR_ALIGN | MB_HDR_MEMINFO | MB_HDR_VIDEO

section .multiboot.data
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

section .multiboot.text

	;; Function: entry
	;;
	;;
	;; in:
	;;     EAX - Multiboot magic
	;;     EBX - Multiboot structure
	;;
	;; out:
	;;     none
	;;
global entry
entry:
	extern stack_top
	mov esp, V2P(stack_top)

	;; save boot params
	mov edi, eax
	mov esi, ebx

	extern feat_detect
	call feat_detect

	;; setup 4MB paging
	;; TODO: check if 4MB paging is available
	mov eax, cr4
	or eax, CR4_PSE
	mov cr4, eax

	extern page_directory
	mov eax, V2P(page_directory)
	mov cr3, eax

	;; enable paging
	mov eax, cr0
	or eax, CR0_PG | CR0_PE | CR0_WP
	mov cr0, eax

	extern page_directory
	mov eax, page_directory - KERNBASE
	extern kernel_start
	mov ebx, kernel_start
