	;; File: bios.inc

struc bios_disk_packet
{
	.size      db 0x10
	.zero      db 0
	.sectors   dw ?
	.segment   dw ?
	.offset    dw ?
	.lba_lower dd ?
	.lba_upper dd ?
}

	;; Function: bios_print
	;;
	;; Parameters:
	;;
	;;     SI - null-terminated string to print
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
