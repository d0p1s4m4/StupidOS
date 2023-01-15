[BITS 32]
section .text
global setup_idt
setup_idt:
	lidt [idt_ptr]
	ret

section .data

idt_ptr:
	dw idt_entries.end - idt_entries - 1
	dd idt_entries

idt_entries:
	times 256 dd 0x00000000, 0x00000000
.end:
