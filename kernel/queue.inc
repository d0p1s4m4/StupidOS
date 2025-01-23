	;; File: queue.inc

;;; MARK: doubly linked lists

	;; Group: List
	;; Doubly linked lists

	;; Struc: ListHead
struc ListHead initial {
	.first dd initial
}

	;; Struc: ListEntry
struc ListEntry {
	.next dd ?
	.prev dd ?
}

	;; Function: list_insert_head
	;;  
	;; In:
	;;    EAX - Head (see <ListHead>)
	;;    EDX - Element (see <ListEntry>)
	;;
list_insert_head:
	mov ecx, [eax]
	mov [edx], ecx
	or ecx, ecx
	jz @f
	mov [ecx + 4], edx
@@:
	mov [eax], edx
	mov [edx + 4], edx
	ret

	;; Function: list_remove
	;;
	;; In:
	;;    EAX - Element.
list_remove:
	mov ecx, [eax + 4]
	mov edx, [eax]
	or edx, edx
	jz @f
	mov [edx + 4], ecx
@@:
	mov [ecx], edx
	ret

	;; Function: list_map
	;;
	;; In:
	;;    EAX - Pointer to <ListHead>
	;;    EDX - Function to apply
	;;
list_map:
	push ebp
	mov ebp, esp

	push esi
	push edi

	; save function pointer
	mov esi, edx

	; load first elem
	mov edi, [eax]
@@:
	or edi, edi
	jz @f ; exit loop if element is null

	mov eax, edi
	call esi

	mov edi, [edi] ; load next element
	jmp @b
@@:
	pop edi
	pop esi

	leave
	ret

;;; MARK: doubly linked tail queues

	;; Group: TailQ
	;; doubly linked tail queues

	;; Struc: TailQHead
struc TailQHead {
	.first dd ?
	.last dd $$
}

	;; Struc: TailQEntry
struc TailQEntry {
	.next dd ?
	.prev dd ?
}

	;; Function: tailq_insert_head
	;;
	;; In:
	;;    EAX - Head.
	;;    EDX - Element.
tailq_insert_head:

	ret

	;; Function: tailq_insert_after
	;;
tailq_insert_after:
	ret

	;; Function: tailq_insert_tail
	;; 
	;; In:
	;;    EAX - X
	;;    EDX - X
tailq_insert_tail:
	ret

	;; Function: tailq_remove
tailq_remove:
	ret

	;; Function: tailq_map
tailq_map:
	ret
