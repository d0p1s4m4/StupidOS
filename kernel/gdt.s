[BITS 32]

section .text

global setup_gdt
setup_gdt:
	lgdt [gdt_ptr]
	jmp 0x08:.flush_cs
.flush_cs:
	mov ax, 0x10
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
	dw 0x0 ; limit
	dw 0x0 ; base (low)
	db 0x0 ; base (mid)
	db 0x0 ; access
	db 0x0 ; granularity
	db 0x0 ; base (high)

	;; kernel mode code segment
	dw 0xFFFF
	dw 0x00
	db 0x00
	db 0x9A
	db 0xCF
	db 0x00

	;; kernel mode data segment
	dw 0xFFFF
	dw 0x00
	db 0x00
	db 0x92
	db 0xCF
	db 0x00

	;; user mode code segment
	dw 0xFFFF
	dw 0x00
	db 0x00
	db 0xFA
	db 0xCF
	db 0x00

	;; user mode data segment
	dw 0xFFFF
	dw 0x00
	db 0x00
	db 0xF2
	db 0xCF
	db 0x00

	;; TSS
.end:
