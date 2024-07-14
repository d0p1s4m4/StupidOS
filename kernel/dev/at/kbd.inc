	;; File: kbd.inc

kbd_getc:
	ret

kbd_irq:
	pusha

	mov esi, szMsgKbdIRQ
	call klog

	mov al, PIC_EOI
	out PIC1_COMMAND, al

	popa
	iret

szMsgKbdIRQ db "KBD: IRQ", 0
