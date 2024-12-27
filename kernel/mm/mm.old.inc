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

include "pmm.old.inc"
include "../sys/mmu.inc"

	;; Macro: KV2P
	;; Convert virtual address to physical
macro KV2P reg {
	sub reg, KERNEL_VIRT_BASE
}

	;; Macro: KP2V
	;; Convert physical address to virtual
macro KP2V reg {
	add reg, KERNEL_VIRT_BASE
}

	;; Function: mm_kmap
mm_kmap:
	push esp
	mov ebp, esp
	sub esp, 0x10

	call pmm_alloc_page
	mov [ebp-4], eax

	; clear page dir
	mov edi, eax
	mov ecx, 4096
	xor al, al
	rep stosb

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
	mov edx, [ebp-4]
	add edx, (768 * 4)
	KV2P eax
	mov [edx], eax

	mov eax, [ebp-4]

	leave
	ret

	;; Function: mm_clone_pte
	;;
	;; In:
	;;    EAX - source page table
	;;
	;; Out:
	;;    EAX - cloned page table
	;;
mm_clone_pte:
	push esp
	mov ebp, esp
	sub esp, 0x10

	leave
	ret

	;; Function: mm_clone_pgdir
	;;
	;; In:
	;;    EAX - source page dir
	;;
	;; Out:
	;;    EAX - cloned page dir
mm_clone_pgdir:
	push esp
	mov ebp, esp
	sub esp, 0x10

	mov [ebp-4], eax
	call pmm_alloc_page

	mov [ebp-8], eax
	mov ecx, 4096
	mov edi, eax
	xor al, al
	rep stosb

	xor ecx, ecx
.loop:
	mov eax, [ebp-4]
	add eax, ecx
	mov eax, [eax]
	cmp eax, 0
	je .next
	cmp ecx, (768*4)
	jb @f
	mov edx, [ebp-8]
	add edx, ecx
	mov [edx], eax
	jmp .next
@@:
	mov edx, eax
	call pmm_alloc_page
.next:
	add ecx, 4
	cmp ecx, 4096
	jb .loop

	leave
	ret

	;; Function: mm_init
	;; Initialize the memory manager
mm_init:
	mov esi, szMsgMmInit
	call klog

	call mm_kmap
	mov [pKernelPgDir], eax

	; map whole memory 
	mov esi, 0x400000
	mov ebx, (769 * 4)
.loop:
	call pmm_alloc_page
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
	add edx, ebx
	add ebx, 4
	KV2P eax
	mov [edx], eax

	cmp esi, [stBootInfo.high_mem]
	jb .loop

	; reload cr3
	mov eax, [pKernelPgDir]
	KV2P eax
	mov cr3, eax

	push eax
	mov esi, szMsgMmKernelPgDir
	call klog

	ret

	;; Function: mm_mmap
	;;
	;; In:
	;;   XXX - addr
	;;   XXX - length
	;;   XXX - prot
	;;   XXX - flags
mm_mmap:
	ret

	;; Function: mm_munmap
	;;
	;; In:
	;;    XXX - addr
	;;    XXX - length
	;;
mm_munmap:
	ret

mm_wallk_pagedir:
	ret

szMsgMmInit db "MM: initialize", 0
szMsgMmKernelPgDir db "MM: Kernel page dir at %x phys", 0
pKernelPgDir dd 0
