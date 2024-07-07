	;; File: mm.inc
	;; StupidOS Memory Manager

include "pmm.inc"

mm_init:
	mov esi, szMsgMmInit
	call klog

	call pmm_alloc_page
	mov [pKernelPgDir], eax

	push eax
	mov esi, szMsgMmKernelPgDir
	call klog

	; clear page dir
	mov ecx, 4096
	xor al, al
	mov edi, [pKernelPgDir]
	rep stosb

	;; Map kernel and kmemory
	call pmm_alloc_page
	push eax
	mov ecx, 4096
	mov edi, eax
	xor al, al
	rep stosb
	pop eax

	;; Map video


	;; Map free memory

	; mov eax, [pKernelPgDir]
	; mov cr3, eax

	ret

mm_wallk_pagedir:
	ret

szMsgMmInit db "MM: initialize", 0
szMsgMmKernelPgDir db "MM: Kernel page dir at %x", 0
pKernelPgDir dd ?
