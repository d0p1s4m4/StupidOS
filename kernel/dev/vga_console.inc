struc console
{
	.buffer db 80*25 dup (?)
	.lineno db ?
	.colno db ?
}
