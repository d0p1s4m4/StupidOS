	;; File: pmm.old.inc

macro ALIGN reg {
	local ..end

	push reg
	and reg, 0xFFF
	pop reg
	jz ..end
	and reg, 0xFFFFF000
	add reg, 0x1000
..end:
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
	mov eax, uPmmLock
	call lock_acquire

	cmp [pFreeList], 0
	je .error
	mov eax, [pFreeList]
	mov edx, [eax]
	mov [pFreeList], edx

	inc [cUsedPage]
	dec [cFreePage]


	push eax
	mov eax, uPmmLock
	call lock_release
	pop eax

	ret
.error:
	mov esi, szErrorNoMemLeft
	call klog

	mov eax, uPmmLock
	call lock_release
	xor eax, eax

	ret

	;; Function: pmm_free_page
	;; push page back to free list
	;;
	;; In:
	;;    EAX - page to be freed
pmm_free_page:
	mov eax, uPmmLock
	call lock_acquire
	or eax, eax
	jz @f
	mov edx, [pFreeList]
	mov [eax], edx
	mov [pFreeList], eax

	inc [cFreePage]
	dec [cUsedPage]
@@:
	mov eax, uPmmLock
	call lock_release
	ret

	;; Function: pmm_free_range
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

	inc [cTotalPage]
	inc [cFreePage]

	add esi, 4096
	cmp esi, [ebp-8]
	jb .loop

	call pmm_stats

	leave
	ret

	;; Function: pmm_stats
pmm_stats:
	push dword [cFreePage]
	push dword [cUsedPage]
	push dword [cTotalPage]
	mov esi, szMsgPmmStats
	call klog
	ret

pFreeList dd 0
szMsgPmmInit      db "PMM: initialize", 0
szMsgPmmFreeRange db "PMM: add free memory range %x - %x", 0
szErrorNoMemLeft  db "Error: no free memory left", 0
szMsgPmmStats     db "PMM: Total page: %x | Used page: %x | Free page: %x", 0
uPmmLock          dd 0

; Some stats
cFreePage  dd 0
cUsedPage  dd 0
cTotalPage dd 0
