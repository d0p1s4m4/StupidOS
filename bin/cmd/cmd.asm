	format COFF

	section '.text' code

	public main
	public _start
_start:	
main:
	int 0x2A

	section '.data' data

	INCLUDE 'builtins.inc'
