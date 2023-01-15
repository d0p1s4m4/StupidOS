section .text

COM1 equ 0x3F8
COM2 equ 0x2F8
COM3 equ 0x3E8
COM4 equ 0x2E8

THR equ 0x0
RBR equ 0x0
IER equ 0x1
IIR equ 0x2
LCR equ 0x3
MCR equ 0x4
LSR equ 0x5
MSR equ 0x7
DLL equ 0x0
DLH equ 0x0

%macro COM_OUT 3
	mov dx, %1+%2
	mov al, %3
	out dx, al
%endmacro

%macro COM_IN 2
	mov dx, %1+%2
	in al, dx
%endmacro

global serial_init
serial_init:
	COM_OUT COM1, IER, 0x00
	COM_OUT COM1, LCR, 0x80
	COM_OUT COM1, DLH, 0x00
	COM_OUT COM1, DLL, 0x0C

	COM_OUT COM1, LCR, 0x03

	ret

global serial_write
serial_write:
	push ebp
	mov ebp, esp
	mov ecx, [ebp + 8]
	COM_OUT COM1, THR, cl 
	pop ebp
	ret
