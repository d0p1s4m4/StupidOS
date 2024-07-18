	;; File: idt.inc

idt_set_gate:
	mov [aInterruptGate + (ecx * 8)], ax
	mov [aInterruptGate + (ecx * 8) + 2], word 0x08

	mov [aInterruptGate + (ecx * 8) + 5], word 0x8E
	shr eax, 16
	mov [aInterruptGate + (ecx * 8) + 6], ax
	ret

idt_setup:
	xor ecx, ecx
@@:
	mov eax, [aISRTable + (ecx * 4)]
	call idt_set_gate
	inc ecx
	cmp ecx, 32
	jb @b

	mov eax, pit_irq
	call idt_set_gate
	inc ecx

	mov eax, kbd_irq
	call idt_set_gate
	inc ecx

@@:
	mov eax, irq_dummy
	call idt_set_gate
	inc ecx
	cmp ecx, 0x30
	jb @b

	mov ecx, 35
	mov eax, com_irq2
	call idt_set_gate

	mov ecx, 36
	mov eax, com_irq1
	call idt_set_gate

	mov ecx, 38
	mov eax, floppy_irq
	call idt_set_gate

	mov ecx, 41
	mov eax, ne2k_irq
	call idt_set_gate

	mov ecx, 0x42
	mov eax, isr_syscall
	call idt_set_gate

	lidt [pIDT]
	sti
	ret

pIDT:
	dw aInterruptGate.end-aInterruptGate-1
	dd aInterruptGate

; variable: idt_entries
aInterruptGate:
	dd 256*2 dup(0)
.end:
