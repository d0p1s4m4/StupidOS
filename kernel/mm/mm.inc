
	;; File: mm.inc
include "pmm.inc"

mm_init:
	mov esi, szMsgMmInit
	call klog

	call pmm_alloc_page
	mov [pKernelPgDir], eax

	push eax
	mov esi, szMsgMmKernelPgDir
	call klog

	;; Map kernel
	
	;; Map video

	;; Map low memory at ????

	;; Map free memory

	; mov eax, [pKernelPgDir]
	; mov cr3, eax

	ret

mm_wallk_pagedir:
	ret

szMsgMmInit db "MM: initialize", 0
szMsgMmKernelPgDir db "MM: Kernel page dir at %x", 0
pKernelPgDir dd ?
