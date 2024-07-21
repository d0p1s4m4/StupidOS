	;; File: com.inc
	;;
	;; Usefull links:
	;; - <PC16550D datasheet at https://www.scs.stanford.edu/10wi-cs140/pintos/specs/pc16550d.pdf>

COM1 = 0x3F8
COM2 = 0x2F8
COM3 = 0x3E8
COM4 = 0x2E8

NCOM = 4

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
	mov ax, COM1
	call com_probe
	or eax, eax
	jnz @f
	mov esi, szMsgCom1Found
	call klog
@@:
	mov ax, COM2
	call com_probe
	or eax, eax
	jnz @f
	mov esi, szMsgCom2Found
	call klog
@@:
	mov ax, COM3
	call com_probe
	or eax, eax
	jnz @f
	mov esi, szMsgCom3Found
	call klog
@@:
	mov ax, COM4
	call com_probe
	or eax, eax
	jnz @f
	mov esi, szMsgCom4Found
	call klog
@@:
	ret

	;; Function: com_probe
com_probe:
	mov dx, ax
	add dx, UART8250_LCR
	xor al, al
	out dx, al
	dec dx
	out dx, al
	xor ecx, ecx
@@:
	inc ecx
	cmp ecx, 100
	jbe @b

	in al, dx
	and al, 0x38
	or al, al
	jz @f
	mov eax, 1
	ret
@@:
	xor eax, eax
	ret

com_irq1:
	pusha

	popa
	iret

com_irq2:
	pusha
	popa
	iret

com_open:
	ret 

com_close:
	ret

com_read:
	ret

com_write:
	ret

com_ioctl:
	ret

com_select:
	ret

com_stop:
	ret

com_mmap:
	ret

com_reset:
	ret

uCom1Lock dd 0
uCom2Lock dd 0
uComActive db 0
szMsgCom1Found db "COM: com1 found", 0
szMsgCom2Found db "COM: com2 found", 0
szMsgCom3Found db "COM: com3 found", 0
szMsgCom4Found db "COM: com4 found", 0

com_device:
	db 'com', 0, 0, 0, 0, 0
	dd com_init

com_cdevsw:
	dd com_open
	dd com_close
	dd com_read 
	dd com_write
	dd com_ioctl
	dd com_select
	dd com_stop
	dd com_mmap
	dd com_reset
