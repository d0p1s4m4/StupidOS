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
	mov [handle], eax
	mov eax, [esp+8]
	mov [system_table], eax

	mov ebx, [eax + EFI_SYSTEM_TABLE.ConOut]

	mov eax, 1
	push eax
	push ebx
	call [ebx + EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL.Reset]
	add esp, 8

	push hello_msg
	push ebx
	call [ebx + EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL.OutputString]
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

hello_msg  du 'StupidOS EFI Bootloader', CR, LF, 0

			; Search path: / /boot /boot/efi
search_paths du '\\', 0, \
				'\\boot', 0, \
				'\\boot\\efi', 0, 0
kernel_file du 'vmstupid.sys', 0

handle       dd ?
system_table dd ? 
