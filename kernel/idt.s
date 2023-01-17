[BITS 32]
section .text
global setup_idt
setup_idt:
%assign i 0
extern isr %+ i
	mov eax, isr %+ i
	mov [idt_entries + i * 4], ax
	shr eax, 16
	mov word [idt_entries + i * 4 + 2], 0x8
	mov byte [idt_entries + i * 4 + 5], 0x8E
	mov [idt_entries + i * 4 + 6], ax

%assign i i+1
%rep 32
%endrep
	lidt [idt_ptr]
	sti
	ret

section .data

idt_ptr:
	dw 256 * 8
	dd idt_entries

idt_entries:
	times 256 dd 0x00000000, 0x00000000
	;; dw isr_low
	;; dw kernel cs
	;; db zero
	;; db attr
	;; dw isr_high
.end:
