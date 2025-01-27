	;; File: fs.inc

	section '.text' code executable readable

efi_fs_init:

	; OpenProtocol(EFI_HANDLE Handle, EFI_GUID *proto, VOID **Interface, EFI_HANDLE AgentHandle, UINT32 Attrs)
	EFI_CALL [fnOpenProtocol], [hImage], aFSProtoGUID, pLoadedImage, [hImage], 0, 2

	or eax, eax
	jnz .error
	ret
.error:
	mov eax, szErrTmp
	call efi_log

	ret

	section '.data' data readable writeable

szErrTmp du "Can't OpenProtocol(Simple fs)", 0
aFSProtoGUID db EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID
pLoadedImage dd 0
