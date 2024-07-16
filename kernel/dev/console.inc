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

console_init:
	ret

console_device:
	dd console_probe
	dd console_write
	dd console_write

uConsoleLock dd 0
