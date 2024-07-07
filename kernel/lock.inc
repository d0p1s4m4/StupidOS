
	;; 
lock_acquire:
	lock bts [eax], 0
	jnc .end
@@:
	pause
	test [eax], 1
	jne @b

	lock bts [eax], 0
	jc @b
.end:
	ret

release:
	mov [eax], 0
