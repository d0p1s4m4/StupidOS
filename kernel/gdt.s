; file: gdt.s
;
;
[BITS 32]

%include "cpu.inc"

section .text


	; Function: gdt_setup
	; 
	; in:
	;     none
	;
	; out:
	;     none
	;
global gdt_setup
gdt_setup:
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
	istruc gdt_entry
		at gdt_entry.limit_low, dw 0xFFFF
		at gdt_entry.base_low,  dw 0x0000
		at gdt_entry.base_mid,  db 0x00
		at gdt_entry.access,    db 0x9A
		at gdt_entry.flags,     db 0xCF
		at gdt_entry.base_high, db 0x00
	iend

	;; kernel mode data segment
	istruc gdt_entry
		at gdt_entry.limit_low, dw 0xFFFF
		at gdt_entry.base_low,  dw 0x0000
		at gdt_entry.base_mid,  db 0x00
		at gdt_entry.access,    db 0x92
		at gdt_entry.flags,     db 0xCF
		at gdt_entry.base_high, db 0x00
	iend

	;; user mode code segment
	istruc gdt_entry
		at gdt_entry.limit_low, dw 0xFFFF
		at gdt_entry.base_low,  dw 0x0000
		at gdt_entry.base_mid,  db 0x00
		at gdt_entry.access,    db 0xFA
		at gdt_entry.flags,     db 0xCF
		at gdt_entry.base_high, db 0x00
	iend

	;; user mode data segment
	istruc gdt_entry
		at gdt_entry.limit_low, dw 0xFFFF
		at gdt_entry.base_low,  dw 0x0000
		at gdt_entry.base_mid,  db 0x00
		at gdt_entry.access,    db 0xF2
		at gdt_entry.flags,     db 0xCF
		at gdt_entry.base_high, db 0x00
	iend

;;.tss:
	;; TSS
	;;istruc gdt_entry
	;;	at gdt_entry.access, db 0x89
	;;	at gdt_entry.flags, db 0x0
	;;iend
.end:
