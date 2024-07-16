	;; File: dev.inc

struc Device {
	.write dd ?
	.read  dd ?
	.probe dd ?
}

dev_init:
	ret
