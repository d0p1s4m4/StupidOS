[BITS 32]

%include "base.inc"

%macro ISR_NO_ERR 1
global isr%1
isr%1:
	xchg bx, bx
	push dword 0
	push dword %1
	jmp isr_handler
%endmacro

%macro ISR_ERR 1
global isr%1
isr%1:
	xchg bx, bx
	push dword %1
	jmp isr_handler
%endmacro

struc intframe
	;; registers
	.edi: resd 1
	.esi: resd 1
	.ebp: resd 1
	.esp: resd 1
	.ebx: resd 1
	.edx: resd 1
	.ecx: resd 1
	.eax: resd 1

	;;
	.gs: resd 1
	.fs: resd 1
	.es: resd 1
	.ds: resd 1
	.intno: resd 1

	;; by x86 hardware
	.err: resd 1
	.eip: resd 1
	.cs:  resd 1
	.eflags: resd 1

	;; crossring
	.useresp: resd 1
	.ss: resd 1
endstruc	
	
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

panic:
	LOG msg_interrupt
	;htl
	;jmp panic
	ret

;global isr_handler
isr_handler:
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

	;extern pic_eoi
	;call pic_eoi
	
	popa
	pop gs
	pop fs
	pop es
	pop ds
	add esp, 8					; int no & error code

	iret

section .rodata

msg_interrupt db "interrupt", 0xA
	db "edi: %x esi: %x ebp: %x esp: %x", 0xA
	db "ebx: %x edx: %x ecx: %x eax: %x", 0xA
	db "gs:  %x fs:  %x es:  %x ds:  %x", 0xA
	db "int: %x err: %x eip: %x cs:  %x", 0xA
	db "flg: %x usp: %x ss:  %x", 0x0
file db __FILE__, 0
