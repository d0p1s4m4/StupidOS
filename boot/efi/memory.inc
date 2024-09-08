	;; File: memory.inc

	section '.text' code executable readable

efi_memory_init:
	mov eax, [pBootServices]
	mov ecx, [eax + EFI_BOOT_SERVICES.AllocatePool]
	mov [fnAllocatePool], ecx

	mov ecx, [eax + EFI_BOOT_SERVICES.FreePool]
	mov [fnFreePool], ecx

	mov ecx, [eax + EFI_BOOT_SERVICES.GetMemoryMap]
	mov [fnGetMemoryMap], ecx

	ret

	section '.data' data readable writeable

	;; Variable: fnAllocatePool
fnAllocatePool  dd ?
	;; Variable: fnFreePool
fnFreePool      dd ?
	;; Variable: fnGetMemoryMap
fnGetMemoryMap  dd ?
