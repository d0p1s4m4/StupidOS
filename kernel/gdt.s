[BITS 32]

section .text

global setup_gdt
setup_gdt:
	lgdt [gdt_ptr]
	mov eax, cr0
	or al, 1
	mov cr0, eax
	jmp 0x08:.flush_cs
.flush_cs:
	mov ax, 0x10 ; data segment
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	ret

section .data

gdt_ptr:
	dw gdt_entries.end - gdt_entries - 1
	dd gdt_entries

gdt_entries:
	;; null descriptor
	dd 0x0
	dd 0x0

	;; kernel mode code segment
	dw 0xFFFF ; Limit
	dw 0x0000 ; Base (low)
	db 0x00   ; Base (mid)
	db 0x9A   ; Access: 1 (P) 0 (DPL), 1 (S), 1010 (Type)
	db 0xCF   ; Granularity: 1 (G), 1 (D/B), 0 (AVL), Limit
	db 0x00   ; Base (high)

	;; kernel mode data segment
	dw 0xFFFF
	dw 0x0000
	db 0x00
	db 0x92
	db 0xCF
	db 0x00

	;; user mode code segment
	dw 0xFFFF
	dw 0x0000
	db 0x00
	db 0xFA
	db 0xCF
	db 0x00

	;; user mode data segment
	dw 0xFFFF
	dw 0x0000
	db 0x00
	db 0xF2
	db 0xCF
	db 0x00

	;; TSS
.end:
