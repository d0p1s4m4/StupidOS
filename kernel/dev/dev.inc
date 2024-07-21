	;; File: dev.inc

	include 'at/cmos.inc'
	include 'at/com.inc'
	include 'at/ne2k.inc'
	include 'at/pit.inc'
	include 'at/kbd.inc'
	include 'at/cga.inc'
	include 'at/floppy.inc'

struc Device {
	.name db 8 dup(?)
	.init dd ?
}
DEFN Device

struc BlkDev {
	.open     dd ?
	.strategy dd ?
	.ioctl    dd ?
	.close    dd ?
	.dump     dd ?
	.psize    dd ?
}

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
.end:

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
