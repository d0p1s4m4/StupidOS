proc_init:
	mov ecx, sizeof.Process * 64
	xor ax, ax
	mov edi, aProcs
	rep stosb

	ret

proc_alloc:
	mov eax, uProcLock
	call lock_acquire

	xor ecx, ecx
@@:
	mov eax, [ecx + Process.state]
	or eax, eax
	jz @f

	add ecx, sizeof.Process
	cmp ecx, sizeof.Process * 64
	jb @b
	mov esi, szErrorNoFreeProcess
	call klog
	mov eax, -1
	jmp .end
@@:

.end:
	push eax
	mov eax, uProcLock
	call lock_release
	pop eax
	ret

context_switch:
	mov ax, 0x23
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov eax, esp
	push 0x23
	push eax
	pushf
	push 0x1b
	; push addr
	iret

uNextpid dd 1
uProcLock dd 0
pCurrentProc dd 0
szErrorNoFreeProcess db "Error: can't alloc new process", 0
