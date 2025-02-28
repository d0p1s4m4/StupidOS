	format COFF

	section '.text' code
	extrn lulz
	public main
	public _start
_start:	
	jmp main
main:
	call dummy_long_str
	int 0x2A

	call lulz

	ret

	public dummy_long_str
dummy_long_str:
	xor eax, eax
	mov esi, zz
	int 0x41
	ret

	section '.data' data

	INCLUDE 'builtins.inc'
