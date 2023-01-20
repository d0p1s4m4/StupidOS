[BITS 32]

%include "base.inc"
%include "multiboot.inc"

section .text
global setup_pmm
setup_pmm:
	push ebp
	mov ebp, esp
	push edi
	push esi
	push ebx

	mov edi, [ebp + 8]

	mov eax, [edi]
	and eax, 0x40 ; (1 << 6)
	jz .no_mmap
	mov esi, [edi + mb_info.mmap_length]
	mov ebx, [edi + mb_info.mmap_addr]
.loop:
	mov eax, ebx
	LOG msg_mmap_entry, dword [eax + mb_mmap.addr + 4], \
						dword [eax + mb_mmap.addr], \
						dword [eax + mb_mmap.length + 4], \
						dword [eax + mb_mmap.length], \
						dword [eax + mb_mmap.type]
	mov eax, [ebx + mb_mmap.size]
	add eax, 0x4
	add ebx, eax
	sub esi, eax
	jnz .loop

	jmp .end_mem_detection

.no_mmap:
	LOG warn_no_mmap
	mov eax, [edi]
	and eax, 0x1
	jz .err

	xor eax, eax
	mov ecx, [edi + mb_info.mem_lower]
	shl ecx, 0xA ; ecx * 1KiB
	LOG msg_mem_block, eax, ecx
	mov eax, 0x100000
	mov ecx, [edi + mb_info.mem_upper]
	shl ecx, 0xA
	LOG msg_mem_block, eax, ecx

.end_mem_detection:

	xor eax, eax
	jmp .end
.err:
	mov eax, 1
.end:
	pop ebx
	pop esi
	pop ebx
	leave
	ret

section .rodata
warn_no_mmap db "[WARN] mmap flag not set", 0
msg_mmap_entry db "Memory Map Entry:", 0xA
	db 0x9, "Address: (hi): %x (lo): %x", 0xA
	db 0x9, "Length: (hi): %x (lo): %x", 0xA
	db 0x9, "Type: %x", 0
msg_mem_block db "Free Memory:", 0xA
	db 0x9, "Address: %x", 0xA
	db 0x9, "Length: %x", 0
file db __FILE__, 0
