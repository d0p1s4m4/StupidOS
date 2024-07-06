	;; File: kernel.asm
	format binary

	include 'const.inc'

	org KBASE
	use32

	jmp short kmain

db 'STPDKRNL'
db 32 dup(0)

	;; Function: kmain
	;;
	;; Parameters:
	;; 
	;;     EAX - Boot Magic
	;;     EBX - Boot structure address
	;;
kmain:
	xchg bx, bx

	mov esp, stack_top
	
	cmp eax, STPDBOOT_MAGIC
	jne .error_magic

	; init memory manager
	; init idt, gdt
	; copy boot structure 
	;call vga_console_clear

	mov [0xC03B0000], dword 0x08740953
	mov [0xC03B0004], dword 0x05700675
	mov [0xC03B0008], dword 0x03640469
	mov [0xC03B000C], dword 0x0153024F


	;KLOG_INIT

	mov esi, szMsgKernelAlive
	call klog

.halt:
	hlt
	jmp $
.error_magic:
	mov esi, szErrorBootProtocol
.error:
	call klog
	jmp .halt

	include 'klog.inc'
	include 'dev/vga_console.inc'
	include 'mm/mm.inc'

szMsgKernelAlive db "Kernel is alive", 0
szErrorBootProtocol db "Error: wrong magic number", 0

	align 4
stack_bottom:
	rb 0x4000
stack_top:

_end:
	dd 0x0
