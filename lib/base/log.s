[BITS 32]

%ifdef __KERNEL__
extern serial_write
%endif

section .text

putstr:
	push ebp
	mov ebp, esp
	push edi
	mov edi, [ebp + 8]

.loop:
	mov eax, [edi]
	cmp al, 0
	je .end
	push eax
%ifdef __KERNEL__
	call serial_write
%else
%endif
	add esp, 4
	inc edi
	jmp .loop
.end:
	pop edi
	leave
	ret

puthex:
	xchg bx, bx
	push ebp
	mov ebp, esp
	push edi
	push esi
	mov edi, [ebp + 8] ; number
	
	mov eax, hexprefix
	push hexprefix
	call putstr
	add esp, 4

	mov edx, 0xF
	mov ecx, 0x8
	mov esi, buffer
.loop:
	rol edi, 4
	mov eax, edi
	and eax, edx
	mov al, [digits + eax]
	mov [esi], al
	inc esi
	dec ecx
	jnz .loop

	mov eax, buffer
	push eax
	call putstr
	add esp, 4

	pop esi
	pop edi
	leave
	ret

global log_impl
log_impl:
	push ebp
	mov ebp, esp
	push edi
	push esi
	push ebx
	mov eax, [ebp + 8]

	push eax
	call putstr
	add esp, 4

%ifdef __KERNEL__
	mov eax, ':'
	push eax
	call serial_write
	add esp, 4
%else
%endif

	mov edi, [ebp + 12]
	mov esi, 12
.loop:
	mov eax, [edi]
	cmp al, 0
	je .end
	cmp al, '%'
	jne .putchar
	inc edi
	mov eax, [edi]
	cmp al, '%'
	je .next
	cmp al, 'x'
	jne .check_s
	add esi, 4
	mov ebx, ebp
	add ebx, esi
	mov eax, [ebx]
	push eax
	call puthex
	add esp, 4
	jmp .next
.check_s:
	cmp al, 's'
	jne .unknown_format
	add esi, 4
	mov ebx, ebp
	add ebx, esi
	mov eax, [ebx]
	push eax
	call putstr
	add esp, 4
	jmp .next
.unknown_format:
	mov al, '?'
.putchar:
	push eax
%ifdef __KERNEL__
	call serial_write
%else
	;; TODO
%endif
	add esp, 4
.next:
	inc edi
	jmp .loop
.end:

	;;  print new line
%ifdef __KERNEL__
	mov al, 0xA
	push eax
	call serial_write
	add esp, 4
%else
%endif
	pop ebx
	pop esi
	pop edi
	leave
	ret

digits db '0123456789ABCDEF'
hexprefix db '0x', 0
buffer db '00000000', 0
