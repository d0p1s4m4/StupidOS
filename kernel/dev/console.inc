	;; File: console.inc

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

console_init:
	ret

console_device:
	db 'console', 0
	dd console_init
	dd console_write
	dd console_write

console_cdevws:
	dd 0
	dd 0

uConsoleLock dd 0
