	;; File: fat.inc

FAT_SECTOR_SIZE = 512
FAT_MDIR_SIZE   = 32

struc FatBootSector {
	.jump   db 3 dup(?)
	.banner db 8 dup(?)
}
