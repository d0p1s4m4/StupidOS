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
UART8250_MCR_OUT2 = 0x08
UART8250_LSR = 0x5
UART8250_LSR_THRE = 0x20
UART8250_MSR = 0x6
UART8250_SCR = 0x7

	; DLAB = 1
UART8250_DLL = 0x0
UART8250_DLH = 0x1

com_init:
	push ebx
	xor cx, cx
.loop:
	inc ch

	; get io address
	movzx eax, cl
	shl eax, 1
	add eax, aComPorts
	mov bx, [eax]

	mov ax, bx
	push ecx
	call com_probe
	pop ecx
	or eax, eax
	jnz @f
	; comX found yeah
	movzx eax, ch
	push ecx
	push eax
	mov esi, szMsgComFound
	call klog
	pop ecx

	; mark comX as active
	mov al, 1
	shl al, cl
	or byte [uComActive], al
@@:
	inc cl
	cmp cl, NCOM
	jb .loop

	pop ebx
	ret

	;; Function: com_probe
	;;
	;; In:
	;;    AX - IO port
com_probe:
	mov dx, ax
	add dx, UART8250_LCR
	xor al, al
	out dx, al
	dec dx
	out dx, al
	xor ecx, ecx

	;; wait a little
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

	;; Function: com_putc
	;; 
	;; In:
	;;    AL - Char to print
	;;    DX - IO port
	;;
com_putc:
	add dx, UART8250_LSR
	push eax
@@:
	in al, dx
	and al, UART8250_LSR_THRE
	jnz @b
	pop eax
	sub dx, UART8250_LSR
	out dx, al
	ret
	ret

com_irq1:
	pusha
	mov esi, szMsgComIRQ
	call klog
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

szMsgComFound db "COM: com%u found", 0
szMsgComIRQ db "com irq", 0

aComPorts:
	dw COM1
	dw COM2
	dw COM3
	dw COM4
.end:

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
