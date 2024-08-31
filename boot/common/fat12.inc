	;; File: fat12.inc

struc fat_bpb
{
	.jump             db 3 dup(?)
	.oem_name         db 8 dup(?)
	.bytes_per_sects  dw ?
	.sects_per_clust  db ?
	.reserved_sects   dw ?
	.FAT_count        db ?
	.root_dir_entries dw ?
	.total_sects      dw ?
	.media_type       db ?
	.sects_per_FAT    dw ?
	.sects_per_track  dw ?
	.heads_per_cyl    dw ?
}
DEFN fat_bpb

	;; Struct: fat_entry
struc fat_entry
{
	.name          db 8 dup ?
	.ext           db 3 dup ?
	.attrs         db ?
	.reserved      dw ?
	.creation_time dw ?
	.creation_date dw ?
	.access_date   dw ?
	.reserved2     dw ?
	.mod_time      dw ?
	.mod_date      dw ?
	.start         dw ?
	.size          dd ?
}
DEFN fat_entry

	;; Constants: Attributes
	;;  ATTR_READ_ONLY - Read-only
	;;  ATTR_HIDDEN    - Hidden
	;;  ATTR_SYSTEM    - System
	;;  ATTR_VOLUME_ID - Volume label
	;;  ATTR_DIRECTORY - Subdirectory
	;;  ATTR_ARCHIVE   - Archive
	ATTR_READ_ONLY = 0x01
	ATTR_HIDDEN    = 0x02
	ATTR_SYSTEM    = 0x04
	ATTR_VOLUME_ID = 0x08
	ATTR_DIRECTORY = 0x10
	ATTR_ARCHIVE   = 0x20

	;; Section: Globals

	;; Function: fat_load_root
fat_load_root:
	mov ax, DISK_BUFFER/0x10
	mov es, ax

	; load root directory
	mov ax, [sectors_per_FAT]
	xor cx, cx
	mov cl, [FAT_count]
	mul cx
	add ax, [reserved_sectors]
	push ax

	mov bx, [bytes_per_sector]
	mov cl, 0x5
	shr bx, cl
	mov ax, [root_dir_entries]
	xor dx, dx
	div bx
	mov cx, ax
	pop ax

	mov [data_start], ax
	add [data_start], cx

	xor bx, bx

	call disk_read_sectors

	ret

	;; Function: fat_search_root
	;;
	;; Parameters:
	;;
	;;     SI - file to search
	;;
fat_search_root:
	mov cx, [root_dir_entries]
	mov di, 0x0
@@:
	push si
	push cx
	mov cx, 0xB ; name(8) + ext(3)
	push di
	rep cmpsb
	pop di
	pop cx
	pop si
	je .file_found
	add di, 0x20
	loop @b
	; set carry if not found
	stc
	ret
.file_found:
	mov ax, [es:di + fat_entry.start]
	mov ebx, [es:di + fat_entry.size]
	clc
	ret

	;; Function: fat_load_binary
	;;
	;; Parameters:
	;;     AX    - cluster
	;;     ES:BX - buffer
fat_load_binary:
	push ax
	sub ax, 0x2
	xor cx, cx
	mov cl, [sectors_per_cluster]
	mul cx

	add ax, [data_start]
	xor cx, cx
	mov cl, [sectors_per_cluster]
	call disk_read_sectors

	pop ax
	mov cx, ax
	mov dx, ax
	shr dx, 0x1
	add cx, dx
	push bx
	mov bx, DISK_BUFFER
	add bx, cx
	mov dx, [bx]
	pop bx

	test ax, 0x1
	jnz .odd_cluster
.even_cluster:
	and dx, 0xFFF
	jmp .end
.odd_cluster:
	shr dx, 0x4
.end:
	mov ax, dx
	cmp dx, 0xFF0
	jb fat_load_binary
	ret

data_start dw 0x0
