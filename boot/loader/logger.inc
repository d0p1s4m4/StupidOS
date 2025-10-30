bios_log_time:
	clc
	mov ah, 0x02
	int 0x1A

	; print hours
	mov al, ch
	mov ah, al
	shr ah, 4
	and ah, 0xF
	and al, 0xF

	add ah, 0x30
	add al, 0x30
	mov [szTime + 1], ah
	mov [szTime + 2], al

	; print minutes
	mov al, cl
	mov ah, al
	shr ah, 4
	and ah, 0xF
	and al, 0xF

	add ah, 0x30
	add al, 0x30
	mov [szTime + 4], ah
	mov [szTime + 5], al

	; print seconds
	mov al, dh
	mov ah, al
	shr ah, 4
	and ah, 0xF
	and al, 0xF

	add ah, 0x30
	add al, 0x30
	mov [szTime + 7], ah
	mov [szTime + 8], al

	push si
	mov si, szTime
	call bios_print
	pop si

	ret

	;; Function: bios_log_number
	;;
	;; Parameters:
	;;     DI - number
bios_log_number:
	push dx
	push bx
	xor cx, cx
	mov ax, di
	mov bx, 10
.loop_calc:
	xor dx, dx
	div bx
	push dx
	inc cx
	or ax, ax
	jnz .loop_calc
.loop_print:
	pop ax
	add al, '0'
	mov ah, 0x0E
	int 0x10
	dec cx

	or cl, cl
	jnz .loop_print

	pop bx
	pop dx
	ret

	;; Function: bios_log_hex
	;;
	;; Parameters:
	;;     EDI - number
bios_log_hex:
	push si
	mov si, szHexPrefix
	call bios_print
	pop si

	or edi, edi
	jz .print_zero
	push si
	mov si, szBuffer
	xor cl, cl
.loop:
	cmp cl, 8
	je .print_number
	rol edi, 4
	mov eax, edi
	and eax, 0xF
	mov al, byte [sDigits + eax]
	mov [si], al
	inc si
	inc cl
	jmp .loop
.print_zero:
	mov al, '0'
	mov ah, 0x0E
	int 0x10
	jmp .end
.print_number:
	mov [si], byte 0
	mov si, szBuffer
	call bios_print
	pop si
.end:
	ret

bios_log_warn:
	call bios_log_time
	push si
	mov si, szLogWarn
	jmp bios_log_error.common

	;; Function: bios_log_error
	;;
bios_log_error:
	call bios_log_time
	push si
	mov si, szLogError
.common:
	call bios_print
	pop si
	jmp bios_log.loop

	;; Function: bios_log
	;;
	;; Parameters:
	;;     SI      - string to print
	;;     [STACK] - variadic arguments
	;;
bios_log:
	call bios_log_time

.loop:
	mov al, [si]
	or al, al
	jz .end
	cmp al, '%'
	jne .putchar
	inc si
	mov al, [si]
	cmp al, '%'
	je .putchar
	cmp al, 's'
	jne .check_x
	mov di, si
	pop ax
	pop si
	push ax
	call bios_print
	mov si, di
	jmp .next
.check_x:
	cmp al, 'x'
	jne .check_d
	pop ax
	pop dword edi
	push ax
	call bios_log_hex
	jmp .next
.check_d:
	cmp al, 'd'
	jne .unknown_format
	pop ax
	pop word di
	push ax
	call bios_log_number
	jmp .next
.unknown_format:
	mov al, '?'
.putchar:
	mov ah, 0x0E
	int 0x10
.next:
	inc si
	jmp .loop
.end:
	mov si, szEndLine
	call bios_print

	ret

szTime      db '[00:00:00] ', 0
szHexPrefix db '0x', 0
sDigits     db '0123456789ABCDEF'
szEndLine   db CR, LF, 0
szBuffer    db '00000000', 0
szLogError  db 'ERROR: ', 0
szLogWarn   db 'WARN: ', 0
