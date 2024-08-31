STPDFS_NAME_MAX = 28

	;; Struct: inode
struc inode {
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
DEFN inode

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
	mov dword eax, [DISK_BUFFER + sizeof.inode * 2 + inode.size]



	ret

	;; Function: stpdfs_search
	;;
	;; In:
	;;    SI - filename
	;;
stpdfs_search:
	clc	; clear carry flag


	ret

stpdfs_copy_data:
	ret
