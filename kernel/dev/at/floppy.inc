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

FLOPPY_MAX = 2

struc Floppy {
	.active db ?
}

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

floppy_strategy:
	ret

floppy_open:

	xor eax, eax
	ret

floppy_close:
	ret

floppy_ioctl:
	mov eax, ENODEV
	ret

floppy_irq:
	iret

floppy_device:
	db 'floppy', 0, 0
	dd floppy_init
	db 0
	db 0

szMsgFloppy0Found db "floppy0: Found", 0
szMsgFloppy1Found db "floppy1: Found", 0
