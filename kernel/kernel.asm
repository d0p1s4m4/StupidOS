	;; File: kernel.asm
	include 'const.inc'

	org KBASE
	use32

	jmp kmain

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
	; TODO: interupt, vmm
	cmp eax, STPDBOOT_MAGIC
	jne .halt

	KLOG_INIT

	KLOG "kernel alive"

.halt:
	hlt
	jmp $

_edata:

	; BSS
	rb 0x4000

_end:
