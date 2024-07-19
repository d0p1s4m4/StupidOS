	;; File: heap.inc
	;; Base on <https://github.com/CCareaga/heap_allocator/blob/master/commented_heap.c>

struc HeapNode {
	.hole dd ?
	.size dd ?
	.next dd ?
	.prev dd ?
}
DEFN HeapNode

struc HeapInfo {
	.start dd ?
	.end   dd ?
	.bins  dd 9 dup(?)
}
DEFN HeapInfo

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

	mov eax, [ebp-4]
	mov ecx, KERNEL_VIRT_BASE
	mov [ecx + HeapNode.hole], dword 1
	sub eax, sizeof.HeapNode + 4
	mov [ecx + HeapNode.size], eax

	add eax, sizeof.HeapNode
	add eax, ecx
	mov [eax], ecx

	mov ecx, KERNEL_VIRT_BASE
	mov [kheap + HeapInfo.start], ecx
	mov eax, [ebp-4]
	add ecx, eax
	mov [kheap + HeapInfo.end], ecx

.end:
	leave
	ret

heap_alloc:
	ret

heap_free:
	ret

	;; Function: heap_getbin
	;;
	;; In:
	;;   EAX - size
heap_getbin:
	push ebp
	mov ebp, esp
	sub esp, 4

	mov [ebp-4], dword 0
	cmp eax, 4
	jge @f
	mov eax, 4
@@:
	shr eax, 1
	cmp eax, 1
	or eax, eax
	jz @f
	inc dword [ebp-4]
	jmp @b
@@:
	mov eax, [ebp-4]
	sub eax, 2
	cmp eax, 8
	jbe @f
	mov eax, 8
@@:
	leave
	ret

kheap HeapInfo
szMsgHeapInit db "HEAP: initialize %x - %x", 0
szMsgErrorHeap db "ERROR: HEAP: not enough memory", 0
