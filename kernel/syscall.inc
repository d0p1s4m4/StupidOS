SYSCALL_EXIT  = 0x01
SYSCALL_FORK  = 0x02
SYSCALL_READ  = 0x03
SYSCALL_WRITE = 0x04

isr_syscall:
	push eax
	mov esi, szMsgSyscall
	call klog
	iret

szMsgSyscall db "syscall %x", 0
