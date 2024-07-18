	;; File: com.inc
	;;
	;; Usefull links:
	;; - <PC16550D datasheet at https://www.scs.stanford.edu/10wi-cs140/pintos/specs/pc16550d.pdf>

COM1 = 0x3F8
COM2 = 0x2F8
COM3 = 0x3E8
COM4 = 0x2E8

UART8250_RBR = 0x0
UART8250_THR = 0x0

UART8250_IER = 0x1
UART8250_IIR = 0x2
UART8250_FCR = 0x2
UART8250_LCR = 0x3
UART8250_MCR = 0x4
UART8250_LSR = 0x5
UART8250_MSR = 0x6
UART8250_SCR = 0x7

	; DLAB = 1
UART8250_DLL = 0x0
UART8250_DLH = 0x1

com_init:
	ret

com_probe:
	ret

com_irq1:
	pusha

	popa
	iret

com_irq2:
	pusha
	popa
	iret
