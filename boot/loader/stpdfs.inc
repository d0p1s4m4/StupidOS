	;; File: stpdfs.inc

STPDFS_BLOCK_SIZE = 512
STPDFS_NAME_MAX   = 28

	;; Struct: Inode
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

STPDFS_INODE_PER_BLOCK equ STPDFS_BLOCK_SIZE / sizeof.Inode

	;; Struct: DirEntry
struc DirEntry {
	.inum dd ?
	.name db STPDFS_NAME_MAX dup(?)
}
DEFN DirEntry

	;; Function: stpdfs_load_rootdir
	;;
	;; Out:
	;;    
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
	xor bx, bx
	call stpdfs_copy_data

	ret

	;; Function: stpdfs_read_inode
	;; 
	;; copy selected inode from file system to `inode_cache` 
	;;
	;; In:
	;;    EAX - inum
	;;
stpdfs_read_inode:
	; get block where inode is located
	xor edx, edx
	mov ecx, STPDFS_INODE_PER_BLOCK
	div ecx
	add eax, 2

	; read block
	push edx
	push eax
	mov ax, DISK_BUFFER/0x10
	mov es, ax
	pop eax
	mov cx, 1
	xor bx, bx
	call disk_read_sectors
	pop edx

	; copy to inode cache
	mov eax, edx
	mov cl, sizeof.Inode
	mul cl
	movzx esi, ax
	add esi, DISK_BUFFER

	xor ax, ax
	mov es, ax
	mov ecx, sizeof.Inode
	mov edi, inode_cache
	rep movsb

	ret

	;; Function: stpdfs_search
	;;
	;; In:
	;;    SI - filename
	;;
	;; Out:
	;;    EAX - kernel inode
stpdfs_search:
	clc	; clear carry flag
	
	xor ax, ax
	mov es, ax

	xor ecx, ecx
.search_loop:
	cmp ecx, dword [inode_cache + Inode.size]
	jge .search_end

	push ecx

	; check if inum != 0
	mov eax, DISK_BUFFER
	add eax, ecx
	mov edx, [eax]

	or edx, edx
	jz .search_next

	; compare filename
	mov edi, eax
	add edi, 4

	push si
.strcmp:
	mov al, [si]
	cmpsb
	jne .strcmp_not_equal
	or al, al
	jz .kernel_found
	jmp .strcmp
.strcmp_not_equal:
	pop si
.search_next:
	pop ecx
	add ecx, sizeof.DirEntry
	jmp .search_loop
.search_end:
	stc
	ret
.kernel_found:
	pop si
	pop ecx

	mov eax, edx

	clc
	ret

	;; Function: stpdfs_copy_data
	;;
	;; In:
	;;	ES:BX - buffer
	;;
stpdfs_copy_data:
	xor edx, edx
	movzx ebx, bx

	;; read direct
@@:
	cmp ebx, dword [inode_cache + Inode.size]
	jg .all_read
	mov eax, edx
	shl eax, 2
	add eax, inode_cache + Inode.zones
	mov eax, [eax]

	push edx
	push ebx
	mov cx, 1
	call disk_read_sectors
	pop ebx
	pop edx

	add ebx, STPDFS_BLOCK_SIZE
	inc edx
	cmp edx, 7
	jb @b

.indirect_read:
	push ebx
	mov ax, es
	push ax

	mov ax, DISK_BUFFER/0x10
	mov es, ax
	
	mov eax, dword [inode_cache + Inode.zones + 28]
	mov cx, 1
	xor bx, bx
	call disk_read_sectors

	pop ax
	mov es, ax

	pop ebx
	xor edx, edx
@@:
	cmp ebx, dword [inode_cache + Inode.size]
	jg .all_read
	mov eax, edx
	shl eax, 2
	add eax, DISK_BUFFER
	mov eax, [eax]

	mov cx, 1
	push edx
	call disk_read_sectors
	pop edx

	add ebx, STPDFS_BLOCK_SIZE
	inc edx
	cmp edx, 128
	jbe @b
.all_read:
	ret
