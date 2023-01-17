[BITS 32]

%ifdef __KERNEL__
extern serial_write
%endif

section .text

putstr:
	push ebp
	mov ebp, esp
	mov ebx, [ebp + 8]
.loop:
	mov eax, [ebx]
	cmp al, 0
	je .end
	push eax
%ifdef __KERNEL__
	call serial_write
%else
%endif
	add esp, 4
	inc ebx
	jmp .loop
.end:
	pop ebp
	ret

putuint:
	push ebp
	mov ebp, esp

	mov ebx, [ebp + 8]
	mov edx, [ebp + 12]

	mov eax, buffer
	push eax
	call putstr

	pop ebp
	ret

global log_impl
log_impl:
	push ebp
	mov ebp, esp
	mov eax, [ebp + 8]

	push eax
	call putstr
	add esp, 4

%ifdef __KERNEL__
	push ':'
	call serial_write
	add esp, 4
%else
%endif

.loop:
	mov eax, [ebp + 12]
	push eax
	call putstr
	add esp, 4
.end:

%ifdef __KERNEL__
	mov al, 0xA
	push eax
	call serial_write
	add esp, 4
%else
%endif
	pop ebp
	ret

digits db '0123456789ABCDEF'
buffer db '0000000000', 0
