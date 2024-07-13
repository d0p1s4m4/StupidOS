SYSCALL_EXIT = 0x01
SYSCALL_FORK = 0x02

isr_syscall:
	push eax
	mov esi, szMsgSyscall
	call klog
	iret

szMsgSyscall db "syscall %x", 0
