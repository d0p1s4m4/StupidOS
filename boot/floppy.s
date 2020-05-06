	[ORG 0x7c00]
	[BITS 16]

	jmp short entry
	nop

	;; -----------------------------------------------------------
	;; OEM & Bios Parameter Block
	;; -----------------------------------------------------------
	db "StupidOS"

	dw 512 			; bytes per sector
	db 1			; sector per cluster
	dw 1			; reserved sector
	db 2			; number of FATs
	dw 224			; root entries
	dw 2400			; total sector
	db 0xF9			; media descriptor type
	dw 7			; sectors per FAT
	dw 15			; sectors per track
	dw 2			; heads per cylinder
	dd 0			; hidden sectors
	dd 0			; total sector big
	db 0			; drive number
	db 0			; unused
	db 0x29 		; drive signature (0x29 = floppy) 
	dd 0x00000000		; serial number (ignore)
	db 'STUPID DISK'	; volume label
	db 'FAT12   '		; filesystem type
	
	;; -----------------------------------------------------------
	;; Print string using bios interrupt
	;; -----------------------------------------------------------
boot_print:
	pusha
.loop:
	lodsb
	cmp al, 0
	jz .end
	mov ah, 0x0E		; bios print
	mov bx, 0x07
	int 0x10
	jmp .loop
.end:
	popa
	ret

	;; -----------------------------------------------------------
	;; Variables
	;; -----------------------------------------------------------
	copyright db "Copyright (c) d0p1", 13, 10, 0
	err_disk_reset db "Error reseting disk..retry", 13, 10, 0

	;; -----------------------------------------------------------
	;; entry
	;; -----------------------------------------------------------
entry:
	xor ax, ax
	mov ds, ax

	mov si, copyright
	call boot_print
reset_disk:
	xor ah, ah
	xor dl, dl
	int 0x13
	jnc .reset_disk_end
	mov si, err_disk_reset
	call boot_print
	jmp reset_disk
.reset_disk_end:
	
hang:
	jmp hang
	cli
	hlt

	times 510-($-$$) db 0
	db 0x55
	db 0xAA
	
