	;; File: mm.inc
	;; StupidOS Memory Manager
	;;
	;; About: Memory Layout
	;;
	;; >                 Virtual
	;; > 0xFFFFFFFF +---------------+
	;; >            |               |
	;; >            | Device Memory |
	;; >            |               |
	;; >            +---------------+
	;; >            |               |
	;; >            | Kernel Heap   |
	;; >            |               |
	;; >   KERN_END +---------------+
	;; >            |               |
	;; >            | Stupid Kernel |
	;; >            |               |
	;; > 0xC0100000 +---------------+
	;; >            | I/O Space and |
	;; >            | phys memory   |
	;; >            | Lower than 1M | kernel mode only
	;; > 0xC0000000 +---------------+
	;; >            |               | user mode
	;; >            |  userspace    |
	;; >            |               |
	;; > 0x00000000 +---------------+
	;;

include "pmm.inc"
include "../sys/mmu.inc"

	;; Macro: KV2P
	;; Convert virtual address to physical
macro KV2P reg {
	sub reg, KERNEL_VIRT_BASE
}

	;; Macro: KP2V
macro KP2V reg {
	add reg, KERNEL_VIRT_BASE
}

mm_kmap:
	ret

	;; Function: mm_init
mm_init:
	mov esi, szMsgMmInit
	call klog

	call pmm_alloc_page
	mov [pKernelPgDir], eax

	; clear page dir
	mov ecx, 4096
	xor al, al
	mov edi, [pKernelPgDir]
	rep stosb

	;; Map kernel and kmemory
	call pmm_alloc_page
	xor esi, esi
	xor ecx, ecx
	mov edi, eax
@@:
	mov edx, esi
	or edx, (PTE_P or PTE_W)
	mov [edi], edx
	add edi, 4
	add esi, 4096
	inc ecx
	cmp ecx, 1024
	jb @b

	or eax, (PDE_P or PDE_W)
	mov edx, [pKernelPgDir]
	add edx, (768 * 4)
	KV2P eax
	mov [edx], eax

	;; Map free memory

	mov eax, [pKernelPgDir]
	KV2P eax
	mov cr3, eax

	push eax
	mov esi, szMsgMmKernelPgDir
	call klog

	ret

mm_wallk_pagedir:
	ret

szMsgMmInit db "MM: initialize", 0
szMsgMmKernelPgDir db "MM: Kernel page dir at %x phys", 0
pKernelPgDir dd 0
