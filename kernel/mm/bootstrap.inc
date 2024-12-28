	;; File: bootstrap.inc
	;; Bootstrap whole PMM and MM

	;; Function: mm_bootstrap
	;; Setup recursive paging.
	;; map page dir at 0xFFFFF000
mm_bootstrap:
	mov esi, szMsgMmBootstrap
	call klog
	; 0x400000
	; PDE entry: 0x7ffc00 phys addr and curr virt addr: 0xc07ffc00

	; map first 4MB at KERNEL_VIRT_BASE
	xor esi, esi
	mov edi, KERNEL_VIRT_BASE + 0x400000 + (768 * PAGE_SIZE)
@@:
	mov edx, esi
	or edx, (PTE_P or PTE_W)
	mov [edi], edx
	add edi, 4
	add esi, PAGE_SIZE
	cmp esi, kend - KERNEL_VIRT_BASE
	jb @b

	; recusive page dir
	mov esi, 0x400000
	mov edi, KERNEL_VIRT_BASE + 0x400000 + (1023 * PAGE_SIZE)
	xor ecx, ecx
@@:
	mov edx, esi
	or edx, (PTE_P or PTE_W)
	mov [edi], edx
	add edi, 4
	add esi, PAGE_SIZE
	inc ecx
	cmp ecx, 1024
	jb @b

	mov eax, 0x400000 + (1023 * PAGE_SIZE)
	mov cr3, eax

	ret

szMsgMmBootstrap db "MM: boostrap", 0
