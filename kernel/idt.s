; file: idt.s
;
[BITS 32]
section .text

idt_set_table:
	push ebp
	mov ebp, esp

	mov ecx, [ebp + 8]

	extern isr_list
	mov eax, [isr_list + (ecx * 4)]

	; offset (low)
	mov word [idt_entries + (ecx * 8)], ax
	; segment selector (kernel code)
	mov word [idt_entries + (ecx * 8) + 2], 0x08
	; zero (skip)
	; attr:  1 (Present) 00 (DPL) 0 1 (D: 32bits) 110
	mov byte [idt_entries + (ecx * 8) + 5], 0x8E

	; offset (high)
	shr eax, 16
	mov word [idt_entries + (ecx * 8) + 6], ax

	leave
	ret

global idt_setup
idt_setup:
%assign i 0
%rep 256
	push dword i
	call idt_set_table
	add esp, 4
%assign i i+1
%endrep

	lidt [idt_ptr]
	sti
	ret

section .data
align 8
idt_ptr:
	dw idt_entries.end-idt_entries-1
	dd idt_entries

; variable: idt_entries
align 8
idt_entries:
	times 256 dd 0x00000000, 0x00000000
	;; dw offset (low)
	;; dw segment selector
	;; db zero
	;; db attr | P | DPL | 0 D 1 1 0 |
	;; dw offset (high)
.end:
