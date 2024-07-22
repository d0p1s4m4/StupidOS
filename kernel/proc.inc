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

uNextpid dd 1
uProcLock dd 0
pCurrentProc dd 0
szErrorNoFreeProcess db "Error: can't alloc new process", 0
