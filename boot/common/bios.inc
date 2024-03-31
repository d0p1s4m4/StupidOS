	;; File: bios.inc


	;; Function: bios_print
	;;
	;; Parameters:
	;;
	;;     si - null-terminated string to print
	;;
bios_print:
	lodsb
	or al, al
	jz @f
	mov ah, 0x0E
	int 0x10
	jmp bios_print
@@:
	ret
