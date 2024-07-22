SYSCALL_EXIT  = 0x01
SYSCALL_FORK  = 0x02
SYSCALL_READ  = 0x03
SYSCALL_WRITE = 0x04

syscall_null:
	ret

isr_syscall:
	push eax
	mov esi, szMsgSyscall
	call klog
	
	iret

szMsgSyscall db "syscall %x", 0

apSyscallTable:
	dd syscall_null ; SYSCALL_NULL
	dd 0 ; SYSCALL_EXIT
	dd 0 ; SYSCALL_FORK
	dd 0 ; SYSCALL_READ
	dd 0 ; SYSCALL_WRITE
