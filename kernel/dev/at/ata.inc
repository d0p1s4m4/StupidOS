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

	;; Constant:  ATA_DATA
	;; Read/Write data
ATA_DATA     = 0x0

	;; Constant: ATA_ERROR
	;;
	;; > ┌────┬─────┬────┬───┬────┬──┬───┬───┐
    ;; > │AMNF│TKZNF│ABRT│MCR│IDNF│MC│UNC│BBK│
    ;; > └────┴─────┴────┴───┴────┴──┴───┴───┘
    ;; >      7     6    5   4    3  2   1   0
	;;
	;; AMNF  - Address mark not found.
	;; TKZNF - Track zero not found.
	;; ABRT  - Aborted command.
	;; MCR   - Media change request.
	;; IDNF  - ID not found.
	;; MC    - Media changed
	;; UNC   - Uncorrectable data error.
	;; BBK   - Bad Block detected.
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

	;; Constant: ATA_FEATURES
ATA_FEATURES = 0x1

	;; Constant: ATA_SECCOUNT
ATA_SECCOUNT = 0x2

	;; Constant: ATA_SECNUM
ATA_SECNUM   = 0x3

	;; Constant: ATA_CYLLO
ATA_CYLLO    = 0x4

	;; Constant: ATA_CYLHI
ATA_CYLHI    = 0x5

	;; Constant: ATA_DRVHEAD
	;; Drive/Head register
	;;
ATA_DRVHEAD  = 0x6
ATA_DRVHEAD_DRV = 0x10
ATA_DRVHEAD_LBA = 0x40

	;; Constant: ATA_COMMAND
ATA_COMMAND  = 0x7

	;; Constant: ATA_CMD_RESTORE
	;; Recalibrate
ATA_CMD_RESTORE    = 0x10
	;; Constant: ATA_CMD_DIAGNOSTIC
	;; Execute device diagnostic
ATA_CMD_DIAGNOSTIC = 0x90
	;; Constant: ATA_CMD_IDENTIFY
ATA_CMD_IDENTIFY   = 0xA0

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

	;; Constant: ATA_DRVADDR
	;; 
ATA_DRVADDR = 0x1
ATA_DRVADDR_DS0 = 0x01
ATA_DRVADDR_DS1 = 0x02
ATA_DRVADDR_WTG = 0x40


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

	;; Function: ata_select
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
	push ecx
	call ata_wait
	pop ecx

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

ata_bdevsw:
	dd 0

szMsgAtaFound db "ATA: hd%u found", 0
