bios_log_time:
	clc
	mov ah, 0x02
	int 0x1A

	mov al, ch
	mov ah, al
	shr ah, 4
	and ah, 0xF
	and al, 0xF
	;aam
	add ah, 0x30
	add al, 0x30
	mov [time + 1], ah
	mov [time + 2], al

	mov al, cl
	mov ah, al
	shr ah, 4
	and ah, 0xF
	and al, 0xF
	;aam
	add ah, 0x30
	add al, 0x30
	mov [time + 4], ah
	mov [time + 5], al

	mov al, dh
	mov ah, al
	shr ah, 4
	and ah, 0xF
	and al, 0xF
	;aam
	add ah, 0x30
	add al, 0x30
	mov [time + 7], ah
	mov [time + 8], al

	mov si, time
	call bios_print

	ret

	;; Function: bios_log_number
	;;
	;; Parameters:
	;;    XX - number
	;;
bios_log_number:
	push ebp
	mov ebp, esp
	sub esp, 10

	leave
	ret

	;; Function: bios_log_hex
	;;
bios_log_hex:
	push bp
	mov ebp, esp
	sub esp, 12
	mov si, hex_prefix
	call bios_print

	or edi, edi
	jz .print_zero
.loop:
	or edi, edi
	jz .print_number
	rol edi, 4
	mov eax, edi
	and eax, 0xF
	mov al, [hex_digits + eax]
	mov [esp], al
	inc esp
	jmp .loop
.print_zero:
	mov al, '0'
	mov ah, 0x0E
	int 0x10
	jmp .end
.print_number:
	mov [esp], byte 0
	mov si, [ebp - 12]
	call bios_print
.end:
	leave
	ret

	;; Function: bios_log
	;;
	;; Parameters:
	;;     SI      - string to print
	;;     [STACK] - variadic arguments
	;;
bios_log:
	push si
	call bios_log_time
	pop si

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
	jne .unknown_format
	pop ax
	pop edi
	push ax
	push si
	call bios_log_hex
	pop si
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
	mov si, endline
	call bios_print

	ret

time db '[00:00:00] ', 0
hex_prefix db '0x', 0
hex_digits db '0123456789ABCDEF'
endline db CR, LF, 0
