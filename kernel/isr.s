[BITS 32]

%include "base.inc"

%macro ISR_NO_ERR 1
global isr%1
isr%1:
	cli
	push byte 0
	push byte %1
	jmp isr_handler
%endmacro

%macro ISR_ERR 1
global isr%1
isr%1:
	cli
	push byte %1
	jmp isr_handler
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

global isr_handler
isr_handler:
	pusha

	mov ax, ds
	push eax
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	LOG msg_interrupt

	extern pic_eoi
	call pic_eoi

	pop eax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	popa
	add esp, 8

	sti
	iret

section .rodata

msg_interrupt db "interrupt", 0
file db __FILE__, 0
