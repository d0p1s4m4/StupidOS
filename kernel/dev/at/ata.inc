	;; File: ata.inc
	;;
	;; Usefull links:
	;; - <ATA PIO Mode at https://wiki.osdev.org/ATA_PIO_Mode>
	;; - <IDE spec at https://www.cpcwiki.eu/imgs/a/a2/IDE_SPEC.PDF>

ATA_PRIMARY_IO        = 0x1F0
ATA_PRIMARY_IO_CTRL   = 0x3F6
ATA_SECONDARY_IO      = 0x170
ATA_SECONDARY_IO_CTRL = 0x376

ATA_CHAN0_IO = 0x1F0
ATA_CHAN1_IO = 0x170
ATA_CHAN2_IO = 0x1E8
ATA_CHAN3_IO = 0x168

ATA_DATA     = 0x0
	;; Constant: ATA_ERROR
	;;
ATA_ERROR    = 0x1
ATA_ERROR_AMNF  = 0x01
ATA_ERROR_TKZNF = 0x02
ATA_ERROR_ABRT  = 0x04
ATA_ERROR_MCR   = 0x08
ATA_ERROR_IDNF  = 0x10
ATA_ERROR_MC    = 0x20
ATA_ERROR_UNC   = 0x40
ATA_ERROR_BBK   = 0x80

ATA_FEATURES = 0x1
ATA_SECCOUNT = 0x2
ATA_SECNUM   = 0x3
ATA_CYLLO    = 0x4
ATA_CYLHI    = 0x5
	;; Constant: ATA_DRVHEAD
	;; Drive/Head register
	;;
ATA_DRVHEAD  = 0x6
ATA_DRVHEAD_DRV = 0x10
ATA_DRVHEAD_LBA = 0x40

ATA_COMMAND  = 0x7

	;; Constant: ATA_STATUS
	;; 
ATA_STATUS   = 0x7
ATA_STATUS_ERR  = 0x01
ATA_STATUS_IDX  = 0x02
ATA_STATUS_CORR = 0x04
ATA_STATUS_DRQ  = 0x08
ATA_STATUS_SRV  = 0x10
ATA_STATUS_DF   = 0x20
ATA_STATUS_RDY  = 0x40
ATA_STATUS_BSY  = 0x80

	;; Constant: ATA_CTRL
	;; Device control register (Control base + 0)
ATA_CTRL = 0x0
ATA_CTRL_nIEN = 0x02
ATA_CTRL_SRST = 0x04
ATA_CTRL_HOB  = 0x80

ATA_DRVADDR = 0x1
ATA_DRVADDR_DS0 = 0x01
ATA_DRVADDR_DS1 = 0x02
ATA_DRVADDR_WTG = 0x40

ATA_CMD_RESTORE    = 0x10
ATA_CMD_DIAGNOSTIC = 0x90
ATA_CMD_IDENTIFY   = 0xA0

	;; Function: ata_wait
	;;
	;; In:
	;;    AX - IO port
	;;
ata_wait:
	mov dx, ax
	add dx, ATA_STATUS
	xor ecx, ecx
@@:
	inc ecx
	cmp ecx, 5000
	jg @f
	in al, dx
	and al, ATA_STATUS_BSY
	jnz @b
@@:
	ret

ata_select:
	ret

ata_cmd:
	ret

	;; Function: ata_probe
	;; In:
	;;    AX - IO port
	;;
ata_probe:
	push bx
	mov bx, ax
	xor ecx, ecx
.loop:
	call ata_wait

	; select drive
	mov dx, bx
	add dx, ATA_DRVHEAD
	mov al, cl
	shl al, 4
	or al, 0xa0
	out dx, al

	mov ax, bx
	push ecx
	call ata_wait
	pop ecx

	mov dx, bx
	add dx, ATA_COMMAND
	mov al, ATA_CMD_DIAGNOSTIC
	out dx, al

	mov ax, bx
	push ecx
	call ata_wait
	pop ecx

	mov dx, bx
	add dx, ATA_STATUS
	in al, dx
	and al, ATA_STATUS_ERR or ATA_STATUS_DRQ
	jnz .skip

	mov ax, bx
	push ecx
	call ata_wait
	pop ecx

	mov dx, bx
	add dx, ATA_COMMAND
	mov al, ATA_CMD_RESTORE
	out dx, al

	mov ax, bx
	push ecx
	call ata_wait
	pop ecx

	mov dx, bx
	add dx, ATA_STATUS
	in al, dx
	and al, ATA_STATUS_ERR or ATA_STATUS_DRQ
	jnz .skip

	push ecx
	cmp bx, ATA_CHAN1_IO
	jne @f
	add ecx, 2
	jmp .drive_found
@@:
	cmp bx, ATA_CHAN2_IO
	jne @f
	add ecx, 4
	jmp .drive_found
@@:
	cmp bx, ATA_CHAN3_IO
	jne @f
	add ecx, 8
	jmp .drive_found
@@:
.drive_found:

	push ecx
	mov esi, szMsgAtaFound
	call klog
	pop ecx
.skip:
	inc cl
	cmp cl, 2
	jb .loop
	pop bx
	ret

	;; Function: ata_init
ata_init:
	mov ecx, aAtaChans
@@:
	push ecx
	mov ax, word [ecx]
	call ata_probe
	pop ecx
	add ecx, 2
	cmp ecx, aAtaChans.end
	jb @b
	ret

ata_primary_irq:
	iret

ata_secondary_irq:
	iret

ata_device:
	db "ata", 0, 0, 0, 0, 0
	dd ata_init

aAtaChans:
	dw ATA_CHAN0_IO
	dw ATA_CHAN1_IO
	dw ATA_CHAN2_IO
	dw ATA_CHAN3_IO
.end:

szMsgAtaFound db "ATA: hd%u found", 0
