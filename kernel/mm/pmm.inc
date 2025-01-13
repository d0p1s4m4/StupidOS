	;; File: pmm.inc
	;;
	;; Our PMM is just a linked list of page-aligned size blocks. 
	;;
	;; > ┌─────┐      ┌─────┐       
	;; > │size │  ┌──►│size │       
	;; > │next ├──┘   │next ├────► 0
	;; > └─────┘      └─────┘       
	;;

;;; MARK: macros and structs

	;; Constant: PMM_VIRT_TEMP
PMM_VIRT_TEMP = 0xD0000000
PMM_BITMAP_USED = 0x0
PMM_BITMAP_FREE = 0x1

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

;;; MARK: private functions

	;; Function: _pmm_map_temp_page
	;;
	;; In:
	;;   EAX - physical address to map at <PMM_VIRT_TEMP>
_pmm_map_temp_page:
	mov ecx, MM_KERNEL_PTE_BASE + (PMM_VIRT_TEMP shr 12) * 4
	or eax, 0x3
	mov [ecx], eax

if CONFIG_COMPAT_I386
	mov eax, 0x40000 + (1023 * PAGE_SIZE)
	mov cr3, eax
else
	mov eax, PMM_VIRT_TEMP
	invlpg [eax]
end if
	ret

_pmm_unmap_temp_page:
	mov ecx, MM_KERNEL_PTE_BASE + (PMM_VIRT_TEMP shr 12) * 4
	mov dword [ecx], 0

if CONFIG_COMPAT_I386
	mov eax, 0x40000 + (1023 * PAGE_SIZE)
	mov cr3, eax
else
	mov eax, PMM_VIRT_TEMP
	invlpg [eax]
end if
	ret

	;; Function: _pmm_init_region
	;;
	;; Add new memory region to the linked list.
	;;
	;; In:
	;;    EAX - Start
	;;    EDX - End
	;;
_pmm_init_region:
	push ebp
	mov ebp, esp

	push edi
	push esi

	mov edi, eax
	mov esi, edx

	push edx
	push eax
	mov eax, szMsgPmmFreeRange
	call klog
	pop eax
	pop edx

	call _pmm_map_temp_page

	xor ecx, ecx
	mov [PMM_VIRT_TEMP], esi
	mov [PMM_VIRT_TEMP + PMMFreeRange.next], ecx
	mov ecx, [pPmmFreeListHead]
	or ecx, ecx
	jz @f
	mov [PMM_VIRT_TEMP + PMMFreeRange.next], ecx
@@:
	mov [pPmmFreeListHead], edi

	call _pmm_unmap_temp_page

	pop edi
	pop esi

	leave
	ret

_pmm_init_low_memory:
	push ebp
	mov ebp, esp

	mov eax, [stBootInfo.low_mem]
	push eax
	xor eax, eax
	push eax
	mov eax, szMsgPmmFreeRange
	call klog
	add esp, 8

	; clear bitmap
	push edi
	xor eax, eax
	mov ecx, 8
	mov edi, au32PmmLowMemBitmap
	rep stosd
	
	mov edi, au32PmmLowMemBitmap
	xor eax, eax
	xor ecx, ecx
@@:
	add eax, PAGE_SIZE
	cmp eax, [stBootInfo.low_mem]
	jg @f
	mov ebx, 1
	shl ebx, cl
	or [edi], ebx
	inc ecx
	cmp ecx, 32
	jb @b
	xor ecx, ecx
	add edi, 4
	jmp @b

@@:
	pop edi

	leave
	ret

;;; MARK: public functions

	;; Function: pmm_alloc
	;; 
	;; In:
	;;    EAX - size in page
	;;
	;; Out:
	;;    EAX - first page physical address, 0 on error
	;;
pmm_alloc:
	mov eax, [pPmmFreeListHead]
	or eax, eax
	jnz @f
	mov edx, ENOMEM
	ret	; no memory left :'(
@@:
	call _pmm_map_temp_page

	call _pmm_unmap_temp_page

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

	;; Function: pmm_init
	;; 
	;; Out:
	;;    EAX - return -1 on error
pmm_init:
	mov eax, szMsgPmmInit
	call klog

	call _pmm_init_low_memory

if CONFIG_TRACE_PMM
	mov byte [cga_color], CGA_COLOR_FG_BLUE
	push dword [au32PmmLowMemBitmap + 28]
	push dword [au32PmmLowMemBitmap + 24]
	push dword [au32PmmLowMemBitmap + 20]
	push dword [au32PmmLowMemBitmap + 16]
	push dword [au32PmmLowMemBitmap + 12]
	push dword [au32PmmLowMemBitmap + 8]
	push dword [au32PmmLowMemBitmap + 4]
	push dword [au32PmmLowMemBitmap]

	mov eax, szTracePmmBitmap

	TRACE

	add esp, 32

end if

	mov eax, kend - KERNEL_VIRT_BASE
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

;;; MARK: variables

	;; Variable: pPMMFreeListHead
	;; Hold first free list entry physical address
pPmmFreeListHead dd 0

au32PmmLowMemBitmap rd 8

szMsgPmmInit      db "PMM: initialize", 0
szMsgPmmFreeRange db "PMM: add free memory region %x - %x", 0
szErrorNoMemLeft  db "Error(PMM): no free memory left", 0
if CONFIG_TRACE_PMM
szTracePmmBitmap db "Trace(PMM): low memory bitmap", CR, LF, \
	"    %x", CR, LF, "    %x", CR, LF, "    %x", CR, LF, "    %x", CR, LF, \
	"    %x", CR, LF, "    %x", CR, LF, "    %x", CR, LF, "    %x", 0
end if
