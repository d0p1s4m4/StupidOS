[BITS 32]

global syscall_table
syscall_table:
	dd 0						; sys_exit
	dd 0						; sys_fork
	dd 0						; sys_read
	dd 0						; sys_write
	dd 0						; sys_open
	dd 0						; sys_close
.end:
