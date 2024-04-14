	format binary
	use16

	include '../common/const.inc'
	include '../common/macro.inc'

	org MBR_BASE
	cli
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov sp, ax
	cld
	; relocate from 0x7C00 to 0x0600
	mov cx, 0x0100
	mov si, BOOTSECT_BASE
	mov di, MBR_BASE
	rep movsw
	jmp 0x0:start
start:
	; TODO: read partition table and load bootable one.

	times 436-($-$$) db 0x90
UID db 'STUPIDDISK'
	; partition table
PT1 db 16 dup(0)
PT2 db 16 dup(0)
PT3 db 16 dup(0)
PT4 db 16 dup(0)
	; magic
	db 0x55, 0xAA
