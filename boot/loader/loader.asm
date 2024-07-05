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
	jle @f

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
	
	; for now fat12 is asumed

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
	or al, 1
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
	include '../common/fat12.inc'
	include 'disk.inc'
	include 'logger.inc'
	include 'memory.inc'
	include 'video.inc'
	include 'gdt.inc'
	include '../common/bootinfo.inc'

uDrive    rb 1
bDriveLBA db FALSE

pKernelStartFat    dw 0
uKernelSize        dd 0

szMsgStage2        db "StupidOS Loader", 0
szKernelFile       db "VMSTUPIDSYS", 0
szMsgKernelFound   db "Kernel found, size: %x", 0
szMsgErrorA20      db "ERROR: can't enable a20 line", 0
szMsgErrorMemory   db "ERROR: can't detect available memory", 0
szMsgErrorSector   db "ERROR: reading sector", CR, LF, 0
szMsgErrorNotFound db "ERROR: kernel not found", 0

	use32
	; =========================================================================
	; protected mode code
	; =========================================================================
multiboot:
	; https://www.gnu.org/software/grub/manual/multiboot/multiboot.html#Machine-state

	; get screen information

	; get memory map

	; get kernel from module


common32:
	mov [0xB8000], dword 0x07690748

	; identity map first 1MB
	xor esi, esi
	xor ecx, ecx
	mov edi, boot_0_page_table
@@:
	mov edx, esi
	or edx, 0x3
	mov [edi], edx
	add edi, 4
	add esi, 4096
	inc ecx
	cmp ecx, 1024
	jb @b
	
	mov dword [boot_page_directory], boot_0_page_table + 0x3 ; preset and writable

	; map kernel at 0xC0000000
	mov esi, KERNEL_PRELOAD
	mov edi, boot_768_page_table
	xor ecx, ecx
@@:
	mov edx, esi
	or edx, 0x3
	mov [edi], edx
	add edi, 4
	add esi, 4096
	add ecx, 4096
	cmp ecx, [uKernelSize]
	jbe @b

	; map 0xB8000 (vga) to 0xC03B0000
	; 0xC03B0000 >> 12 & 0x3FF == 944
	mov dword [boot_768_page_table + 944 * 16], 0xB8000 + 0x3
	mov dword [boot_page_directory + (768 * 4)], boot_768_page_table + 0x3

	mov eax, boot_page_directory
	mov cr3, eax

	mov eax, cr0
	or eax, 0x80010000
	mov cr0, eax

	push boot_structure
	push STPDBOOT_MAGIC

	mov eax, 0xC0000000
	jmp eax
hang:
	hlt
	jmp $

_edata:

boot_structure BootInfo

	align 4096
boot_page_directory:
	rb 4096

boot_768_page_table:
	rb 4096

boot_0_page_table:
	rb 4096

_end:
