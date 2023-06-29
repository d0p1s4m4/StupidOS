
struc superblock
	.devid: resd 1
	.size: resd 1
	.ninode: resd 1
	.fs_type: resd 1
endstruc

struc inode
	.devid: resd 1
	.size: resd 1
	.inode: resd 1s
	.access: resd 1
endstruc
