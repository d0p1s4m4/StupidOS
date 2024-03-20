	format COFF

	section '.text' code

	public main
main:
	int 0x2A

	section '.data' data

	INCLUDE 'builtins.inc'
