[BITS 32]
section .text

global setup_pic
setup_pic:
	mov al, 0x11
	out 0x20, al
	out 0xA0, al
	
	mov al, 0x20
	out 0x21, al
	mov al, 0x28
	out 0xA1, al

	mov al, 4
	out 0x21, al
	mov al, 2
	out 0xA1, al

	; mask all
	mov al, 0xFF
	out 0x21, al
	out 0xA1, al
	ret
