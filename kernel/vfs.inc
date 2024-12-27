	;; File: vfs.inc
	;;
	;; Usefull links:
	;; - <https://www.cs.fsu.edu/~awang/courses/cop5611_s2024/vnode.pdf>
	;;

	;; Struc: VFS
	;; 
	;; .name - Filesystem name
	;; .next - pointer to next VFS
	;; .ops  - pointer to VFS function
struc VFS {
	.name    dd ?
	.next    dd ?
	.ops_ptr dd ?
}
DEFN VFS

	;; Struc: Mount
	;;
	;; .next    - address of next mount
	;; .ops_ptr - address of mount ops
	;; .vnodes  -  
struc Mount {
	.next    dd ?
	.ops_ptr dd ?
	.vnodes  dd ?
	.flag    dd ?
	.bsize   dd ?
	.data    dd ?
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

macro VNODE_TYPE name,value {
VNODE_TYPE_#name equ value
}
	;; Constants: vnode types
	;;
	;; VNODE_TYPE_NON  - XXX
	;; VNODE_TYPE_REG  - XXX
	;; VNODE_TYPE_DIR  - XXX
	;; VNODE_TYPE_BLK  - XXX
	;; VNODE_TYPE_CHR  - XXX
	;; VNODE_TYPE_LNK  - XXX
	;; VNODE_TYPE_SOCK - XXX
	;; VNODE_TYPE_BAD  - XXX
VNODE_TYPE NON, 0x0
VNODE_TYPE REG, 0x1
VNODE_TYPE DIR, 0x2
VNODE_TYPE BLK, 0x3
VNODE_TYPE CHR, 0x4
VNODE_TYPE LNK, 0x5
VNODE_TYPE SOCK, 0x6
VNODE_TYPE BAD, 0x7

	;; Struc: VNode
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

	;; Function: vfs_register
	;;
	;; In:
	;;    EAX - Pointer to VFS struct
	;;
vfs_register:
	mov edx, [pVFS]
	mov [eax + VFS.next], edx
	mov [pVFS], eax

	mov eax, [eax + VFS.name]
	push eax
	mov esi, szMsgVFSRegister
	call klog
	ret

vfs_init:
	call fatfs_init
	call stpdfs_init
	call xv6fs_init
	ret

pVFS    dd 0
pRootFS dd 0

szMsgVFSRegister db 'VFS: register %s', 0
