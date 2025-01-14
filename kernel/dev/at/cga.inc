	;; File: cga.inc
	;;
	;; Usefull links:
	;; - <http://www.techhelpmanual.com/901-color_graphics_adapter_i_o_ports.html>
	;; - <http://www.osdever.net/FreeVGA/vga/crtcreg.htm>
CGA_MEMORY      = 0xC00B8000
CGA_COMMAND     = 0x3d4
CGA_DATA        = 0x3d5
CGA_CURSOR_HIGH = 0xE
CGA_CURSOR_LOW  = 0xF
CGA_COLUMNS = 80
CGA_LINES = 24

CGA_COLOR_FG_BLACK         = 0x00
CGA_COLOR_FG_BLUE          = 0x01
CGA_COLOR_FG_GREEN         = 0x02
CGA_COLOR_FG_CYAN          = 0x03
CGA_COLOR_FG_RED           = 0x04
CGA_COLOR_FG_MAGENTA       = 0x05
CGA_COLOR_FG_BROWN         = 0x06
CGA_COLOR_FG_LIGHT_GRAY    = 0x07
CGA_COLOR_FG_DARK_GRAY     = 0x08
CGA_COLOR_FG_LIGHT_BLUE    = 0x09
CGA_COLOR_FG_LIGHT_GREEN   = 0x0A
CGA_COLOR_FG_LIGHT_CYAN    = 0x0B
CGA_COLOR_FG_LIGHT_RED     = 0x0C
CGA_COLOR_FG_LIGHT_MAGENTA = 0x0D
CGA_COLOR_FG_YELLOW        = 0x0E
CGA_COLOR_FG_WHITE         = 0x0F

CGA_COLOR_BG_BLACK         = 0x00
CGA_COLOR_BG_BLUE          = 0x10
CGA_COLOR_BG_GREEN         = 0x20
CGA_COLOR_BG_CYAN          = 0x30
CGA_COLOR_BG_RED           = 0x40
CGA_COLOR_BG_MAGENTA       = 0x50
CGA_COLOR_BG_BROWN         = 0x60
CGA_COLOR_BG_LIGHT_GRAY    = 0x70
CGA_COLOR_BG_DARK_GRAY     = 0x80
CGA_COLOR_BG_LIGHT_BLUE    = 0x90
CGA_COLOR_BG_LIGHT_GREEN   = 0xA0
CGA_COLOR_BG_LIGHT_CYAN    = 0xB0
CGA_COLOR_BG_LIGHT_RED     = 0xC0
CGA_COLOR_BG_LIGHT_MAGENTA = 0xD0
CGA_COLOR_BG_YELLOW        = 0xE0
CGA_COLOR_BG_WHITE         = 0xF0

macro CGA_SET_COLOR col {
	mov byte [bCGAColor], col
}

	;; Function: cga_getpos
	;; 
	;; Out:
	;;    AX - cursor position
cga_getpos:
	; read cursor position
	xor ax, ax
	
	; get high
	mov dx, CGA_COMMAND
	mov al, CGA_CURSOR_HIGH
	out dx, al
	mov dx, CGA_DATA
	in al, dx
	xchg al, ah

	; get low
	mov dx, CGA_COMMAND
	mov al, CGA_CURSOR_LOW
	out dx, al
	mov dx, CGA_DATA
	in al, dx

	ret

	;; Function: cga_setpos
	;; 
	;; In:
	;;    AX - cursor position
cga_setpos:
	mov dx, CGA_COMMAND
	push ax
	mov al, CGA_CURSOR_HIGH
	out dx, al
	pop ax
	xchg al, ah
	mov dx, CGA_DATA
	out dx, al

	mov dx, CGA_COMMAND
	mov al, CGA_CURSOR_LOW
	out dx, al
	xchg al, ah
	mov dx, CGA_DATA
	out dx, al

	ret

	;; Function: cga_putc
	;; 
	;; In:
	;;    AL - charactere to print
cga_putc:
	push ebp
	mov ebp, esp
	sub esp, 0x2

	push eax
	call cga_getpos
	mov [ebp-0x2], ax
	pop eax
	cmp al, CR
	jne @f
	leave
	ret
@@:
	cmp al, LF
	jne @f
	mov ax, [ebp-0x2]
	mov dl, CGA_COLUMNS
	div dl
	mov al, CGA_COLUMNS
	sub al, ah
	xor ah, ah
	mov cx, [ebp-0x2]
	add cx, ax
	jmp .next
@@:
	mov edx, CGA_MEMORY
	xor ecx, ecx
	mov cx, [ebp-0x2]
	shl cx, 1
	add edx, ecx
	mov ah, byte [bCGAColor]
	mov word [edx], ax
	mov cx, [ebp-0x2]
	inc cx

.next:
	mov ax, cx
	mov dl, CGA_COLUMNS
	div dl
	cmp al, CGA_LINES
	jbe .end

	mov [ebp-0x2], cx

	; scroll up
	push esi
	push edi
	mov ecx, CGA_COLUMNS*(CGA_LINES)
	mov edi, CGA_MEMORY
	mov esi, CGA_MEMORY+80*2
	rep movsw

	mov ecx, CGA_COLUMNS
	xor ax, ax
	mov edi, CGA_MEMORY+(CGA_COLUMNS*(CGA_LINES)*2)
	rep stosw

	pop esi
	pop edi

	mov cx, CGA_COLUMNS*(CGA_LINES)
.end:
	mov ax, cx
	call cga_setpos

	leave
	ret

cga_device:
	db 'video', 0, 0, 0

bCGAColor db CGA_COLOR_FG_LIGHT_GRAY
