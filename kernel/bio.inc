	;; File: bio.inc
	;; Buffer cache

	;; Struc: Buffer
	;; 
	;; .dev      - Device id
	;; .block    - Block number
	;; .ulock    - lock
	;; .refcount - refcount
	;; .data     - block data
struc Buffer {
	.dev      dd ?
	.block    dd ?
	.ulock    dd ?
	.refcount dd ?
	.data db 1024 dup(?)
}
DEFN Buffer

	;; Function: bio_init
bio_init:
	mov ecx, sizeof.Buffer * 30
	xor ax, ax
	mov edi, aBuffers
	rep stosb
	ret

	;; Function: bio_read
	;;
	;; In:
	;;    EAX - devid (AH major, AL minor)
	;;    ECX - block
bio_bread:
	push eax
	mov eax, uBIOLock
	call lock_acquire
	pop eax

	movzx ecx, ah
	shl ecx, 2
	add ecx, aBlockDevices
	cmp ecx, aBlockDevices.end
	jb @f

	mov esi, szErrorInvalidDevid
	call klog
	jmp .end
@@:
	mov ecx, [ecx]
	mov ecx, [ecx + BlkDev.strategy]

	call ecx

.end:
	mov eax, uBIOLock
	call lock_release
	ret

	;; Function: bio_bwrite
bio_bwrite:
	ret

	;; Function: bio_brelse
	;; Unbusy a buffer and release it to the free lists.
bio_brelse:
	mov eax, uBIOLock
	call lock_acquire

	mov eax, uBIOLock
	call lock_release
	ret

uBIOLock dd 0
szErrorInvalidDevid db "Error: invalid devid", 0
