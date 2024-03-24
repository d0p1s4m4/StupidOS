	format COFF
	use32

	public _start

	extrn main
	extrn exit

	section '.text' code

	public _start
_start:

	xor ebp, ebp
	call main

	call exit

