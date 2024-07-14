	;; File: pit.inc
	;;
	;; Usefull links:
	;; - <http://www.osdever.net/bkerndev/Docs/pit.htm>

PIT_CHANNEL0 = 0x40
PIT_CHANNEL1 = 0x40
PIT_CHANNEL2 = 0x42
PIT_COMMAND  = 0x43

	;; Function: pit_init
pit_init:
	ret

	;; Function: pit_irq
pit_irq:
	pusha

	;mov esi, szMsgPitIRQ
	;call klog

	mov al, PIC_EOI
	out PIC1_COMMAND, al

	popa 

	iret

szMsgPitIRQ db "PIT: IRQ", 0
