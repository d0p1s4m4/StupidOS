
	;; 
lock_acquire:
	mov edx, 1	
	xchg edx, [eax]
	or edx, edx
	jnz lock_acquire
	ret

lock_release:
	xor edx, edx
	xchg [eax], edx
	ret
