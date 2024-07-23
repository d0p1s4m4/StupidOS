	;; File: bio.inc
	;; Buffer cache

	;; Struc: Buffer
struc Buffer {
	.dev      dd ?
	.block    dd ?
	.ulock    dd ?
	.refcount dd ?
	.data db 512 dup(?)
}
DEFN Buffer

	;; Function: bio_init
bio_init:
	mov ecx, sizeof.Buffer * 30
	xor ax, ax
	mov edi, aBuffers
	rep stosb
	ret

bio_get:
	mov eax, uBIOLock
	call lock_acquire

	mov eax, uBIOLock
	call lock_release
	ret

bio_brelse:
	ret

uBIOLock dd 0
