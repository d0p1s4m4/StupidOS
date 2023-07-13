	;; File: head.s
	;;
	;; About: CPU compatibility
	;; /!\ Only work on *486+* for now.
	;; - `invlpg` which is not part of 386 ISA.
	;;
[BITS 32]

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
	;; Setup boot page table, map kernel to higher half
	;; then jump to <entry_high>
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
	;; disable interrupt
	cli

	;; save boot params
	mov edi, eax
	mov esi, ebx

	;; setup 4MB paging
	;; TODO: check if 4MB paging is available
	mov eax, cr4
	or eax, CR4_PSE
	mov cr4, eax

	cmp edi, MB_MAGIC
	jne .skip_map_multiboot
	;; check if multiboot struct is in first 4Mib
	;; otherwith add entry in page dir
	mov eax, 400000
	cmp ebx, eax
	jg .map_multiboot
	jmp .skip_map_multiboot
.map_multiboot:
	;; TODO: for now let's assume data are bellow 4Mib 
	jmp .end
.skip_map_multiboot:
	add esi, KERNBASE
.end:
	extern page_directory
	mov eax, V2P(boot_page_dir)
	mov cr3, eax

	;; enable paging
	mov eax, cr0
	or eax, CR0_PG | CR0_PE | CR0_WP
	mov cr0, eax

	;; Jump to higher half
	lea eax, entry_high
	jmp eax						; near jump, indirect

section .text

	;; Function: entry_high
	;; Invalidate page[0], setup stack then call <kmain>
entry_high:
	;; unmap first 4MiB, since it's not needed anymore
	mov dword [boot_page_dir], 0
	invlpg [0]

	;; Setup stack
	extern stack_top
	mov esp, stack_top
	xor ebp, ebp

	push esi					; multiboot struct
	push edi					; multiboot magic
	extern kmain
	call kmain

	cli
hang:
	hlt
	jmp hang

section .data
align 0x1000
boot_page_dir:
	dd 0 | PDE_P | PDE_W | PDE_PS
	times (P2PDE(KERNBASE) - 1) dd 0
	dd 0 | PDE_P | PDE_W | PDE_PS
	times 0x400 - (boot_page_dir - $$) dd 0
