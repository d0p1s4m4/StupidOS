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

	mov [drive_number], dl

	mov si, msg_stage2
	call bios_log

	; enable A20 line
	call a20_enable
	jc .error_a20

	; check drive type
	; dl <= 0x7F == floppy
	; dl >= 0x80 == hard drive
	; dl == 0xE0 ~= CD/DVD
	; dl <= 0xFF == hard drive
	mov dl, [drive_number]
	cmp dl, 0x7F
	; skip disk extension check
	jle @f 

	; check disk extensions
	mov ah, 0x41
	mov bx, 0x55AA
	int 0x13
	jc @f
	mov [drive_lba], 1
@@:
	; detect filesystem (FAT12/16 or StpdFS)
	; load kernel from filesystem

    ; fetch memory map from bios
    call memory_get_map
    jc .error_memory

	; video information
	call video_setup

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

.error_memory:
	mov si, msg_error_memory
	jmp .error
.error_a20:
	mov si, msg_error_a20
.error:
	call bios_log
@@:
	hlt
	jmp @b

	include 'a20.inc'
	include '../common/bios.inc'
	include 'logger.inc'
	include 'memory.inc'
	include 'video.inc'
	include 'gdt.inc'

drive_number rb 1
drive_lba    db 0

msg_stage2        db "StupidOS Loader", 0
kernel_fat12_file db "VMSTUPIDSYS", 0
config_fat12_file db "BOOT    INI", 0
msg_error_a20     db "ERROR: can't enable a20 line", 0
msg_error_memory  db "ERROR: can't detect available memory", 0

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
	mov dword [0xb8000], 0x07690748

	; paging 
	; identity map first 1MB
	; map kernel to 0xC0000000

	push STPDBOOT_MAGIC

	mov eax, 0xC0000000
	jmp eax
hang:
	hlt
	jmp $

_edata:

boot_structure:

	align 4096
boot_page_directory:
	rb 4096

boot_768_page_table:
	rb 4096

_end:
