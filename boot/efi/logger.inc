	section '.text' code executable readable

	;; Function: efi_log_init
efi_log_init:
	mov eax, [pSystemTable]
	add eax, EFI_SYSTEM_TABLE.ConOut
	mov eax, [eax]
	mov [pConOut], eax

	mov ecx, [eax + EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL.Reset]
	mov [fnOutputReset], ecx

	mov ecx, [eax + EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL.OutputString]
	mov [fnOutputStr], ecx

	mov eax, [pRuntimeServices]
	mov ecx, [eax + EFI_RUNTIMES_SERVICES.GetTime]
	mov [fnGetTime], ecx

	push 1
	push [pConOut]
	call [fnOutputReset]
	add esp, 8

	ret

	;; Function: efi_log_time
efi_log_time:
	; GetTime(EFI_TIME *time, EFI_TIME_CAPS *cap)
	push 0
	push stEfiTime
	call [fnGetTime]
	add esp, 8

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

	push szTime
	push [pConOut]
	call [fnOutputStr]
	add esp, 8

	ret

	;; Function: efi_log
	;;
	;; In:
	;;    ESI - string to print
efi_log:
	call efi_log_time

	push esi
	push [pConOut]
	call [fnOutputStr]
	add esp, 8

	; print CRLF
	push szEndLine
	push [pConOut]
	call [fnOutputStr]
	add esp, 8

	ret

section '.data' data readable writable

szTime    du '[00:00:00] ', 0
szEndLine du CR, LF, 0

;; Variable: pConOut
;; Pointer to EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL
pConOut       dd ?
fnOutputReset dd ?
fnOutputStr   dd ?

stEfiTime EFI_TIME
fnGetTime dd ?
