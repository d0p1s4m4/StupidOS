	;; File: disk.inc

	;; Function: disk_read_sectors
	;;
	;; In:
	;;   AX    - LBA starting sector
	;;   CX    - Sector count
	;;   ES:BX - buffer 
disk_read_sectors:
	cmp byte [bDriveLBA], TRUE
	je .lba_read
	push ax
	push bx
	push cx
	xor dx, dx
	div word [sectors_per_track]
	inc dx
	mov [S], dx
	xor dx, dx
	div word [heads_per_cylinder]
	mov [C], ax
	mov [H], dx
	mov ah, 0x2
	mov al, 0x1
	mov ch, byte [C]
	mov cl, byte [S]
	mov dh, byte [H]
	mov dl, [uDrive]
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
	mov si, szMsgErrorSector
	call bios_print
	ret
.lba_read:
	push si
	mov word [disk_packet.sectors], cx
	mov word [disk_packet.segment], es
	mov word [disk_packet.offset], bx
	mov dword [disk_packet.lba], eax
	mov si, disk_packet
	mov dl, [uDrive]
	mov ah, 0x42
	int 0x13
	jc @b
	pop si
	ret

C dw 0x00
H dw 0x00
S dw 0x00

disk_packet:
	db 0x10
	db 0
.sectors:
	dw 0
.offset:
	dw 0
.segment:
	dw 0
.lba:
	dd 0
	dd 0


sectors_per_track  dw 18
heads_per_cylinder dw 2
bytes_per_sector   dw 512
sectors_per_FAT    dw 9
FAT_count          db 2
reserved_sectors   dw 1
root_dir_entries   dw 224
sectors_per_cluster db 1
