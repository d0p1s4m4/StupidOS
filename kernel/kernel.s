; File: kernel.s

[BITS 32]

; Function: kmain
.global kmain
kmain:
	push ebp
	mov ebp, esp

	leave
	ret
