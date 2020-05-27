	[ORG 0x7c00]
	[BITS 16]

	jmp short entry
	nop

	;; -----------------------------------------------------------
	;; OEM & Bios Parameter Block
	;; -----------------------------------------------------------
	db "StupidOS"

bpb_bytes_per_sector	dw 512 			; bytes per sector
bpb_sectors_per_cluster	db 1			; sector per cluster
bpb_reserved_sectors	dw 1			; reserved sector
bpb_number_of_fats	db 2			; number of FATs
bpb_root_entries	dw 224			; root entries
bpb_total_sectors	dw 2880			; total sector
bpb_media		db 0xF0			; media descriptor type
bpb_sectors_per_fat	dw 9			; sectors per FAT
bpb_sectors_per_track	dw 18			; sectors per track
bpb_heads_per_cylinder	dw 2			; heads per cylinder
bpb_hidden_sectors	dd 0			; hidden sectors
bpb_total_sector_big	dd 0			; total sector big
bpb_drive_number	db 0			; drive number
bpb_unused		db 0			; unused
bpb_signature		db 0x29 		; drive signature (0x29 = floppy) 
bpb_serial_number	dd 0x00000000		; serial number (ignore)
bpb_volume_label	db 'STUPID DISK'	; volume label
bpb_filesystem		db 'FAT12   '		; filesystem type

	;; -----------------------------------------------------------
	;; entry
	;; -----------------------------------------------------------
entry:
	xor ax, ax
	mov ds, ax

	mov si, msg_boot
	call boot_print
	mov si, msg_copyright
	call boot_print
reset_disk:
	xor ah, ah
	xor dl, dl
	int 0x13
	jc reset_disk

	;; load kernel from FAT
	;; store size of root dir in cx
	xor cx, cx
	xor dx, dx
	mov ax, 0x0020
	mul word [bpb_root_entries]
	div word [bpb_bytes_per_sector]
	xchg ax, cx

	;; store loc of root directory in ax
	mov al, byte [bpb_number_of_fats]
	mul word [bpb_sectors_per_fat]
	add ax, word [bpb_reserved_sectors]
	;; mov word [datasec], ax
	;; mov word [datasec], cx
	
	mov bx, 0x0200
	
hang:
	jmp hang
	cli
	hlt

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

fatal_error:
	pusha
	mov cx, si
	mov si, msg_error
	call boot_print
	mov si, cx
	call boot_print
	mov si, msg_press_any_key
	call boot_print
	xor ax, ax
	int 0x16		; wait keypressed 
	int 0x19		; reboot
	popa
	ret

disk_read_sector:
	pusha
	
	popa
	ret
	
	;; -----------------------------------------------------------
	;; Variables
	;; -----------------------------------------------------------
	msg_boot db "StupidOS - Bootloader", 13, 10, 0
	msg_copyright db "Copyright (c) d0p1", 13, 10, 0
	msg_press_any_key db "Press any key...", 13, 10, 0

	msg_error db "[ERROR] ", 0
	err_kern_not_found db "KERNEL.BIN not found...", 13, 10, 0

	kernel_filename db "KERNEL  BIN"

	;; -----------------------------------------------------------
	;; End
	;; -----------------------------------------------------------
	times 510-($-$$) db 0
	db 0x55
	db 0xAA
	
