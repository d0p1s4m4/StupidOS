; File: kernel.s
[BITS 32]

%include "machdep.inc"

section .bss
align 16
stack_bottom:
	resb 16384
global stack_top
stack_top:

section .text
; Function: kmain
global kmain
kmain:
	push ebp
	mov ebp, esp

	leave
	ret

section .data
global machdep
machdep:
	istruc machinfo
		at machinfo.has_cpuid, db 0
		at machinfo.has_pse, db 0
		at machinfo.has_pae, db 0
	iend
