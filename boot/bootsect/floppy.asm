	;; File: floppy.asm
	format binary
	use16

	include '../common/const.inc'
	include 'sys/macro.inc'

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

	mov si, szLoaderFile
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
	mov si, szError
	call bios_print
	pop si
	call bios_print
	mov si, szNotFound
	call bios_print
	hlt
	jmp $

	; CHS to LBA
	; LBA = (C * HPC + H) * SPT + (S - 1)

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
	; https://en.wikipedia.org/wiki/Logical_block_addressing
	; convert LBA to CHS
	; HPC = Head per Cluster
	; SPT = Sector per Track

	; S = (LBA % SPT) + 1
	push ax
	push bx
	push cx
	xor dx, dx
	div word [sectors_per_track]
	inc dx
	mov [S], dx

	; H = (LBA / SPT) % HPC
	; C =  LBA  / (HPC * SPT)
	xor dx, dx
	div word [heads_per_cylinder]
	mov [C], ax
	mov [H], dx

	; read sectors
	mov ah, 0x2
	mov al, 0x1
	mov ch, byte [C]
	mov cl, byte [S]
	mov dh, byte [H]
	mov dl, [drive_number]

	int 0x13
	jc @f
	pop cx
	pop bx
	pop ax
	add bx, word [bytes_per_sector]
	inc ax
	loop disk_read_sectors
	ret
@@:
	mov si, szErrorSector
	call bios_print
	ret

C dw 0x00
H dw 0x00
S dw 0x00

	include '../common/bios.inc'
	include '../common/fat12.inc'

szError          db "ERROR: ", 0
szNotFound       db " not found", CR, LF, 0
szErrorSector    db "reading sector", CR, LF, 0

szLoaderFile     db "STPDLDR SYS", 0

stage1_start dw 0x0


	rb 0x7C00+512-2-$
	db 0x55, 0xAA
