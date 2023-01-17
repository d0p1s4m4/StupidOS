[BITS 32]
section .text
global setup_idt
setup_idt:
%assign i 0
%rep 32
extern isr %+ i
	mov eax, isr %+ i
	; offset (low)
	mov word [idt_entries + (i * 8)], ax
	; segment selector (kernel code)
	mov word [idt_entries + (i * 8) + 2], 0x08

	; zero (skip)
	; attr:  1 (Present) 00 (DPL) 0 1 (D: 32bits) 110
	mov byte [idt_entries + (i * 8) + 5], 0x8E

	; offset (high)
	shr eax, 16
	mov word [idt_entries + (i * 8) + 6], ax
%assign i i+1
%endrep

	lidt [idt_ptr]
	sti
	ret

section .data
align 8
idt_ptr:
	dw 255 * 8
	dd idt_entries

align 8
idt_entries:
	times 256 dd 0x00000000, 0x00000000
	;; dw offset (low)
	;; dw segment selector
	;; db zero
	;; db attr | P | DPL | 0 D 1 1 0 |
	;; dw offset (high)
.end:
