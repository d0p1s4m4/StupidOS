; File: kernel.s
[BITS 32]

%include "base.inc"

section .bss
align 16
stack_bottom:
	resb 16384
global stack_top
stack_top:

section .text
; Function: kmain
global kmain
kmain:
	push ebp
	mov ebp, esp

	extern serial_init
	call serial_init

	LOG msg_hello_world
	
	leave
	ret

section .rodata

msg_hello_world db "StupidOS v", STUPID_VERSION, " (built with ", __NASM_VER__, " on ",  __DATE__, " ", __TIME__, ")", 0

file db __FILE__, 0
