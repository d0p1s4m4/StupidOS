	;; File: lock.inc

	;; Function: lock_acquire 
	;;
	;; In:
	;;    EAX - lock address
lock_acquire:
	mov edx, 1	
	xchg edx, [eax]
	or edx, edx
	jnz lock_acquire
	ret

	;; Function: lock_release
	;;
	;; In:
	;;    EAX - lock address
lock_release:
	xor edx, edx
	xchg [eax], edx
	ret
