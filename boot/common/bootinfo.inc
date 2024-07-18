	;; File: bootinfo.inc

	include '../../kernel/sys/bootinfo.inc'

	;; Constant: BOOTINFO_MEMORY_LIMIT
BOOTINFO_MEMORY_LIMIT = 0xFFFFF000


	;; Function: boot_info_add_memmap
	;;
	;; In:
	;;     EAX - base
	;;     EDX - length
	;;
boot_info_add_memmap:
	cmp eax, BOOTINFO_MEMORY_LIMIT
	jbe @f
	ret
@@:
	mov ecx, BOOTINFO_MEMORY_LIMIT
	sub ecx, edx
	cmp eax, ecx
	jbe @f
	ret
@@:

	; ----------------------------------------------------------------------
	; dirty hack - TODO: clean this mess
	cmp eax, 0x100000
	jne @f
	mov [boot_structure.high_mem], edx
	add [boot_structure.high_mem], eax
@@:
	or eax, eax
	jnz @f
	mov [boot_structure.low_mem], edx
@@:
	; end hack
	; ----------------------------------------------------------------------

	push ebx
	xor ecx, ecx
.loop:
	mov ebx, [boot_structure.mmap + ecx * 8 + 4]
	or ebx, ebx
	jnz @f
	mov [boot_structure.mmap + ecx * 8], eax
	mov [boot_structure.mmap + ecx * 8 + 4], edx
	jmp .end
@@:
	inc ecx
	cmp ecx, 40
	jne .loop

.end:
	pop ebx
	ret

boot_sanitize_memory_map:
	ret

	;; Function: boot_info_print_mmap
boot_info_print_mmap:
	xor ecx, ecx
.loop:
	mov eax, [boot_structure.mmap + ecx * 8 + 4]
	or eax, eax
	jz @f
	push ecx
	push dword [boot_structure.mmap + ecx * 8 + 4]
	push dword [boot_structure.mmap + ecx * 8]
	mov si, szMsgMemMap
	call bios_log
	pop ecx
@@:
	inc ecx
	cmp ecx, 40
	jbe .loop
	ret

szMsgMemMap db 'Address: %x - Length: %x', 0
