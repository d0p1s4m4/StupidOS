	format COFF
	use32

	section '.text' code

	;; Function: rc4_init
	;;
	;; Parameters:
	;; 
	;;     [esp+8]  - state
	;;     [esp+12] - key
	;;     [esp+16] - key length
	;; 
rc4_init:
	push ebp
	mov ebp, esp

	mov eax, [ebp+8]
	xor ecx, ecx

.loop:
	
.end:
	leave
	ret

	;; Function: rc4
	;;
	;; Parameters:
	;;
	;;     [esp+8]  - state
	;;     [esp+12] - out buffer
	;;     [esp+16] - input buffer
	;;     [esp+20] - intput buffer size
rc4:
	push ebp
	mov ebp, esp

	push esi
	push edi
	push ebx

	xor esi, esi
	xor edi, edi
	xor ecx, ecx

.loop:
	cmp ecx, [esp+20]
	je .end

	inc esi
	mov eax, esi
	movzx esi, al

	add edi, esi
	mov eax, edi
	movzx edi, al

	mov edx, [esp+8]

	mov al, byte [edx+esi]
	mov bl, byte [edx+edi]

	mov byte [edx+esi], bl
	mov byte [edx+edi], al

	inc ecx
	jmp .loop
.end:

	pop ebx
	pop edi
	pop esi

	leave
	ret
