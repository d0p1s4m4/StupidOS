	;; File: memory.inc

	section '.text' code executable readable

efi_memory_init:
	mov eax, [pEfiBootServices]
	mov ecx, [eax + EFI_BOOT_SERVICES.AllocatePool]
	mov [fnAllocatePool], ecx

	mov ecx, [eax + EFI_BOOT_SERVICES.FreePool]
	mov [fnFreePool], ecx

	mov ecx, [eax + EFI_BOOT_SERVICES.GetMemoryMap]
	mov [fnGetMemoryMap], ecx

	call efi_get_memory_map

	ret

efi_get_memory_map:
	push ebp
	mov ebp, esp

	sub esp, 4

	lea eax, [ebp - 4]
	EFI_CALL [fnGetMemoryMap], uMemMapSize, 0, eax, uDescSize, uDescVer

	add dword [uMemMapSize], 4096

	EFI_CALL [fnAllocatePool], EFI_LOADER_DATA, [uMemMapSize], pMemMap

	LOG szRes2, eax

	EFI_CALL [fnGetMemoryMap], uMemMapSize, [pMemMap], eax, uDescSize, uDescVer

	LOG szRes, eax

	leave
	ret

	section '.data' data readable writeable

	;; Variable: fnAllocatePool
fnAllocatePool  dd ?
	;; Variable: fnFreePool
fnFreePool      dd ?
	;; Variable: fnGetMemoryMap
fnGetMemoryMap  dd ?

uMemMapSize dd 0
uDescSize   dd 0
uDescVer    dd 0
pMemMap     dd 0

szRes du "Result: %x", 0
szRes2 du "Result: %x", 0
