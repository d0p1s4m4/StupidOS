proc_init:
	mov ecx, sizeof.Process * 64
	xor ax, ax
	mov edi, aProcs
	rep stosb

	ret


uProcLock dd 0
pCurrentProc dd 0
