	;; File: ne2k.inc
	;; 
	;; Usefull links:
	;; - <datasheet at https://web.archive.org/web/20010612150713/http://www.national.com/ds/DP/DP8390D.pdf>

NE2K_IOBASE = 0x300
NE2K_ASIC   = NE2K_IOBASE + 0x10
NE2K_NIC    = NE2K_IOBASE + 0x00

NE2K_RESET = 0xF

; PAGE 0
DP8390D_CR    = 0x0

DP8390D_CR_PS1 = 0x80
DP8390D_CR_PS0 = 0x40
DP8390D_CR_RD2 = 0x20
DP8390D_CR_RD1 = 0x10
DP8390D_CR_RD0 = 0x08
DP8390D_CR_TXP = 0x04
DP8390D_CR_STA = 0x02
DP8390D_CR_STP = 0x01

DP8390D_CLDA0 = 0x1
DP8390D_CLDA1 = 0x2
DP8390D_BNRY  = 0x3
DP8390D_TSR   = 0x4
DP8390D_NCR   = 0x5
DP8390D_FIFO  = 0x6
DP8390D_ISR   = 0x7
DP8390D_CRDA0 = 0x8
DP8390D_CRDA1 = 0x9
DP8390D_RSR   = 0xC
DP8390D_CNTR0 = 0xD
DP8390D_CNTR1 = 0xE
DP8390D_CNTR2 = 0xF

DP8390D_PSTART = 0x1
DP8390D_PSTOP  = 0x2
DP8390D_TPSR   = 0x4
DP8390D_TBCR0  = 0x5
DP8390D_TBCR1  = 0x6
DP8390D_RSAR0  = 0x8
DP8390D_RSAR1  = 0x9
DP8390D_RBCR0  = 0xA
DP8390D_RBCR1  = 0xB
DP8390D_RCR    = 0xC
DP8390D_TCR    = 0xD
DP8390D_DCR    = 0xE
DP8390D_IMR    = 0xF

; PAGE 1
DP8390D_PAR0 = 0x1
DP8390D_PAR1 = 0x2
DP8390D_PAR2 = 0x3
DP8390D_PAR3 = 0x4
DP8390D_PAR4 = 0x5
DP8390D_PAR5 = 0x6
DP8390D_CURR = 0x7
DP8390D_MAR0 = 0x8
DP8390D_MAR1 = 0x9
DP8390D_MAR2 = 0xA
DP8390D_MAR3 = 0xB
DP8390D_MAR4 = 0xC
DP8390D_MAR5 = 0xD
DP8390D_MAR6 = 0xE
DP8390D_MAR7 = 0xF

ne2k_init:
	call ne2k_probe
	or eax, eax
	jnz @f
	mov esi, szMsgNe2kfound
	call klog
@@:
	ret


ne2k_probe:
	mov dx, NE2K_ASIC + NE2K_RESET
	in al, dx
	out dx, al

	mov ecx, 5000
@@:
	dec ecx
	or ecx, ecx
	jnz @b

	mov dx, NE2K_NIC + DP8390D_CR
	mov al, DP8390D_CR_RD2 or DP8390D_CR_STP
	out dx, al
	
	mov ecx, 5000
@@:
	dec ecx
	or ecx, ecx
	jnz @b

	mov dx, NE2K_NIC + DP8390D_CR
	in al, dx
	and al, (DP8390D_CR_RD2 or DP8390D_CR_TXP or DP8390D_CR_STA or DP8390D_CR_STP)
	cmp al, (DP8390D_CR_RD2 or DP8390D_CR_STP)
	jne @f

	xor eax, eax
@@:
	ret

ne2k_irq:
	pusha
	mov esi, szMsgNe2kIRQ
	call klog
	popa
	iret

ne2k_device:
	db 'ne2k', 0, 0, 0, 0
	dd ne2k_init

szMsgNe2kfound db "NE2K: found", 0
szMsgNe2kIRQ db "NE2K: IRQ", 0
