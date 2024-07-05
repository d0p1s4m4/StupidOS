CMOS_ADDRESS = 0x70
CMOS_DATA    = 0x71

CMOS_REG_SECOND = 0x00
CMOS_REG_MINUTE = 0x02
CMOS_REG_HOUR   = 0x04

COM1 = 0x3F8

klog_print:
	mov dx, COM1
@@:
	lodsb
	or al, al
	jz @f
	out dx, al
	jmp @b
@@:
	ret

klog_print_date:
@@:
	mov al, 0x0A
	out CMOS_ADDRESS, al
	in al, CMOS_DATA
	and al, 0x80
	jnz @b

	mov al, CMOS_REG_HOUR
	out CMOS_ADDRESS, al
	in al, CMOS_DATA

	mov ah, al
	shr ah, 4
	and ah, 0xF
	and al, 0xF

	add ah, 0x30
	add al, 0x30
	mov [szTime + 1], ah
	mov [szTime + 2], al

	mov al, CMOS_REG_MINUTE
	out CMOS_ADDRESS, al
	in al, CMOS_DATA

	mov ah, al
	shr ah, 4
	and ah, 0xF
	and al, 0xF

	add ah, 0x30
	add al, 0x30
	mov [szTime + 4], ah
	mov [szTime + 5], al

	mov al, CMOS_REG_SECOND
	out CMOS_ADDRESS, al
	in al, CMOS_DATA

	mov ah, al
	shr ah, 4
	and ah, 0xF
	and al, 0xF

	add ah, 0x30
	add al, 0x30

	mov [szTime + 7], ah
	mov [szTime + 8], al


	push esi
	mov esi, szTime
	call klog_print
	pop esi

	ret

	;; Function: klog
	;; Output kernel log
klog:
	call klog_print_date

	call klog_print

	mov esi, szCRLF
	call klog_print
	ret

szTime db '[00:00:00] ', 0
szCRLF db CR, LF, 0
