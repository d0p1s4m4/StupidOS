STPDFS_NAME_MAX = 28

	;; Struct: inode
struc Inode {
	.inode   dw ?
	.nlink   dw ?
	.uid     dw ?
	.gid     dw ?
	.flags   dw ?
	.size    dd ?
	.zones   dd 10 dup(?)
	.actime  dq ?
	.modtime dq ?
}
DEFN Inode

	;; Struct: dirent
struc dirent {
	.inum dd ?
	.name db STPDFS_NAME_MAX dup(?)
}
DEFN dirent

	;; Function: stpdfs_load_rootdir
	;;
	;; Out:
stpdfs_load_rootdir:
	; read first inode
	mov ax, DISK_BUFFER/0x10
	mov es, ax
	mov ax, 2
	mov cx, 1
	xor bx, bx

	call disk_read_sectors

	; root dir is inode 1
	xor ax, ax
	mov es, ax
	mov ecx, sizeof.Inode
	mov esi, DISK_BUFFER + sizeof.Inode
	mov edi, inode_cache
	rep movsb

	mov ax, DISK_BUFFER/0x10
	mov es, ax

	call stpdfs_copy_data

	xchg bx, bx

	ret

	;; Function: stpdfs_search
	;;
	;; In:
	;;    SI - filename
	;;
stpdfs_search:
	clc	; clear carry flag


	ret

	;; Function: stpdfs_copy_data
	;;
	;; In:
	;;	ES  - Buffer
stpdfs_copy_data:
	xor edx, edx
	mov bx, es
	movzx ebx, bx
@@:
	mov ax, es
	movzx ecx, ax
	sub ecx, ebx
	cmp ecx, dword [inode_cache + Inode.size]
	jg .all_read

	mov eax, edx
	shl eax, 2
	add eax, dword [inode_cache + Inode.zones]

	push edx
	push ebx
	mov cx, 1
	xor bx, bx
	call disk_read_sectors
	pop ebx
	pop edx

	mov ax, es
	add ax, 512
	mov es, ax
	inc edx
	cmp edx, 8
	jbe @b

.indirect_read:

.all_read:
	ret
