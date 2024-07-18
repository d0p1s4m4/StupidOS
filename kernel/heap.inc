	;; File: heap.inc

	;; Function: heap_init
heap_init:
	push ebp
	mov ebp, esp
	sub esp, 4
	mov [ebp-4], eax
	mov ecx, KERNEL_VIRT_BASE
	add eax, ecx
	push eax
	push ecx
	mov esi, szMsgHeapInit
	call klog

	; TODO

	leave
	ret

heap_alloc:
	ret

heap_free:
	ret

szMsgHeapInit db "HEAP: initialize %x - %x", 0
