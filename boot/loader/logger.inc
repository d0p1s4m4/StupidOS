bios_log_time:
	clc
	mov ah, 0x02
	int 0x1A

	mov al, ch
	aam
	add ah, 0x30
	add al, 0x30
	mov [time + 1], ah
	mov [time + 2], al

	mov al, cl
	aam
	add ah, 0x30
	add al, 0x30
	mov [time + 4], ah
	mov [time + 5], al

	mov al, dh
	aam
	add ah, 0x30
	add al, 0x30
	mov [time + 7], ah
	mov [time + 8], al

	mov si, time
	call bios_print

	ret

bios_log_hex:
	ret

	;; Function: bios_log
	;;
	;; Parameters:
	;;     SI      - string to print
	;;     [STACK] - variadic arguments
	;;
bios_log:
	push si
	call bios_log_time
	pop si

	call bios_print


	mov si, endline
	call bios_print
	ret

time db '[00:00.00] ', 0
endline db CR, LF, 0
