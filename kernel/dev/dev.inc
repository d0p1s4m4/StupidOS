	;; File: dev.inc

	include 'at/cmos.inc'
	include 'at/com.inc'
	include 'at/ne2k.inc'
	include 'at/pit.inc'
	include 'at/kbd.inc'
	include 'at/cga.inc'
	include 'at/ata.inc'
	include 'at/floppy.inc'

struc Device {
	.name db 8 dup(?)
	.init dd ?
}
DEFN Device

	;; Struc: BlkDev
	;;
	;; .open     - Open the device in preparation for I/O operations
	;; .strategy - Do a read or write operation.
	;; .close    - Close a device.
	;; .dump     - Write all physical memory to the device.
	;; .psize    - Return the size of a disk-drive partition.
	;;
struc BlkDev {
	.open     dd ?
	.strategy dd ?
	.ioctl    dd ?
	.close    dd ?
	.dump     dd ?
	.psize    dd ?
}
DEFN BlkDev

	;; Struc: CharDev
	;;
	;; .open   - open the device.
	;; .close  - close the device..
	;; .read   - do an input operation.
	;; .write  - do an output operation.
	;; .ioctl  - do an I/O control operation.
	;; .select - poll device for I/O readiness.
	;; .stop   - stop output on the device.
	;; .mmap   - map device offset to memory location
	;; .reset  - reinitialize device after a bus reset.
	;;
struc CharDev {
	.open   dd ?
	.close  dd ?
	.read   dd ?
	.write  dd ?
	.ioctl  dd ?
	.select dd ?
	.stop   dd ?
	.mmap   dd ?
	.reset  dd ?
}

aBlockDevices:
	dd floppy_bdevsw
	dd ata_bdevsw
.end:

aCharDevices:
	dd console_cdevws
	dd com_cdevsw
.end:

aDevices:
	dd floppy_device
	dd kbd_device
	dd console_device
	dd com_device
	dd ne2k_device
	dd ata_device
.end:

	;; Function: dev_init
dev_init:
	mov ecx, aDevices
@@:
	mov eax, [ecx]
	mov eax, [eax + Device.init]
	push ecx
	call eax
	pop ecx
	add ecx, 4
	cmp ecx, aDevices.end
	jb @b

	ret
