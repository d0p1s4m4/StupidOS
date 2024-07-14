	;; File: floppy.inc
	;;
	;; Usefull links:
	;; - <https://wiki.osdev.org/Floppy_Disk_Controller>
	;; - <datasheet at http://www.osdever.net/documents/82077AA_FloppyControllerDatasheet.pdf>

CMOS_FLOPPY_TYPE = 0x10

FLOPPY_NONE   = 0x0
FLOPPY_360KB  = 0x1
FLOPPY_1_2MB  = 0x2
FLOPPY_720KB  = 0x3
FLOPPY_1_44MB = 0x4
FLOPPY_2_88MB = 0x5

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

floppy_irq:
	iret

szMsgFloppy0Found db "floppy0: Found", 0
szMsgFloppy1Found db "floppy1: Found", 0
