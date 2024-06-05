	;; File: bootia32.asm
	format PE DLL EFI at 10000000h
	entry efimain

	section '.text' code executable readable

	include '../common/const.inc'
	include '../common/macro.inc'
	include 'uefi.inc'

	;; Function: efimain
	;;
	;; Parameters:
	;;
	;;     [esp+4] - handle
	;;     [esp+8] - <EFI_SYSTEM_TABLE>
	;;
	;; Returns:
	;;
	;;     eax - efi status
	;;
efimain:
	mov eax, [esp+4]
	mov [hImage], eax
	mov eax, [esp+8]
	mov [pSystemTable], eax

	mov ebx, [eax + EFI_SYSTEM_TABLE.RuntimeServices]
	mov [pRuntimeServices], ebx

	mov ebx, [eax + EFI_SYSTEM_TABLE.BootServices]
	mov [pBootServices], ebx

	;mov ecx, [ebx + EFI_BOOT_SERVICES.OpenProtocol]
	;mov [fnOpenProtocol], ecx

	mov ebx, [eax + EFI_SYSTEM_TABLE.ConOut]
	mov [pConOut], ebx

	mov ecx, [ebx + EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL.Reset]
	mov [fnOutputReset], ecx
	mov ecx, [ebx + EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL.OutputString]
	mov [fnOutputStr], ecx

	mov eax, 1
	push eax
	push [pConOut]
	call [fnOutputReset]
	add esp, 8

	push szHelloMsg
	push [pConOut]
	call [fnOutputStr]
	add esp, 8

	; load config
	
	; #=======================#
	; search and load kernel
	; openVolume()
	; for path in search_path
	;   if (open(path + file) == ok)
	;     break
	; if not found
	;    error

	; get memory map

	; paging

	; jump to kernel

	jmp $

	xor eax, eax
	ret

	section '.reloc' fixups data discardable

	section '.data' data readable writeable

szHelloMsg  du 'StupidOS EFI Bootloader', CR, LF, 0

			; Search path: / /boot /boot/efi
aSearchPaths du '\\', 0, \
				'\\boot', 0, \
				'\\boot\\efi', 0, 0
szKernelFile du 'vmstupid.sys', 0
szConfigFile du 'stpdboot.cfg', 0 

hImage       dd ?
pSystemTable dd ?

;; Variable: pBootServices
pBootServices   dd ?
fnAllocatePages dd ?
fnFreePages     dd ?
fnGetMemoryMap  dd ?
fnOpenProtocol  dd ?
fnCloseProtocol dd ?

;; Variable: pRuntimeServices
pRuntimeServices dd ?


;; Variable: pConOut
;; Pointer to EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL
pConOut       dd ?
fnOutputReset dd ?
fnOutputStr   dd ?

;; Variable: pLoadFileProtocol
;; Pointer to EFI_LOAD_FILE_PROTOCOL
pLoadFileProtocol dd ?
fnLoadFile        dd ?
