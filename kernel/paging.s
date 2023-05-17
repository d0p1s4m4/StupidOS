[BITS 32]

PE_PRESENT equ 1 << 0
PE_WRITABLE equ 1 << 1
PE_USERMODE equ 1 << 2
PE_ACCESSED equ 1 << 5
PE_DIRTY equ 1 << 6

section .data
align 4096

page_directory:
	times 1024 dd 0x00000000

section .text

	; Function: setup_paging
	; 
	; in:
	;     none
	;
	; out:
	;     none
	;
global setup_paging
setup_paging:
	mov eax, page_directory
	mov cr3, eax

	mov eax, cr0
	or eax, 1 << 31				; enable paging
	mov cr0, eax

	ret
