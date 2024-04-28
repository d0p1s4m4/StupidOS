	;; File: floppy.asm
	format binary
	use16

	include '../common/const.inc'
	include '../common/macro.inc'

	org BOOTSECT_BASE

	jmp short _start
	nop

if FLOPPY_SIZE eq FLOPPY_2880
	SECTORS_PER_CLUSTER = 2
	ROOT_DIR_ENTRIES    = 240
	TOTAL_SECTORS       = 5760
	SECTORS_PER_TRACK   = 36
else
	SECTORS_PER_CLUSTER = 1
	ROOT_DIR_ENTRIES    = 224
	TOTAL_SECTORS       = 2880
	SECTORS_PER_TRACK   = 18
end if

	; Boot Record
OEM_identifier      db 'STUPID  '
bytes_per_sector    dw 512
sectors_per_cluster db SECTORS_PER_CLUSTER
reserved_sectors    dw 1
FAT_count           db 2
root_dir_entries    dw ROOT_DIR_ENTRIES
total_sectors       dw TOTAL_SECTORS
media_desc_type     db 0xF0
sectors_per_FAT     dw 9
sectors_per_track   dw SECTORS_PER_TRACK
heads_per_cylinder  dw 2
hidden_sectors      dd 0
large_sector_count  dd 0
	; Extended Boot Record
drive_number db 0
reserved     db 0
signature    db 0x29   		; 0x28 or 0x29
volume_id    dd 0xB00B135 	; hope mine will grow :'(
volume_label db 'Stupid Boot'
system_id    db 'FAT12  '


_start:
	cli
	cld
	jmp 0x0:.canonicalize_cs

.canonicalize_cs:
	xor ax, ax
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov sp, 0x7c00

	mov [drive_number], dl

	; reset floppy disk
@@:
	mov dl, [drive_number]
	xor ah, ah
	int 0x13
	jc @b

	call fat_load_root

	; search in root directory

	mov si, stage1_file
	call fat_search_root
	jc .error_not_found
	mov [stage1_start], ax

	; load fat
	xor ax, ax
	mov al, [FAT_count]
	mul word [sectors_per_FAT]
	mov cx, ax
	mov ax, [reserved_sectors]

	xor bx, bx

	call disk_read_sectors

	; load stage 2
	mov ax, LOADER_BASE/0x10
	mov es, ax
	mov ax, [stage1_start]
	xor bx, bx
	call fat_load_binary

	mov dl, [drive_number]

	jmp 0x0:LOADER_BASE

.error_not_found:
	push si
	mov si, msg_error
	call bios_print
	pop si
	call bios_print
	mov si, msg_not_found
	call bios_print
	hlt
	jmp $

	include '../common/bios.inc'
	include '../common/fat12.inc'

msg_error     db "ERROR: ", 0
msg_not_found db " not found", CR, LF, 0

stage1_file db "STPDLDR SYS", 0

stage1_start dw 0x0


	rb 0x7C00+512-2-$
	db 0x55, 0xAA
