	;; File: vfs.inc
	;;
	;; Usefull links:
	;; - <https://www.cs.fsu.edu/~awang/courses/cop5611_s2024/vnode.pdf>
	;;

struc VFS {
	.next   dd ?
	.op     dd ?
	.vnodes dd ?
	.flag   dd ?
	.bsize  dd ?
	.data   dd ?
}

struc VFSOps {
	.mount  dd ?
	.umount dd ?
	.root   dd ?
	.statfs dd ?
	.sync   dd ?
	.fid    dd ?
	.vget   dd ?
}

VNODE_TYPE_NON  = 0x0
VNODE_TYPE_REG  = 0x1
VNODE_TYPE_DIR  = 0x2
VNODE_TYPE_BLK  = 0x3
VNODE_TYPE_CHR  = 0x4
VNODE_TYPE_LNK  = 0x5
VNODE_TYPE_SOCK = 0x6
VNODE_TYPE_BAD  = 0x7

struc VNode {
	.flag     dw ?
	.refcount dw ?
	.shlockc  dw ?
	.exlockc  dw ?
	.vfs      dd ?
	.op       dd ?
	.sock     dd ?
	.vfsp     dd ?
	.type     db ?
	.data     dd ?
}

struc VNodeOps {
	.open     dd ?
	.close    dd ?
	.rdwr     dd ?
	.ioctl    dd ?
	.select   dd ?
	.getattr  dd ?
	.setattr  dd ?
	.access   dd ?
	.lookup   dd ?
	.create   dd ?
	.remove   dd ?
	.link     dd ?
	.rename   dd ?
	.mkdir    dd ?
	.rmdir    dd ?
	.readdir  dd ?
	.symlink  dd ?
	.readlink dd ?
	.fsync    dd ?
	.inactive dd ?
	.bmap     dd ?
	.strategy dd ?
	.bread    dd ?
	.brelse   dd ?
}

vfs_init:
	ret

	
