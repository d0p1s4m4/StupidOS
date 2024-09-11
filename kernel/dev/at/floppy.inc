	;; File: floppy.inc
	;;
	;; Usefull links:
	;; - <https://wiki.osdev.org/Floppy_Disk_Controller>
	;; - <datasheet at http://www.osdever.net/documents/82077AA_FloppyControllerDatasheet.pdf>

FLOPPY_BDEV_MAJOR = 0
FLOPPY_CDEV_MAJOR = 9

FLOPPY_NONE   = 0x0
FLOPPY_360KB  = 0x1
FLOPPY_1_2MB  = 0x2
FLOPPY_720KB  = 0x3
FLOPPY_1_44MB = 0x4
FLOPPY_2_88MB = 0x5

	;; TODO: disk geometry for each floppy type
HEADS_PER_CYLINDER  = 2
SECTORS_PER_CLUSTER = 1
BYTES_PER_SECTOR    = 512
SECTORS_PER_TRACK   = 18
TOTAL_SECTORS       = 2880

FLOPPY_MAX = 2

	;; Constants: Registers
	;; 
	;; FLOPPY_STATUS_REGA         - Read-only register
	;; FLOPPY_STATUS_REGB         - Read-only register
	;; FLOPPY_DIGITAL_OUTPUT_REG  - XXX
	;; FLOPPY_TAPE_DRIVE_REG      - XXX
	;; FLOPPY_MAIN_STATUS_REG     - Read-only register
	;; FLOPPY_DATARATE_SELECT_REG - Write-only register
	;; FLOPPY_DATA_FIFO           - XXX
	;; FLOPPY_DIGITAL_INPUT_REG   - Read-only register
	;; FLOPPY_CONF_CONTROL_REG    - Write-only register
FLOPPY_STATUS_REGA         = 0x3F0
FLOPPY_STATUS_REGB         = 0x3F1
FLOPPY_DIGITAL_OUTPUT_REG  = 0x3F2
FLOPPY_TAPE_DRIVE_REG      = 0x3F3
FLOPPY_MAIN_STATUS_REG     = 0x3F4
FLOPPY_DATARATE_SELECT_REG = 0x3F4
FLOPPY_DATA_FIFO           = 0x3F5
FLOPPY_DIGITAL_INPUT_REG   = 0x3F7
FLOPPY_CONF_CONTROL_REG    = 0x3F7

	;; Constants: Floppy commands
	;;
	;; FLOPPY_READ_TRACK         - XXX
	;; FLOPPY_SPECIFY            - XXX
	;; FLOPPY_SENSE_DRIVE_STATUS - XXX
	;; FLOPPY_WRITE_DATA         - XXX
	;; FLOPPY_READ_DATA          - XXX
	;; FLOPPY_RECALIBRATE        - XXX
	;; FLOPPY_SENSE_INTERRUPT    - XXX
	;; FLOPPY_WRITE_DELETED_DATA - XXX
	;; FLOPPY_READ_ID            - XXX
	;; FLOPPY_READ_DELETED_DATA  - XXX
	;; FLOPPY_FORMAT_TRACK       - XXX
	;; FLOPPY_DUMPREG            - XXX
	;; FLOPPY_SEEK               - XXX
	;; FLOPPY_VERSION            - XXX
	;; FLOPPY_SCAN_EQUAL         - XXX
	;; FLOPPY_PERPENDICULAR_MODE - XXX
	;; FLOPPY_CONFIGURE          - XXX
	;; FLOPPY_LOCK               - XXX
	;; FLOPPY_VERIFY             - XXX
	;; FLOPPY_SCAN_LOW_OR_EQUAL  - XXX
	;; FLOPPY_SCAN_HIGH_OR_EQUAL - XXX
FLOPPY_READ_TRACK         = 2
FLOPPY_SPECIFY            = 3
FLOPPY_SENSE_DRIVE_STATUS = 4
FLOPPY_WRITE_DATA         = 5
FLOPPY_READ_DATA          = 6
FLOPPY_RECALIBRATE        = 7
FLOPPY_SENSE_INTERRUPT    = 8
FLOPPY_WRITE_DELETED_DATA = 9
FLOPPY_READ_ID            = 10
FLOPPY_READ_DELETED_DATA  = 12
FLOPPY_FORMAT_TRACK       = 13
FLOPPY_DUMPREG            = 14
FLOPPY_SEEK               = 15
FLOPPY_VERSION            = 16
FLOPPY_SCAN_EQUAL         = 17
FLOPPY_PERPENDICULAR_MODE = 18
FLOPPY_CONFIGURE          = 19
FLOPPY_LOCK               = 20
FLOPPY_VERIFY             = 22
FLOPPY_SCAN_LOW_OR_EQUAL  = 25
FLOPPY_SCAN_HIGH_OR_EQUAL = 29

	;; Constants: DOR bitflag
	;;
	;; FLOPPY_DOR_MOTD  - Set to turn drive 3's motor ON
	;; FLOPPY_DOR_MOTC  - Set to turn drive 2's motor ON
	;; FLOPPY_DOR_MOTB  - Set to turn drive 1's motor ON
	;; FLOPPY_DOR_MOTA  - Set to turn drive 0's motor ON
	;; FLOPPY_DOR_IRQ   - Set to enable IRQs and DMA
	;; FLOPPY_DOR_RESET - Reset on clear
	;; FLOPPY_DOR_DSEL  - Select drive
FLOPPY_DOR_MOTD  = 0x80
FLOPPY_DOR_MOTC  = 0x40
FLOPPY_DOR_MOTB  = 0x20
FLOPPY_DOR_MOTA  = 0x10
FLOPPY_DOR_IRQ   = 0x08
FLOPPY_DOR_RESET = 0x04
FLOPPY_DOR_DSEL  = 0x03 

	;; Constants: MSR bitflag
	;;
	;; FLOPPY_MSR_RQM  - XXX
	;; FLOPPY_MSR_DIO  - XXX
	;; FLOPPY_MSR_NDMA - XXX
	;; FLOPPY_MSR_CB   - XXX
	;; FLOPPY_MSR_ACTD - XXX
	;; FLOPPY_MSR_ACTC - XXX
	;; FLOPPY_MSR_ACTB - XXX
	;; FLOPPY_MSR_ACTA - XXX
FLOPPY_MSR_RQM  = 0x80
FLOPPY_MSR_DIO  = 0x40
FLOPPY_MSR_NDMA = 0x20
FLOPPY_MSR_CB   = 0x10
FLOPPY_MSR_ACTD = 0x08
FLOPPY_MSR_ACTC = 0x04
FLOPPY_MSR_ACTB = 0x02
FLOPPY_MSR_ACTA = 0x01

	;; Struct: Floppy
	;;
	;; .active - XXX
	;; .motor  - XXX
struc Floppy {
	.active db ?
	.motor  db ?
}
DEFN Floppy

floppy_probe:
	mov al, CMOS_FLOPPY_TYPE
	out CMOS_COMMAND, al
	in al, CMOS_DATA
	mov ah, al
	and al, 0x0F
	and ah, 0xF0
	or al, al
	jz @f
	push ax
	mov esi, szMsgFloppy1Found
	call klog
	pop ax
@@:
	or ah, ah
	jz @f
	mov esi, szMsgFloppy0Found
	call klog
@@:
	ret

floppy_init:
	call floppy_probe
	ret

	;; Function: floppy_lba_to_chs
	;; Convert LBA to CHS
	;;
	;; In:
	;;   EAX - LBA
	;;
	;; Out:
	;; 	AL - Cylinder
	;;  AH - Head
	;;  CL - Sector
floppy_lba_to_chs:
	push ebp
	mov ebp, esp
	sub esp, 1
	; CYL = LBA / (HPC * SPT)
	; HEAD = (LBA % (HPC * SPT)) / SPT
	; SECT = (LBA % (HPC * SPT)) % SPT + 1

	mov edx, eax
	shr edx, 16
	mov cx, (HEADS_PER_CYLINDER * SECTORS_PER_TRACK)
	div cx

	mov [ebp-1], al ; cyl

	mov ax, dx
	mov cl, SECTORS_PER_TRACK
	div cl

	mov cl, ah
	inc cl
	xchg al, ah
	mov al, [ebp-1]
	leave
	ret

	;; Function: floppy_strategy
	;; Do a read or write operation
	;;
	;; In:
	;;    EAX - Address of <Buffer>
floppy_strategy:
	push ebp
	mov esp, ebp
	sub esp, 4
	mov [ebp-4], eax

	push esi
	mov esi, szMsgFdStrat
	push [eax+Buffer.block]
	mov ax, [eax+Buffer.dev]
	movzx ecx, al
	push ecx
	call klog
	pop esi


	leave
	ret

	;; Function: floppy_open
	;; Open the device for I/O operations
floppy_open:
	push ebp
	mov ebp, esp

	mov eax, [ebp+4]
	cmp al, FLOPPY_MAX
	jb @f
	mov eax, ENXIO
	jmp .end
@@:
	xor eax, eax
.end:
	pop ebp
	ret

	;; Function: floppy_close
	;; Close a device.
floppy_close:
	xor eax, eax
	ret

floppy_ioctl:
	mov eax, ENODEV
	ret

floppy_dump:
	ret

floppy_psize:
	xor eax, eax
	ret

floppy_irq:
	pusha
	mov esi, szMsgFloppyIRQ
	popa
	iret

szMsgFloppyIRQ db 'Floppy: IRQ', 0

floppy_device:
	db 'floppy', 0, 0
	dd floppy_init
	db FLOPPY_BDEV_MAJOR
	db FLOPPY_CDEV_MAJOR

floppy_bdevsw:
	dd floppy_open
	dd floppy_strategy
	dd floppy_ioctl
	dd floppy_close
	dd floppy_dump
	dd floppy_psize

szMsgFloppy0Found db "floppy: fd0 found", 0
szMsgFloppy1Found db "floppy: fd1 found", 0
szMsgFdStrat      db "fd%u: strategy (blk: %u)", 0
