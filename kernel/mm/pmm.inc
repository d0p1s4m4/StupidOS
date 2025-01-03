	;; File: pmm.inc
	;;
	;; Our PMM is just a linked list of page-aligned size blocks. 
	;;
	;; > ┌─────┐      ┌─────┐       
	;; > │size │  ┌──►│size │       
	;; > │next ├──┘   │next ├────► 0
	;; > └─────┘      └─────┘       
	;;

	;; Macro: PAGE_ALIGN_UP reg
macro PAGE_ALIGN_UP reg {
	add reg, PAGE_SIZE - 1
	and reg, -PAGE_SIZE
}

	;; Macro: PAGE_ALIGN_DOWN reg
macro PAGE_ALIGN_DOWN reg {
	and reg, -PAGE_SIZE
}
	
	;; Struc: PMMFreeRange
struc PMMFreeRange {
	.size dd ?
	.next dd ?
}
DEFN PMMFreeRange

	;; Function: _pmm_map_temp_page
	;;
_pmm_map_temp_page:
	ret

_pmm_unmap_temp_page:
	ret

_pmm_init_region:
	push ebp
	mov ebp, esp

	push eax
	call _pmm_map_temp_page
	pop eax

	xor ecx, ecx
	mov [MM_VIRT_TEMP], edx
	mov [MM_VIRT_TEMP + PMMFreeRange.next], ecx
	mov ecx, [pPMMFreeListHead]
	or ecx, ecx
	jz @f
	mov [MM_VIRT_TEMP + PMMFreeRange.next], ecx
@@:
	mov [pPMMFreeListHead], eax

	call _pmm_unmap_temp_page

	leave
	ret

	;; Function: pmm_alloc
	;; 
	;; In:
	;;    EAX - size in page
	;;
	;; Out:
	;;    EAX - first page physical address, 0 on error
pmm_alloc:
	ret

	;; Function: pmm_alloc_page
	;;
	;; Out:
	;;    EAX - page physical address, 0 on error
pmm_alloc_page:
	mov eax, 1
	call pmm_alloc
	ret

	;; Function: pmm_free
	;; 
	;; In:
	;;    EAX - Start
	;;    EDX - End
pmm_free:
	ret

	;; Function: _pmm_init_region
	;;
	;; Add new memory region to the linked list.
	;;
	;; In:
	;;    EAX - Start
	;;    EDX - End
	;;

	;; Function: pmm_init
	;; 
	;; Out:
	;;    EAX - return -1 on error
pmm_init:
	mov eax, szMsgMmInit
	call klog

	mov eax, kend
	PAGE_ALIGN_UP eax

	mov edx, 0x400000
	sub edx, eax
	jle @f
	cmp edx, PAGE_SIZE
	jle @f

	mov edx, 0x400000
	call _pmm_init_region

@@:
	mov eax, 0x800000
	mov edx, [stBootInfo.high_mem]
	PAGE_ALIGN_DOWN edx
	call _pmm_init_region

	xor eax, eax ; TODO: check if enough memory and so on
	ret

	;; Variable: pPMMFreeListHead
	;; Hold first free list entry physical address
pPMMFreeListHead dd 0

szMsgPmmInit      db "PMM: initialize", 0
szMsgPmmFreeRange db "PMM: add free memory region %x - %x", 0
szErrorNoMemLeft  db "Error(PMM): no free memory left", 0
