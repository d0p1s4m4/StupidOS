	;; File: kernel.asm
	include 'const.inc'

	org KBASE
	use32

	jmp short kmain

db 'STPDKRNL'
db 32 dup(0)

	include 'klog.inc'
	include 'mm/mm.inc'

	;; Function: kmain
	;;
	;; Parameters:
	;; 
	;;     EAX - Boot Magic
	;;     EBX - Boot structure address
	;;
kmain:
	xchg bx, bx
	mov [0xC03B0000], dword 0x08690948
	mov [0xC03B0004], dword 0x05690648
	; TODO: interupt, vmm
	cmp eax, STPDBOOT_MAGIC
	jne .halt

	;KLOG_INIT

	;KLOG "kernel alive"

.halt:
	hlt
	jmp $

_edata:

	; BSS
	rb 0x4000

_end:
