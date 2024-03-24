	format PE DLL EFI at 10000000h
	entry efimain

	section '.text' code executable readable

	include '../common/const.inc'
	include '../common/macro.inc'
	include 'uefi.inc'

	; ESP     => return address
	; ESP + 4 => Handle
	; ESP + 8 => SysTable
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

	jmp $

	xor eax, eax
	ret

	section '.reloc' fixups data discardable

	section '.data' data readable writeable

hello_msg   du 'StupidOS EFI Bootloader', 13, 10, 0
kernel_file du 'vmstupid.sys', 0

handle       dd ?
system_table dd ? 
