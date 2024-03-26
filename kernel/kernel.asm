	;; File: kernel.asm
	include 'const.inc'

	org KBASE
	use32

	jmp kmain

	include 'mm/mm.inc'

kmain:
	nop

_edata:

	; BSS
	rb 0x4000

_end:
