[BITS 32]

section .data
align 4096

page_directory:
	times 1024 dd 0x00000000

section .text

global setup_paging
setup_paging:
	mov eax, page_directory
	mov cr3, eax

	mov eax, cr0
	or eax, 0x80000001
	mov cr0, eax

	ret
