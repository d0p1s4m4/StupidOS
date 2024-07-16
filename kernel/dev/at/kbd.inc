	;; File: kbd.inc
	;;
	;; Usefull links:
	;; - <https://wiki.osdev.org/PS/2_Keyboard>

KBD_STATP = 0x64
KBD_ATAP  = 0x60 
KBS_DIB   = 0x01

kbd_getc:
	in al, KBD_STATP
	and al, KBS_DIB
	jz @f
	in al, KBD_ATAP
	mov ah, al
	and ah, 0x80
	jnz @f
	mov edx, aKbdNormalMap
	movzx eax, al
	add edx, eax
	mov byte al, [edx]
	call cga_putc 
@@:
	ret

kbd_irq:
	pusha

	mov al, PIC_EOI
	out PIC1_COMMAND, al

	call kbd_getc

	popa
	iret

szMsgKbdIRQ db "KBD: IRQ", 0

aKbdNormalMap:
	db    0, 0x1B,  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  '0'
	db  '-',  '=', 0x08, 0x09,  'q',  'w',  'e',  'r',  't',  'y',  'u',  'i'
	db  'o',  'p',  '[',  ']',   LF,    0,  'a',  's',  'd',  'f',  'g',  'h'
	db  'j',  'k',  'l',  ';',  "'",  '`',    0,  '\',  'z',  'x',  'c',  'v'
	db  'b',  'n',  'm',  ',',  '.',  '/',    0,  '*',    0,  ' ',    0,    0
	db    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,   '7'
	db   '8',  '9',  '-',  '4',  '5',  '6',  '+',  '1',  '2',  '3',  '0',  '.'
	db     0,    0,    0,    0,    0,    0,    0,    0,    0, 0x1B,  '1',  '2'
	db   '3',  '4',  '5',  '6',  '7',  '8',  '9',  '0',  '-',  '=', 0x08, 0x09
	db   'q',  'w',  'e',  'r',  't',  'y',  'u',  'i',  'o',  'p',  '[',  ']'
	db    LF,    0,  'a',  's',  'd',  'f',  'g',  'h',  'j',  'k',  'l',  ';'
	db   "'",  '`',    0,  '\',  'z',  'x',  'c',  'v',  'b',  'n',  'm',  ','
	db   '.',  '/',    0,  '*',    0,  ' ',    0,    0,    0,    0,    0,    0
	db     0,    0,    0,    0,    0,    0,    0,   '7', '8',  '9',  '-',  '4'
	db   '5',  '6',  '+',  '1',  '2',  '3',  '0',  '.'
