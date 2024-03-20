	INCLUDE 'const.inc'

	ORG KBASE
	USE32

	jmp kmain

	INCLUDE 'mm/mm.inc'

kmain:
	nop

_edata:

	; BSS
	rb 0x4000

_end:
