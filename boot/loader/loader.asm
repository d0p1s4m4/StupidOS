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
	je .multiboot

	use16
	;; non multiboot process
	push cs
	pop ds

	mov si, msg_stage2
	call bios_print

	call a20_enable
	jc .error_a20

    ; detect memory
    call memory_get_map
    jc .error_memory
    xchg bx, bx

	call video_setup

.multiboot:
	jmp .hang

.error_memory:
	mov si, msg_error_memory
	jmp .error
.error_a20:
	mov si, msg_error_a20
.error:
	call bios_print
.hang:
	hlt
	jmp $

	include 'a20.inc'
	include '../common/bios.inc'
	include 'memory.inc'
	include 'video.inc'

msg_stage2       db "StupidOS Bootloader (Stage 1)", CR, LF, 0
msg_error_a20    db "ERROR: can't enable a20 line", CR, LF, 0
msg_error_memory db "ERROR: can't detect available memory", CR, LF, 0

	;;
bi_screen_width:	dw 0
bi_screen_height:	dw 0

_edata:

	; BSS
	rb 0x4000

_end:
