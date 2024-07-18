	;; File: com.inc

COM_MAJOR = 1

com_init:
	ret

com_probe:
	ret

com_irq:
	pusha

	popa
	iret

