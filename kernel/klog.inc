	;; File: klog.inc
	;; Kernel logging utilities
	;;

COM1 = 0x3F8

	;; Macro: TRACE
macro TRACE msg, [arg] {
common
	_ARGS = 0
reverse
	match anything, arg \{
		_ARGS = _ARGS + 4
		push dword arg
	\}
common
	CGA_SET_COLOR CGA_COLOR_FG_CYAN
	mov eax, msg
	call klog
	if _ARGS
		add esp, _ARGS
	end if
	CGA_SET_COLOR CGA_COLOR_FG_LIGHT_GRAY
}

	;; Function: _klog_print_str
	;;
	;; In: EAX - Null-terminated string to print
_klog_print_str:
	push ebp
	mov ebp, esp
	
	push esi
	mov dx, COM1
	mov esi, eax
@@:
	lodsb
	or al, al
	jz @f
	out dx, al

	pusha
	call cga_putc
	popa
	
	jmp @b
@@:
	pop esi
	leave
	ret

	;; Function: _klog_print_int
	;;
	;; In:
	;;    EAX - number
_klog_print_int:
	test eax, eax
	jns @f
	; TODO print '-'
	mov ecx, eax
	mov dx, COM1
	mov al, '-'
	out dx, al
	pusha
	call cga_putc
	popa
	mov eax, ecx
@@:
	mov ecx, eax
	sar ecx, 31
	xor ecx, eax
	sub ecx, eax
	
	mov ecx, eax
	call _klog_print_unsigned

	ret

	;; Function: _klog_print_unsigned
	;;
	;; In:
	;;     EAX - number
_klog_print_unsigned:
	push ebx
	mov ebx, 10
	xor ecx, ecx
@@:
	xor edx, edx
	div ebx
	push edx
	inc ecx
	or eax, eax
	jnz @b

@@:
	pop eax
	add al, '0'
	mov dx, COM1
	out dx, al
	pusha
	call cga_putc
	popa

	dec ecx
	or cl, cl
	jnz @b

	pop ebx
	
	ret

	;; Function: _klog_print_hex
	;;
	;; In:
	;;    EAX - number
_klog_print_hex:
	push ebp
	mov ebp, esp

	push edi
	mov edi, eax
	
	mov eax, szHexPrefix
	call _klog_print_str

	or edi, edi
	jz .print_zero

	xor cl, cl
.begin:
	cmp cl, 8
	je .print_number
	rol edi, 4
	mov eax, edi
	and eax, 0xF
	mov al, byte [sDigit + eax]
	mov [szKlogBuffer + ecx], al
	inc cl
	jmp .begin

.print_zero:
	mov al, '0'
	mov dx, COM1
	out dx, al
	pusha
	call cga_putc
	popa
	jmp .end

.print_number:
	mov [szKlogBuffer + ecx], byte 0
	mov eax, szKlogBuffer
	call _klog_print_str
	
.end:	
	pop edi
	leave
	ret
	
	;; Function: _klog_print_time
	;; 
_klog_print_time:
	; wait CMOS ready
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

	mov eax, szTime
	call _klog_print_str

	ret

	;; Function: klog
	;; 
	;; Output kernel log
	;;
	;; In:
	;;    EAX      - null-terminated string
	;;    STACK[X] - var_arg
klog:
	push ebp
	mov ebp, esp

	push esi
	mov esi, eax
	
	; display log time
	call _klog_print_time

	mov ecx, 8
.begin:
	mov al, [esi]
	or al, al
	jz .end
	; search for formating
	cmp al, '%'
	jne .putchar
	; read next char
	inc esi
	mov al, [esi]

	; check if another '%'
	cmp al, '%'
	je .putchar
	
	;  check if string format
	cmp al, 's'
	jne @f
	; print string
	mov eax, [ebp + ecx]
	push ecx
	call _klog_print_str
	pop ecx
	add ecx, 4
	
	jmp .next

	; check if hex format
@@:
	cmp al, 'x'
	jne @f

	mov eax, [ebp + ecx]
	push ecx
	call _klog_print_hex
	pop ecx
	add ecx, 4

	jmp .next

	; check if int format
@@:
	cmp al, 'd'
	jne @f

	mov eax, [ebp + ecx]
	push ecx
	call _klog_print_int
	pop ecx
	add ecx, 4

	jmp .next

	; check if unsigned
@@: 
	cmp al, 'u'
	jne @f

	mov eax, [ebp + ecx]
	push ecx
	call _klog_print_unsigned
	pop ecx
	add ecx, 4

	jmp .next
	
	; unknown format
@@:
	mov al, '?'

.putchar: 			; XXX: fix this mess
	mov dx, COM1
	out dx, al
	pusha
	call cga_putc
	popa

.next:
	inc esi
	jmp .begin

.end:
	mov eax, szCRLF
	call _klog_print_str

	pop esi

	leave
	ret

szTime       db '[00:00:00] ', 0
szCRLF	     db CR, LF, 0
sDigit       db '0123456789ABCDEF'
szHexPrefix  db '0x', 0	
szKlogBuffer db '00000000', 0
