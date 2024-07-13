	;; File: vga_console.inc

VGA_COLUMNS = 80
VGA_LINES = 25
VGA_BASE = 0xC00B8000

vga_lineno db 0
vga_colno db 0


	;; Function: vga_console_clear
vga_console_clear:
	mov ecx, VGA_COLUMNS*VGA_LINES
	xor al, al
	mov edi, VGA_BASE
	rep stosb
	ret

vga_console_putc:
	
	ret

