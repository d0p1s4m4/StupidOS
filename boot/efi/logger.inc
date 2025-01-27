	;; File: logger.inc

	section '.text' code executable readable

	;; Function: efi_log_init
efi_log_init:
	EFI_GET_INTERFACE eax, ConOut
	mov [pConOut], eax

	mov ecx, [eax + EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL.Reset]
	mov [fnOutputReset], ecx

	mov ecx, [eax + EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL.OutputString]
	mov [fnOutputStr], ecx

	mov eax, [pRuntimeServices]
	mov eax, [eax + EFI_RUNTIMES_SERVICES.GetTime]
	mov [fnGetTime], eax

	EFI_CALL [fnOutputReset], [pConOut], 1

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
	mov esi, eax

	call efi_log_time

	EFI_CALL [fnOutputStr], [pConOut], esi

	; print CRLF
	EFI_CALL [fnOutputStr], [pConOut], szEndLine

	pop esi

	leave
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
