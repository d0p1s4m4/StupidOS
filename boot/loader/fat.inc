	;; File: fat.inc

include '../common/fat12.inc'

	;; Function: fat_read_bpb
fat_read_bpb:
	mov ax, DISK_BUFFER/0x10
	mov es, ax
	xor bx, bx
	mov ax, 0
	mov cx, 1
	call disk_read_sectors

	mov word ax, [DISK_BUFFER + fat_bpb.sects_per_track]
	mov word [sectors_per_track], ax
	mov word ax, [DISK_BUFFER + fat_bpb.heads_per_cyl]
	mov word [heads_per_cylinder], ax
	mov word ax, [DISK_BUFFER + fat_bpb.bytes_per_sects]
	mov word [bytes_per_sector], ax
	mov word ax, [DISK_BUFFER + fat_bpb.sects_per_FAT]
	mov word [sectors_per_FAT], ax
	mov byte al, [DISK_BUFFER + fat_bpb.FAT_count]
	mov byte [FAT_count], al
	mov word ax, [DISK_BUFFER + fat_bpb.reserved_sects]
	mov word [reserved_sectors], ax
	mov word ax, [DISK_BUFFER + fat_bpb.root_dir_entries]
	mov word [root_dir_entries], ax
	mov byte al, [DISK_BUFFER + fat_bpb.sects_per_clust]
	mov byte [sectors_per_cluster], al
	ret

