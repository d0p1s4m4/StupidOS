	;; File: loader.asm
	format binary

	include '../common/const.inc'
	include '../common/macro.inc'
	include 'multiboot.inc'

	org LOADER_BASE
	use32

	jmp _start

	align 4
multiboot_header:
	mb_header MultibootHeader multiboot_header

	;; Function: _start
	;; Loader entry point.
_start:
	cmp eax, MULTIBOOT_MAGIC
	je multiboot

	use16
	align 2

	; =========================================================================
	;  real mode code 
	; =========================================================================
	push cs
	pop ds

	mov [uDrive], dl

	mov si, szMsgStage2
	call bios_log

	; enable A20 line
	call a20_enable
	jc .error_a20

	; check drive type
	; dl <= 0x7F == floppy
	; dl >= 0x80 == hard drive
	; dl == 0xE0 ~= CD/DVD
	; dl <= 0xFF == hard drive
	mov dl, [uDrive]
	cmp dl, 0x7F
	; skip disk extension check
	jbe @f

	; check disk extensions
	mov ah, 0x41
	mov bx, 0x55AA
	int 0x13
	jc @f
	mov [bDriveLBA], TRUE

@@:
	; detect filesystem (FAT12/16 or StpdFS)
	; load kernel from filesystem

	; StupidFS layout
	; +---------+---------+---------....--+----....---+
	; | bootsec | stpd sb | i-nodes ...   | data      |
	; +---------+---------+---------....--+----....---+
	; 0        512      1024             XXXX        XXXX
	;

	; read stupidfs super block
	mov ax, DISK_BUFFER/0x10
	mov es, ax
	mov ax, 1
	mov cx, 1
	xor bx, bx
	call disk_read_sectors

	cmp dword [DISK_BUFFER], STPDFS_MAGIC
	jne  .fat_fallback

	mov si, szMsgStpdFSFound
	call bios_log

	call stpdfs_load_rootdir

	mov si, szKernelStpdfsFile
	call stpdfs_search
	jc .error_not_found

	call stpdfs_read_inode

	mov ebx, dword [inode_cache + Inode.size]
	mov [uKernelSize], ebx

	mov ax, KERNEL_PRELOAD/0x10
	mov es, ax
	xor bx, bx
	call stpdfs_copy_data

	jmp .skip_fat

	; fallback to fat12	
	; for now fat12 is asumed
.fat_fallback:
	mov si, szMsgFatFallback
	call bios_log

	; get bpb
	call fat_read_bpb

	call fat_load_root

	mov si, szKernelFile
	call fat_search_root
	jc .error_not_found
	mov [pKernelStartFat], ax
	mov [uKernelSize], ebx

	push ebx
	mov si, szMsgKernelFound
	call bios_log

	; load fat
	xor ax, ax
	mov al, [FAT_count]
	mul word [sectors_per_FAT]
	mov cx, ax
	mov ax, [reserved_sectors]

	xor bx, bx

	call disk_read_sectors
	
	; load stage 2
	mov ax, KERNEL_PRELOAD/0x10
	mov es, ax
	mov ax, [pKernelStartFat]
	xor bx, bx
	call fat_load_binary

.skip_fat:

	; fetch memory map from bios
	call memory_get_map
	jc .error_memory

	call boot_info_print_mmap

	; video information
	;call video_setup

	; load GDT and enter Protected-Mode
	lgdt [gdt_ptr]

	; enable protected mode
	mov eax, cr0
	or al, CR0_PE
	mov cr0, eax

	; reload ds, es, fs, gs, ss
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp 0x8:common32

.error_not_found:
	mov si, szMsgErrorNotFound
	jmp .error
.error_memory:
	mov si, szMsgErrorMemory
	jmp .error
.error_a20:
	mov si, szMsgErrorA20
.error:
	call bios_log

@@:
	hlt
	jmp @b

	include 'a20.inc'
	include '../common/bios.inc'
	include 'fat.inc'
	include 'stpdfs.inc'
	include 'disk.inc'
	include 'logger.inc'
	include 'memory.inc'
	include 'video.inc'
	include 'gdt.inc'
	include '../common/bootinfo.inc'
	include '../../kernel/sys/register.inc'
	include '../../kernel/sys/mmu.inc'

uDrive    rb 1
bDriveLBA db FALSE

pKernelStartFat    dw 0
uKernelSize        dd 0

szMsgStage2        db "StupidOS Loader", 0
szKernelFile       db "VMSTUPIDSYS", 0
szKernelStpdfsFile db "vmstupid.sys", 0
szMsgStpdFSFound   db "StupidFS found", 0
szMsgFatFallback   db "Fallback to FATFS", 0
szMsgKernelFound   db "Kernel found, size: %x", 0
szMsgErrorA20      db "ERROR: can't enable a20 line", 0
szMsgErrorMemory   db "ERROR: can't detect available memory", 0
szMsgErrorSector   db "ERROR: reading sector", CR, LF, 0
szMsgErrorNotFound db "ERROR: kernel not found", 0

	use32
	; =========================================================================
	; protected mode code
	; =========================================================================

	;; subroutine: multiboot
	;; This subroutine handle multiboot structures and convert them to <BootInfo>
multiboot:
	; https://www.gnu.org/software/grub/manual/multiboot/multiboot.html#Machine-state

	; get screen information

	; get memory map

	; get kernel from module

	;; subroutine: common32
	;; This subroutine move kernel to 0x100000 and setup temporary pagging then jump to kernel code.
common32:
	mov [0xB8000], dword 0x07690748

	; move kernel to 0x100000
	mov ecx, [uKernelSize]
	mov esi, KERNEL_PRELOAD
	mov edi, KERNEL_BASE
	rep movsb

	;; XXX: refactor this code

	; identity map first 1MB
	xor esi, esi
	xor ecx, ecx
	mov edi, boot_0_page_table
@@:
	mov edx, esi
	or edx, (PTE_P or PTE_W)
	mov [edi], edx
	add edi, 4
	add esi, PAGE_SIZE
	inc ecx
	cmp ecx, 1024
	jb @b

	; map 4mb right after kernel (for now we assume there is enough memory)
	mov edi, boot_kernel_paging_table
	xor ecx, ecx
@@:
	mov edx, esi
	or edx, (PTE_P or PTE_W)
	mov [edi], edx
	add edi, 4
	add esi, PAGE_SIZE
	inc ecx
	cmp ecx, 1024
	jb @b

	; add entries to page directory 	
	mov dword [boot_page_directory], boot_0_page_table + (PDE_P or PDE_W) ; present and writable

	mov dword [boot_page_directory + (768 * 4)], boot_0_page_table + (PDE_P or PDE_W)

	mov dword [boot_page_directory + (769 * 4)], boot_kernel_paging_table + (PDE_P or PDE_W)

	mov eax, boot_page_directory
	mov cr3, eax

	mov eax, cr0
	or eax, (CR0_PG or CR0_WP)
	mov cr0, eax

	mov eax, STPDBOOT_MAGIC
	mov ebx, boot_structure

	mov ecx, 0xC0000000 + KERNEL_BASE
	jmp ecx

hang:
	hlt
	jmp $

_edata:

boot_structure BootInfo

inode_cache rb sizeof.Inode

	align 4096
boot_page_directory:
	rb 4096

	; we asume kernel is less than 4mb
boot_0_page_table:
	rb 4096

	; We map 4mb after kernel for future kernel page table/directory
boot_kernel_paging_table:
	rb 4096

_end:
