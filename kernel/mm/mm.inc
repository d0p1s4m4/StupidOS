	;; File: mm.inc
	;; StupidOS Memory Manager

include "pmm.inc"

macro KV2P reg {
	sub reg, KERNEL_VIRT_BASE
}

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
	or edx, 0x3
	mov [edi], edx
	add edi, 4
	add esi, 4096
	inc ecx
	cmp ecx, 1024
	jb @b

	or eax, 0x03
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
