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
	.dev      dw ?
	.flags    db ?
	.bcount   db ?
	.block    dd ?
	.ulock    dd ?
	.refcount dd ?
	.data db 1024 dup(?)
}
DEFN Buffer

	;; Constants: Buffer flags
	;;
	;; BUFFER_WRITE - XXX
	;; BUFFER_READ  - XXX
	;; BUFFER_ERROR - XXX
	;; BUFFER_DIRTY - XXX
	;; BUFFER_INVAL - mark buffer as invalid
BUFFER_WRITE = 0x0
BUFFER_READ  = 0x1
BUFFER_ERROR = 0x2
BUFFER_DIRTY = 0x4
BUFFER_INVAL = 0x8

	;; Function: bio_init
bio_init:
	mov ecx, sizeof.Buffer * 30
	xor ax, ax
	mov edi, aBuffers
	rep stosb
	ret

	;; Function: bio_getblk
	;;
bio_getblk:
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
	xchg bx, bx
	mov eax, aBuffers
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
