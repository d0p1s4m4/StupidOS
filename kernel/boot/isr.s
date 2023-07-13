[BITS 32]

%include "sys/i386/cpu.inc"
%include "base.inc"

%macro ISR_NO_ERR 1
isr%1:
	push dword 0
	push dword %1
	jmp isr_common
%endmacro

%macro ISR_ERR 1
isr%1:
	push dword %1
	jmp isr_common
%endmacro	

%macro ISR_ADDR 1
	dd isr%1
%endmacro
	
section .text

ISR_NO_ERR 0
ISR_NO_ERR 1
ISR_NO_ERR 2
ISR_NO_ERR 3
ISR_NO_ERR 4
ISR_NO_ERR 5
ISR_NO_ERR 6
ISR_NO_ERR 7
ISR_ERR 8
ISR_NO_ERR 9
ISR_ERR 10
ISR_ERR 11
ISR_ERR 12
ISR_ERR 13
ISR_ERR 14
ISR_NO_ERR 15
ISR_NO_ERR 16
ISR_NO_ERR 17
ISR_NO_ERR 18
ISR_NO_ERR 19
ISR_NO_ERR 20
ISR_NO_ERR 21
ISR_NO_ERR 22
ISR_NO_ERR 23
ISR_NO_ERR 24
ISR_NO_ERR 25
ISR_NO_ERR 26
ISR_NO_ERR 27
ISR_NO_ERR 28
ISR_NO_ERR 29
ISR_NO_ERR 30
ISR_NO_ERR 31

%assign i 32
%rep 224
ISR_NO_ERR i
%assign i i+1
%endrep

panic:
	LOG msg_interrupt
	;htl
	;jmp panic
	ret


isr_handler:
	push ebp
	mov ebp, esp

.end:
	leave
	ret

isr_common:
	xchg bx, bx
	push ds
	push es
	push fs
	push gs
	pusha

	mov ax, 0x10				; data segment
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	LOG msg_interrupt
	;call isr_handler
	;extern pic_eoi
	;call pic_eoi
	
	popa
	pop gs
	pop fs
	pop es
	pop ds
	add esp, 8					; int no & error code

	iret

section .data
global isr_list
isr_list:
%assign i 0
%rep 256
ISR_ADDR i
%assign i i+1
%endrep

section .rodata

msg_interrupt db "interrupt", 0xA
	db "edi: %x esi: %x ebp: %x esp: %x", 0xA
	db "ebx: %x edx: %x ecx: %x eax: %x", 0xA
	db "gs:  %x fs:  %x es:  %x ds:  %x", 0xA
	db "int: %x err: %x eip: %x cs:  %x", 0xA
	db "flg: %x usp: %x ss:  %x", 0x0
file db __FILE__, 0
