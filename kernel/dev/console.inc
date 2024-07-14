	;; File: console.inc
console_probe:
	ret

console_write:
	mov eax, uConsoleLock
	call lock_acquire

	mov eax, uConsoleLock
	call lock_release
	ret

console_read:
	mov eax, uConsoleLock
	call lock_acquire

	mov eax, uConsoleLock
	call lock_release
	ret

uConsoleLock dd 0
