	;; File: logger.inc

	;; Macro: LOG
macro LOG msg, [arg] {
	common
		_ARGS = 0
	reverse
		match anything, arg \{
			_ARGS = _ARGS + 4
			push dword arg
		\}
	common
		mov eax, msg
		call efi_log
		if _ARGS
			add esp, _ARGS
		end if
}

	;; XXX: use StdErr instead of ConOut
macro ERROR msg, [arg] {
	common
		_ARGS = 0
	reverse
		match anything, arg \{
			_ARGS = _ARGS + 4
			push dword arg
		\}
	common
		EFI_CALL [fnSetAttribute], [pConOut], EFI_RED
		mov eax, msg
		call efi_log
		if _ARGS
			add esp, _ARGS
		end if
		EFI_CALL [fnSetAttribute], [pConOut], EFI_LIGHTGRAY
}

macro WARN msg, [arg] {
	common
		_ARGS = 0
	reverse
		match anything, arg \{
			_ARGS = _ARGS + 4
			push dword arg
		\}
	common
		EFI_CALL [fnSetAttribute], [pConOut], EFI_YELLOW
		mov eax, msg
		call efi_log
		if _ARGS
			add esp, _ARGS
		end if
		EFI_CALL [fnSetAttribute], [pConOut], EFI_LIGHTGRAY
}

 	section '.text' code executable readable

	;; Function: efi_log_init
efi_log_init:
	mov eax, [pEfiSystemTable]
	mov eax, [eax + EFI_SYSTEM_TABLE.ConOut]
	mov [pConOut], eax

	mov edx, [eax + EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL.Reset]
	mov [fnOutputReset], edx

	mov edx, [eax + EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL.OutputString]
	mov [fnOutputStr], edx

	mov edx, [eax + EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL.SetAttribute]
	mov [fnSetAttribute], edx

	mov eax, [pEfiRuntimeServices]
	mov eax, [eax + EFI_RUNTIMES_SERVICES.GetTime]
	mov [fnGetTime], eax

	EFI_CALL [fnOutputReset], [pConOut], 1
	EFI_CALL [fnSetAttribute], [pConOut], EFI_LIGHTGRAY

	EFI_CALL [fnOutputStr], [pConOut], szBanner

	ret

efi_log_hex:
	push ebp
	mov ebp, esp
	push edi

	mov edi, eax
	mov eax, szHexPrefix
	EFI_CALL [fnOutputStr], [pConOut], szHexPrefix

	xor ecx, ecx
@@:
	cmp ecx, 16
	je @f
	rol edi, 4
	mov eax, edi
	and eax, 0xF
	shl eax, 1
	mov ax, [sDigit + eax]
	mov [szLogBuffer + ecx], ax
	add ecx, 2
	jmp @b

@@:
	mov [szLogBuffer + ecx], word 0
	EFI_CALL [fnOutputStr], [pConOut], szLogBuffer

	pop edi
	leave
	ret

	;; Function: efi_log_time
efi_log_time:
	; GetTime(EFI_TIME *time, EFI_TIME_CAPS *cap)
	EFI_CALL [fnGetTime], stEfiTime, 0

	; Hours
	mov al, [stEfiTime + EFI_TIME.Hour]
	xor ah, ah
	mov cl, 10
	div cl

	add ah, 0x30
	add al, 0x30

	movzx cx, al
	mov [szTime + 2], cx

	movzx cx, ah
	mov [szTime + 4], cx

	; Minutes
	mov al, [stEfiTime + EFI_TIME.Minute]
	xor ah, ah
	mov cl, 10
	div cl

	add ah, 0x30
	add al, 0x30

	movzx cx, al
	mov [szTime + 8], cx

	movzx cx, ah
	mov [szTime + 10], cx

	; Secondes
	mov al, [stEfiTime + EFI_TIME.Second]
	xor ah, ah
	mov cl, 10
	div cl

	add ah, 0x30
	add al, 0x30

	movzx cx, al
	mov [szTime + 14], cx

	movzx cx, ah
	mov [szTime + 16], cx

	EFI_CALL [fnOutputStr], [pConOut], szTime

	ret

	;; Function: efi_log
	;;
	;; In:
	;;    EAX - string to print
efi_log:
	push ebp
	mov ebp, esp

	push esi
	push edi
	push ebx

	mov esi, eax
	mov edi, eax
	lea ebx, [ebp + 8]

	call efi_log_time

.loop:
	mov dx, [edi]
	or dx, dx
	jz .end

	cmp dx, '%'
	jne .next

	mov word [edi], 0
	EFI_CALL [fnOutputStr], [pConOut], esi
	add edi, 2
	mov esi, edi
	mov dx, [edi]

	; check if another '%'
	cmp dx, '%'
	je .next

	; check string format
	cmp dx, 's'
	jne @f
	EFI_CALL [fnOutputStr], [pConOut], [ebx]
	add esi, 2
	add ebx, 4
	jmp .next

	; check if hex format
@@:
	cmp dx, 'x'
	jne @f
	mov eax, [ebx]
	call efi_log_hex
	add esi, 2
	add ebx, 4
	jmp .next

	; unknown format
@@: 
	mov dx, '?'
	mov [edi], dx

.next:
	add edi, 2
	jmp .loop
.end:

	mov dx, [esi]
	or dx, dx
	jz @f
	EFI_CALL [fnOutputStr], [pConOut], esi
@@:
	; print CRLF
	EFI_CALL [fnOutputStr], [pConOut], szEndLine

	pop ebx
	pop edi
	pop esi

	leave
	ret

section '.data' data readable writable

szTime    du '[00:00:00] ', 0
szEndLine du CR, LF, 0
szBanner  du 0x2554, 0x2550, 0x2550, 0x2550, 0x2550, 0x2550, 0x2550, 0x2550, 0x2550, 0x2550, 0x2550, 0x2550, 0x2550, 0x2550, 0x2550, 0x2550, 0x2557 , CR, LF, \
			 0x2551, ' ',    'S',    't',    'u',    'p',    'i',    'd',    ' ',    'L',    'o',    'a',    'd',    'e',    'r',    ' ',    0x2551 , CR, LF, \
			 0x255a, 0x2550, 0x2550, 0x2550, 0x2550, 0x2550, 0x2550, 0x2550, 0x2550, 0x2550, 0x2550, 0x2550, 0x2550, 0x2550, 0x2550, 0x2550, 0x255d , CR, LF, 0
szHexPrefix du '0x', 0
sDigit      du '0123456789ABCDEF'
szLogBuffer du '00000000', 0
;; Variable: pConOut
;; Pointer to EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL
pConOut        dd ?
fnOutputReset  dd ?
fnOutputStr    dd ?
fnSetAttribute dd ?

stEfiTime EFI_TIME
fnGetTime dd ?
