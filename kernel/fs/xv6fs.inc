	;; File: xv6fs.inc
	;;
	;; Usefull links:
	;; - <https://github.com/mit-pdos/xv6-riscv/blob/riscv/kernel/fs.h>
	;; - <xv6 book at https://pdos.csail.mit.edu/6.1810/2023/xv6/book-riscv-rev3.pdf>
	;;
	;; Section: xv6 Filesystem
	;;
	;; > ┌──────────┬───────────┬───┬───┬───┬──────┬───┬──────┬──────┬───┬──────┬────┬───┬────┐
	;; > │Boot block│Super block│Log│...│Log│Inodes│...│Inodes│Bitmap│...│Bitmap│Data│...│Data│
	;; > └──────────┴───────────┴───┴───┴───┴──────┴───┴──────┴──────┴───┴──────┴────┴───┴────┘
	;;

	;; Constant: XV6FS_BSIZE
	;; xv6 Filesystem block size (1024)
XV6FS_BSIZE     = 1024
	;; Constant: XV6FS_ROOTINO
	;; root inode number
XV6FS_ROOTINO   = 1
	;; Constant: XV6FS_MAGIC
	;; Superblock magic number, MUST BE `0x10203040`
XV6FS_MAGIC     = 0x10203040
	;; Constant: XV6FS_NDIRECT
	;; Number of direct block (12)
XV6FS_NDIRECT   = 12
XV6FS_NINDIRECT = (XV6FS_BSIZE / 4)
XV6FS_MAXFILE   = (XV6FS_NDIRECT + XV6FS_NINDIRECT)
XV6FS_IPB       = (XV6FS_BSIZE / 64)
XV6FS_DIRSIZE   = 14 

	;; Struc: Xv6FS_Sb
	;; xv6FS superblock
	;;
	;; .magic      - See <XV6FS_MAGIC>
	;; .size       - TODO
	;; .nblock     - TODO
	;; .ninodes    - TODO
	;; .nlog       - TODO
	;; .nlogstart  - TODO
	;; .inodestart - TODO
	;; .bmapstart  - TODO
struc Xv6FS_Sb {
	.magic      dd ?
	.size       dd ?
	.nblock     dd ?
	.ninodes    dd ?
	.nlog       dd ?
	.nlogstart  dd ?
	.inodestart dd ?
	.bmapstart  dd ?
}
DEFN Xv6FS_Sb

	;; Struc: Xv6FS_Inode
	;; xv6FS on disk i-node
	;;
	;; .type  - TODO
	;; .major - TODO
	;; .minor - TODO
	;; .nlink - TODO
	;; .size  - TODO
	;; .addrs - TODO
struc Xv6FS_Inode {
	.type  dw ?
	.major dw ?
	.minor dw ?
	.nlink dw ?
	.size  dd ?
	.addrs dd (XV6FS_NDIRECT + 1) dup(?)
}
DEFN Xv6FS_Inode

	;; Struc: Xv6FS_Dirent
	;; xv6FS directory entry
	;;
	;; .inum - inode number
	;; .name - see <XV6FS_DIRSIZE>
struc Xv6FS_Dirent {
	.inum dw ?
	.name db XV6FS_DIRSIZE dup(?)
}

	; ------------------------------------------------------------------------

	;; Section: Implementation


szVfsXv6FSName db 'xv6FS', 0

vops_xv6fs:
	dd 0

vfs_xv6fs:
	dd szVfsXv6FSName
	dd 0
	dd vops_xv6fs

xv6fs_init:
	mov eax, vfs_xv6fs
	call vfs_register
	ret

