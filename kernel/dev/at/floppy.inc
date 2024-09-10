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
