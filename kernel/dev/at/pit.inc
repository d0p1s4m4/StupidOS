	;; File: pit.inc
	;;
	;; Usefull links:
	;; - <http://www.osdever.net/bkerndev/Docs/pit.htm>

PIT_CHANNEL0 = 0x40
PIT_CHANNEL1 = 0x40
PIT_CHANNEL2 = 0x42
PIT_COMMAND  = 0x43


PIT_MAX_FREQ = 1193180

	;; Function: pit_init
	;; 
	;; In:
	;;    AX - freq
pit_init:
	movzx eax, ax
	xor edx, edx
	mov ecx, PIT_MAX_FREQ
	div ecx

	push eax
	mov dx, PIT_COMMAND
	mov al, 0x34 ;channel 0, lobyte/hibyte, rate generator
	pop eax

	mov dx, PIT_CHANNEL0
	out dx, al

	xchg al, ah
	out dx, al

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
