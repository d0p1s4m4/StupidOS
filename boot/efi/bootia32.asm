	;; File: bootia32.asm
	format PE DLL EFI at 10000000h
	entry efimain

	include '../common/const.inc'
	include '../common/macro.inc'
	include 'uefi.inc'
	include '../../kernel/sys/bootinfo.inc'
	include 'logger.inc'
	include 'memory.inc'
	include 'fs.inc'

	section '.text' code executable readable

	;; Function: efimain
	;;
	;; In:
	;;     [ESP+4] - handle
	;;     [ESP+8] - <EFI_SYSTEM_TABLE>
	;;
	;; Out:
	;;     EAX - efi status
	;;
efimain:
	push ebp
	mov ebp, esp

	EFI_INIT [ebp + 8], [ebp + 12]

	mov eax, [esp+8]
	mov [hImage], eax
	mov eax, [esp+12]
	mov [pSystemTable], eax

	mov ebx, [eax + EFI_SYSTEM_TABLE.RuntimeServices]
	mov [pRuntimeServices], ebx

	mov ebx, [eax + EFI_SYSTEM_TABLE.BootServices]
	mov [pBootServices], ebx

	mov ecx, [ebx + EFI_BOOT_SERVICES.OpenProtocol]
	mov [fnOpenProtocol], ecx

	mov ecx, [ebx + EFI_BOOT_SERVICES.Exit]
	mov [fnExit], ecx

	call efi_memory_init
	call efi_log_init

	mov eax, szHelloMsg
	call efi_log

	call efi_fs_init

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

szHelloMsg  du 'StupidOS EFI Bootloader', 0

			; Search path: / /boot /boot/efi
aSearchPaths du '\\', 0, \
				'\\boot', 0, \
				'\\boot\\efi', 0, 0
szKernelFile du 'vmstupid.sys', 0
szConfigFile du 'stpdboot.ini', 0 

stBootInfo BootInfo

hImage       dd ?
pSystemTable dd ?

;; Variable: pBootServices
pBootServices   dd ?
fnOpenProtocol  dd ?
fnCloseProtocol dd ?
fnExit          dd ?

;; Variable: pRuntimeServices
pRuntimeServices dd ?

;; Variable: pLoadFileProtocol
;; Pointer to EFI_LOAD_FILE_PROTOCOL
pLoadFileProtocol dd ?
fnLoadFile        dd ?
