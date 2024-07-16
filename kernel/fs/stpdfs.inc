	;; File: stpdfs.inc

STPDFS_SB_MAGIC = 0x44505453
STPDFS_SB_REV   = 1
STPDFS_BSIZE    = 512
STPDFS_ROOTINO  = 1
STPDFS_NDIRECT  = 7

STPDFS_CLEANLY_UNMOUNTED = 0
STPDFS_ERROR             = 1
STPDFS_DIRTY             = 1

struc StpdFS_Sb {
	.magic dd ?
	.isize dd ?
	.fsize dd ?
	.free  dd 100 dup(?)
	.nfree db ?
	.rev   db ?
	.state dw ?
	.time  dq ?
}
DEFN StpdFS_Sb

struc StpdFS_Inode {
	.mode    dw ?
	.nlink   dw ?
	.uid     dw ?
	.gid     dw ?
	.flags   dw ?
	.size    dd ?
	.zones   dd (STPDFS_NDIRECT + 3) dup(?)
	.actime  dq ?
	.modtime dq ?
}
DEFN StpdFS_Inode

szVfsStpdFSName db 'StupidFS', 0

vops_stpdfs:
	dd 0

vfs_stpdfs:
	dd szVfsStpdFSName
	dd 0
	dd vops_stpdfs

stpdfs_init:
	mov eax, vfs_stpdfs
	call vfs_register
	ret
