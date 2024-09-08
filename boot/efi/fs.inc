	;; File: fs.inc

	section '.text' code executable readable

efi_fs_init:

	; OpenProtocol(EFI_HANDLE Handle, EFI_GUID *proto, VOID **Interface, EFI_HANDLE AgentHandle, UINT32 Attrs)
	push 0
	push [hImage]
	push pLoadedImage
	push aFSProtoGUID
	push [hImage]
	call [fnOpenProtocol]
	add esp, 20

	or eax, eax
	jnz .error
	ret
.error:
	mov esi, szErrTmp
	call efi_log

	ret

	section '.data' data readable writeable

szErrTmp du "Can't OpenProtocol(Simple fs)", 0
aFSProtoGUID db EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID
pLoadedImage dd 0
