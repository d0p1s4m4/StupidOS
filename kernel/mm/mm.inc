
include "pmm.inc"

mm_init:
	mov esi, szMsgMmInit
	call klog

	call pmm_alloc_page
	mov [pKernelPgDir], eax

	push eax
	mov esi, szMsgMmKernelPgDir
	call klog

	ret

szMsgMmInit db "MM: initialize", 0
szMsgMmKernelPgDir db "MM: Kernel page dir at %x", 0
pKernelPgDir dd ?
