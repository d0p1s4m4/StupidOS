	;; File: fat.inc
	;;
	;;
	;; Usefull links:
	;; - <Microsoft FAT Specification at https://academy.cba.mit.edu/classes/networking_communications/SD/FAT.pdf>

FAT_SECTOR_SIZE = 512
FAT_MDIR_SIZE   = 32

struc FATFS_BootSectorBase {
	.jump         db 3 dup(?)
	.oemName      db 8 dup(?)
	.byts_per_sec dw ?
	.sec_per_clus db ?
	.rsvd_sec_cnt dw ?
	.num_fats     db ?
	.root_ent_cnt dw ?
	.tot_sec16    dw ?
	.media        db ?
	.fat_sz16     dw ?
	.sec_per_trk  dw ?
	.num_heads    dw ?
	.hidd_sec     dd ?
	.tot_sec32    dd ?
}

struc FATFS_BootSector16 {
	.base FATFS_BootSectorBase

	;; Extended BPB Structure
	.drv_num      db ?
	.reserved1    db 0
	.boot_sig     db ?
	.volume_id    dd ?
	.volume_label db 11 dup(?)
	.filsys_type  db 8 dup(?)
	.code         db 448 dup(?)
	.signature    dw ?
}
DEFN FATFS_BootSector16

struc FATFS_BootSector32 {
	.base FATFS_BootSectorBase

	.fat_sz32     dd ?
	.ext_flags    dw ?
	.fs_ver       dw ?
	.root_clus    dd ?
	.fs_info      dw ?
	.bk_boot_sec  dw ?
	.reserved0    db 12 dup(?)
	.drv_num      db ?
	.reserved1    db ?
	.boot_sig     db ?
	.volume_id    dd ?
	.volume_label db 11 dup(?)
	.filsys_type  db 8 dup(?)
	.code         db 420 dup(?)
	.signature    dw ?
}
DEFN FATFS_BootSector32

struc FATFS_Dirent {
	.name           db 8 dup(?)
	.ext            db 3 dup(?)
	.attr           db ?
	.reserved       db 0
	.crt_time_tenth db ?
	.crt_time       dw ?
	.crt_date       dw ?
	.lst_acc_date   dw ?
	.fst_clus_hi    dw ?
	.wrt_time       dw ?
	.wrt_date       dw ?
	.fst_clus_lo    dw ?
	.file_size      dd ?
}
DEFN FATFS_Dirent

	; ------------------------------------------------------------------------

	;; Section: Implementation

szVfsFatName db 'FATFS', 0

vfs_fatfs:
	dd szVfsFatName
	dd 0
	dd vops_fatfs

vops_fatfs:
	dd 0

fatfs_init:
	mov eax, vfs_fatfs
	call vfs_register
	ret

fatfs_mount:
	ret

fatfs_start:
	ret

fatfs_unmount:
	ret

fatfs_root:
	ret

fatfs_statvfs:
	ret

fatfs_loadvnode:
	ret

fatfs_newvnode:
	ret

fatfs_mountroot:
	ret

;; vnode ops
fatfs_open:
	ret

fatfs_close:
	ret

fatfs_rdwr:
	ret

fatfs_ioctl:
	ret

fatfs_select:
	ret

fatfs_getattr:
	ret

fatfs_setattr:
	ret

fatfs_access:
	ret

fatfs_lookup:
	ret

fatfs_create:
	ret
