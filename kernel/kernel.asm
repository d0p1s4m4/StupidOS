	;; File: kernel.asm
	include 'const.inc'

	org KBASE
	use32

	jmp kmain

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
	nop

_edata:

	; BSS
	rb 0x4000

_end:
