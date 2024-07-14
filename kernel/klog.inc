	;; File: klog.inc
	;; Kernel logging utilities

CMOS_COMMAND = 0x70
CMOS_DATA    = 0x71

CMOS_REG_SECOND = 0x00
CMOS_REG_MINUTE = 0x02
CMOS_REG_HOUR   = 0x04

COM1 = 0x3F8

klog_kputc:
	mov dx, COM1 + 5
	push eax
@@:
	in al, dx
	and al, 0x20
	jnz @b
	pop eax
	mov dx, COM1
	out dx, al
	ret


	;; Function: klog_print
	;;
	;; In:
	;;    ESI - Null-terminated string to print
klog_print:
	mov dx, COM1
@@:
	lodsb
	or al, al
	jz @f
	out dx, al
	;call klog_kput
	jmp @b
@@:
	ret

	;; Function: klog_print_time
	;;
klog_print_time:
@@:
	mov al, 0x0A
	out CMOS_COMMAND, al
	in al, CMOS_DATA
	and al, 0x80
	jnz @b

	mov al, CMOS_REG_HOUR
	out CMOS_COMMAND, al
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
	out CMOS_COMMAND, al
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
	out CMOS_COMMAND, al
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

klog_print_hex:
	push esi
	mov esi, szHexPrefix
	call klog_print
	pop esi

	or edi, edi
	jz .print_zero
	push esi
	mov esi, szKlogBuffer
	xor cl, cl
.loop:
	cmp cl, 8
	je .print_number
	rol edi, 4
	mov eax, edi
	and eax, 0xF
	mov al, byte [sDigit + eax]
	mov [esi], al
	inc esi
	inc cl
	jmp .loop
.print_zero:
	mov al, '0'
	out dx, al
	jmp .end
.print_number:
	mov [esi], byte 0
	mov esi, szKlogBuffer
	call klog_print
	pop esi
.end:
	ret

	;; Function: klog
	;; Output kernel log
klog:
	call klog_print_time

.loop:
	mov al, [esi]
	or al, al
	jz .end
	cmp al, '%'
	jne .putchar
	inc esi
	mov al, [esi]
	cmp al, '%'
	je .putchar
	cmp al, 's'
	jne .check_x
	mov edi, esi
	pop eax
	pop esi
	push eax
	call klog_print
	mov esi, edi
	jmp .next
.check_x:
	cmp al, 'x'
	jne .unknown_format
	pop eax
	pop edi
	push eax
	call klog_print_hex
	jmp .next
.unknown_format:
	mov al, '?'
.putchar:
	out dx, al
.next:
	inc esi
	jmp .loop
.end:
	mov esi, szCRLF
	call klog_print
	ret

szTime       db '[00:00:00] ', 0
szCRLF       db CR, LF, 0
sDigit       db '0123456789ABCDEF'
szHexPrefix  db '0x', 0
szKlogBuffer db '00000000', 0
