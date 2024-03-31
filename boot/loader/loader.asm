	;; File: loader.asm
	format binary

	include '../common/const.inc'
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

	; =========================================================================
	;  real mode code 
	; =========================================================================
	push cs
	pop ds

	mov si, msg_stage2
	call bios_print

	call a20_enable
	jc .error_a20

    ; detect memory
    call memory_get_map
    jc .error_memory

	call video_setup

	;cli
	;lgdt [gdt_ptr]
	;jmp 0x8:common32
	;jmp $

.error_memory:
	mov si, msg_error_memory
	jmp .error
.error_a20:
	mov si, msg_error_a20
.error:
	call bios_print
@@:
	hlt
	jmp @b

	include 'a20.inc'
	include '../common/bios.inc'
	include 'memory.inc'
	include 'video.inc'
	include 'gdt.inc'

msg_stage2       db "StupidOS Bootloader (Stage 1)", CR, LF, 0
msg_error_a20    db "ERROR: can't enable a20 line", CR, LF, 0
msg_error_memory db "ERROR: can't detect available memory", CR, LF, 0

	;;
bi_screen_width:	dw 0
bi_screen_height:	dw 0

	use32
	; =========================================================================
	; protected mode code
	; =========================================================================
multiboot:

common32:
	;mov bx, 0x0f01
	;mov word [eax], bx
	; paging 
	; identity map first 1MB
	; map kernel to 0xC0000000

hang:
	hlt
	jmp $

_edata:

	align 4096
boot_page_directory:
	rb 4096

boot_0_page_table:
	rb 4096

boot_768_page_table:
	rb 4096

_end:
