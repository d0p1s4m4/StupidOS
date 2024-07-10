macro ISR_ERROR num {
	cli

	jmp isr_common
}

macro ISR_NOERROR num {
	cli

	jmp isr_common
}

isr_common:
	pusha

	mov ax, ds
	push eax

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	pop eax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	popa
	add esp, 8
	sti
	iret
