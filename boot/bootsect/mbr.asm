	format binary
	use16

	include '../common/const.inc'
	include 'sys/macro.inc'
	include '../common/bios.inc'
	include '../common/mbr.inc'

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
	sti
	mov [uBootDrive], dl

	; set LBA
	mov ah, 0x41
	mov bx, 0x55AA
	mov dl, 0x80
	int 0x13
	jc .error_lba
	lea ecx, [PT1]
.loop:
	mov al, byte [ecx]
	bt ax, 7
	jc .found
	lea eax, [PT4]
	test eax, ecx
	je .error_no_bootable
	add cx, 0xF
	jmp .loop
.found:
	mov eax, dword [ecx + Partition.lba]
	mov [disk_packet_lba], eax
	mov si, disk_packet
	mov ah, 0x42
	mov dl, 0x80
	int 13
	jc .error_load
	jmp 0x0:BOOTSECT_BASE
.error_lba:
	mov si, msg_error_lba
	jmp .error_print
.error_no_bootable:
	mov si, msg_error_bootable
	jmp .error_print
.error_load:
	mov si, msg_error_load
.error_print:
	call bios_print
.end:
	hlt
	jmp $

uBootDrive db 0

disk_packet:
	db 0x10
	db 0
	dw 1
	dw BOOTSECT_BASE
	dw 0x0
disk_packet_lba:
	dd 0x0
	dw 0x0

msg_error_lba db "We don't support CHS", CR, LF, 0
msg_error_bootable db "No bootable device found", CR, LF, 0
msg_error_load db "Can't load partition", CR, LF, 0

	rb MBR_BASE+0x1a8-$
UID db 'STUPIDDISK'
	; partition table
PT1 db 16 dup(0)
PT2 db 16 dup(0)
PT3 db 16 dup(0)
PT4 db 16 dup(0)
	; magic
	db 0x55, 0xAA
