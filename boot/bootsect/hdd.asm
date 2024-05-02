	;; File: floppy.asm
	format binary
	use16

	include '../common/const.inc'
	include '../common/macro.inc'

jmp short _start

OEM_identifier      db 'STUPID  '
bytes_per_sector    dw 512
sectors_per_cluster db 0
reserved_sectors    dw 0
FAT_count           db 0
root_dir_entries    dw 0
total_sectors       dw 0
media_desc_type     db 0
sectors_per_FAT     dw 0
sectors_per_track   dw 18
heads_per_cylinder  dw 2
hidden_sectors      dd 0
large_sector_count  dd 0
	; Extended Boot Record
drive_number db 0
reserved     db 0
signature    db 0
volume_id    dd 0xB00B135 	; hope mine will grow :'(
volume_label db 'Stupid Boot'
system_id    db '       '


_start:
	mov [drive_number], dl

	; ensure int 13h extension
	mov ah, 0x41
	mov bx, 0x55AA
	int 0x13
	jc .error_lba

.error_lba:
	mov si, msg_error_13ext
	jmp .error_print


msg_error_13ext db "We don't support CHS", CR, LF, 0

	rb 0x7C00+512-2-$
	db 0x55, 0xAA
