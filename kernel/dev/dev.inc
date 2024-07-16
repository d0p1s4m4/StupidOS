	;; File: dev.inc

struc Device {
	.probe dd ?
	.write dd ?
	.read  dd ?
}
DEFN Device

aDevices:
	dd console_device
	dd floppy_device
.end:

dev_init:
	mov ecx, aDevices
@@:
	mov eax, [ecx]
	mov eax, [eax + Device.probe]
	push ecx
	call eax
	pop ecx
	add ecx, 4
	cmp ecx, aDevices.end
	jb @b

	ret
