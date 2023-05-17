[BITS 32]

PIC1_CMD	equ 0x20
PIC1_DATA	equ 0x21
PIC2_CMD	equ 0xA0
PIC2_DATA	equ 0xA1

section .text

	; Function: setup_pic
	; 
	; in:
	;     none
	;
	; out:
	;     none
	;
global setup_pic
setup_pic:
	mov al, 0x11
	out PIC1_CMD, al
	out PIC2_CMD, al
	
	mov al, 0x20
	out PIC1_DATA, al
	mov al, 0x28
	out PIC2_DATA, al

	mov al, 4
	out PIC1_DATA, al
	mov al, 2
	out PIC2_DATA, al

	; mask all
	mov al, 0xFF
	out PIC1_DATA, al
	out PIC2_DATA, al
	ret

	; Function: pic_eoi
	; 
	; in:
	;     none
	;
	; out:
	;     none
	;
global pic_eoi
pic_eoi:
	mov al, 0x20
	out PIC2_CMD, al
	out PIC1_CMD, al
	ret
