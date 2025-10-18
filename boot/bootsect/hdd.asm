	;; File: hdd.asm
	format binary
	use16

	include '../common/const.inc'
	include 'sys/macro.inc'

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

	call fat_load_root

	; search 
	mov si, stage1_file
	call fat_search_root
	jc .error_not_found
	mov [stage1_start], ax

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
	mov si, msg_error_not_found
	jmp .error
.error_lba:
	mov si, msg_error_13ext
.error:
	push si
	mov si, msg_error
	call bios_print
	pop si
	call bios_print
	hlt
	jmp $

	;; Function: disk_read_sectors
	;; Read sectors from disk to buffer
	;;
	;; Parameters:
	;;
	;;     AX    - LBA starting sector
	;;     CX    - sector count
	;;     ES:BX - buffer
	;;
disk_read_sectors:
	mov word [disk_packet.sectors], cx
	mov word [disk_packet.segment], es
	mov word [disk_packet.offset], bx
	mov word [disk_packet.lba], ax
	mov ds, [disk_packet]
	mov dl, [drive_number]
	mov ah, 0x42
	int 0x13
	ret

include '../common/bios.inc'
include '../common/fat12.inc'

msg_error              db "ERROR: ", 0
msg_error_13ext        db "We don't support CHS", CR, LF, 0
msg_error_not_found    db "loader not found", CR, LF, 0

stage1_file db "STPDLDR SYS", 0
stage1_start dw 0x0

disk_packet:
	db 0x10
	db 0
.sectors:
	dw 0
.segment:
	dw 0
.offset:
	dw 0
.lba:
	dd 0
	dd 0

	rb 0x7C00+512-2-$
	db 0x55, 0xAA
