	;; File: stpdfs.inc
	
	;; Section: Stupid Filesystem
	;;
	;; > ┌──────────┬───────────┬──────┬───┬──────┬────┬───┬────┐
	;; > │Boot block│Super block│Inodes│...│Inodes│Data│...│Data│
	;; > └──────────┴───────────┴──────┴───┴──────┴────┴───┴────┘

	;; Constant: STPDFS_SB_MAGIC
	;; Superblock magic number, MUST BE `0x44505453` (STPD)
STPDFS_SB_MAGIC = 0x44505453
	;; Constant: STPDFS_SB_REV
	;; StupidFS revision, currently `0x1`
STPDFS_SB_REV   = 1
	;; Constant: STPDFS_BSIZE
	;; StupidFS block size (512)
STPDFS_BSIZE    = 512
	;; Constant: STPDFS_BADINO
	;; StupidFS bad inode
STPDFS_BADINO = 0
	;; Constant: STPDFS_ROOTINO
	;; StupidFS root inode number
STPDFS_ROOTINO  = 1
	;; Constant: STPDFS_NDIRECT
	;; Number of direct block (7)
STPDFS_NDIRECT  = 7
	;; Constant: STPDFS_INDIRECT_PER_BLOCK
STPDFS_INDIRECT_PER_BLOCK = STPDFS_BSIZE / 4

	;; Constant: STPDFS_NAME_MAX
	;; Max file name length (28)
STPDFS_NAME_MAX = 28

	;; Enum: StupidFS State
	;; STPDFS_CLEANLY_UNMOUNTED - 0
	;; STPDFS_ERROR             - 1
	;; STPDFS_DIRTY             - 1
STPDFS_CLEANLY_UNMOUNTED = 0
STPDFS_ERROR             = 1
STPDFS_DIRTY             = 1

	;; Enum: StupidFS i-node flags
	;; STPDFS_INO_FLAG_ALOC - I-node is allocated
	;; STPDFS_INO_FLAG_LZP  - I-node data is compressed using LZP algorithm (see <lzp.asm>)
	;; STPDFS_INO_FLAG_ENC  - I-node data is encrypted with XChaCha12 (see <xchacha.asm>)
STPDFS_INO_FLAG_ALOC = 0x8000
STPDFS_INO_FLAG_LZP  = 0x0001
STPDFS_INO_FLAG_ENC  = 0x0002

	;; Constant: STPDFS_INODE_PER_BLOCK
	;; I-node per block
STPDFS_INODE_PER_BLOCK = sizeof.StpdFS_Inode / STPDFS_BSIZE

	;; Constant: STPDFS_DIRENT_PER_BLOCK
	;; Directory entry per block
STPDFS_DIRENT_PER_BLOCK = sizeof.StpdFS_Dirent / STPDFS_BSIZE


	;; Struc: StpdFS_FreeList
	;;
	;; .free  - List of free block (0-99), index 0 point to next freelist
	;; .nfree - Index
	;;
	;; > ┌──────────┐                                                     
	;; > │ block 99 │                              
	;; > ├──────────┤                              
	;; > │ block 98 │                              
	;; > ├──────────┤                              
	;; > │ ...      │                              
	;; > ├──────────┤                              
	;; > │ block 2  │                              
	;; > ├──────────┤                              
	;; > │ block 1  │                              
	;; > ├──────────┤    ┌──────────┐              
	;; > │ block 0  ├───►│ block 99 │              
	;; > └──────────┘    ├──────────┤              
	;; >                 │ ...      │              
	;; >                 ├──────────┤    ┌──────────┐
	;; >                 │ block 0  ├───►│ block 99 │
	;; >                 └──────────┘    ├──────────┤
	;; >                                 │ ...      │
	;; >                                 ├──────────┤
	;; >                                 │ block 0  │
	;; >                                 └──────────┘
	;;                                                
struc StpdFS_FreeList {
	.free dd 100 dup(?)
	.nfree db ?
}

	;; Struc: StpdFS_Sb
	;; .magic     - See <STPDFS_SB_MAGIC>
	;; .isize     - Size in block of the i-node list
	;; .fsize     - Size in block of the entire volume
	;; .freelist  - See <StpdFS_FreeList>
	;; .rev       - See <STPDFS_SB_REV>
	;; .state     - See <StupidFS State>
	;; .time      - Last mod time (64bit UNIX timestamp)
struc StpdFS_Sb {
	.magic dd ?
	.isize dd ?
	.fsize dd ?
	.freelist StpdFS_FreeList
	.rev   db ?
	.state dw ?
	.time  dq ?
}
DEFN StpdFS_Sb

	;; Struc: StpdFS_Inode
	;; StupidFS on disk i-node
	;;
	;; .mode    - File mode
	;; .nlink   - Links count
	;; .uid     - Owner Uid
	;; .gid     - Group Id
	;; .flags   - File flags, see <StupidFS i-node flags>
	;; .size    - Data size in byte
	;; .zone    - See bellow
	;; .actime  - Access time (64-bit UNIX timestamp)
	;; .modtime - Modification time (64-bit UNIX timestamp)
	;;
	;;
	;; Zone 0-6 are direct, zone 7 indirect, zone 8 double indirect, zone 9 triple indirect
	;; >                         ┌────────┐                                                
	;; >                         │        │                                                
	;; >                ┌───────►│Data    │                                                
	;; >                │        │        │                                                
	;; > ┌──────┐ Direct│        └────────┘                                                
	;; > │zone 0├───────┘                                                                  
	;; > ├──────┤                                                                          
	;; > │...   │                                                                          
	;; > ├──────┤          ┌────────┐      ┌────────┐                                      
	;; > │zone 6│          │        ├─────►│        │                                      
	;; > ├──────┤ Indirect │        │      │ Data   │                                      
	;; > │zone 7├─────────►│        │      │        │                                      
	;; > ├──────┤          └────────┘      └────────┘                                      
	;; > │zone 8├───────┐                                                                  
	;; > ├──────┤       │Double indirect┌────────┐    ┌────────┐    ┌────────┐           
	;; > │zone 9│       └──────────────►│        ├───►│        ├───►│        │           
	;; > └──┬───┘                       │        │    │        │    │ Data   │           
	;; >    │                           │        │    │        │    │        │           
	;; >    │                           └────────┘    └────────┘    └────────┘           
	;; >    │ Triple indirect ┌────────┐                                                   
	;; >    └────────────────►│        │    ┌────────┐      ┌────────┐      ┌────────┐
	;; >                      │        ├───►│        │      │        │      │        │
	;; >                      │        │    │        ├─────►│        ├─────►│ Data   │
	;; >                      └────────┘    │        │      │        │      │        │
	;; >                                    └────────┘      └────────┘      └────────┘
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


	;; Struc: StpdFS_Dirent
	;; StupidFS directory entry
	;;
	;; .inode - address of i-node
	;; .name  - null terminated file name (see <STPDFS_NAME_MAX>)
struc StpdFS_Dirent {
	.inode dd ?
	.name db STPDFS_NAME_MAX dup(?)
}
DEFN StpdFS_Dirent

	; ------------------------------------------------------------------------

	;; Section: Implementation


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
