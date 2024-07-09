	;; File: pmm.inc

macro ALIGN reg {
	local end

	push reg
	and reg, 0xFFF
	pop reg
	jz end
	and reg, 0xFFFFF000
	add reg, 0x1000
end:

}

	;; Function: pmm_init
	;; 
	;; In:
	;;    EAX - Start
	;;    EBX - End
pmm_init:
	push eax
	mov esi, szMsgPmmInit
	call klog
	pop eax

	call pmm_free_range
	ret

	;; Function: pmm_alloc_page
	;; 
	;; Out:
	;;    EAX - page address (return zero on error)
pmm_alloc_page:
	cmp [pFreeList], 0
	je .error
	mov eax, [pFreeList]
	mov edx, [eax]
	mov [pFreeList], edx
	ret
.error:
	mov esi, szErrorNoMemLeft
	call klog
	xor eax, eax
	ret

	;; Function: pmm_free_page
	;; push page back to free list
	;;
	;; In:
	;;    EAX - page to be freed
pmm_free_page:
	or eax, eax
	jz @f
	mov edx, [pFreeList]
	mov [eax], edx
	mov [pFreeList], eax
@@:
	ret

	;; Function: pmm_free_range
	;; TODO: allignment 
	;;
	;; In:
	;;    EAX - Start
	;;    EBX - End
pmm_free_range:
	push ebp
	mov ebp, esp
	sub esp, 0x10
	ALIGN eax
	ALIGN ebx
	mov [ebp-4], eax
	mov [ebp-8], ebx

	push ebx
	push eax
	mov esi, szMsgPmmFreeRange
	call klog

	mov esi, [ebp-4]
.loop:
	mov eax, [pFreeList]
	mov [esi], eax
	mov [pFreeList], esi

	add esi, 4096
	cmp esi, [ebp-8]
	jb .loop


	leave
	ret

pFreeList dd 0
szMsgPmmInit      db "PMM: initialize", 0
szMsgPmmFreeRange db "PMM: add free memory range %x - %x", 0
szErrorNoMemLeft  db "Error: no free memory left", 0
