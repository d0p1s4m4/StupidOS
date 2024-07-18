	;; File: gdt.inc

gdt:
	; null descriptor
	dd 0
	dd 0

	; kernel code descriptor
	dw 0xFFFF, 0x0000
	db 0x00, 0x9a, 0xCF, 0x00

	; kernel data descriptor
	dw 0xFFFF, 0x0000
	db 0x00, 0x92, 0xCF, 0x00

	; user code descriptor
	dw 0xFFFF, 0x0000
	db 0x00, 0xFA, 0xCF, 0x00

	; user data descriptor
	dw 0xFFFF, 0x0000
	db 0x00, 0xF2, 0xCF, 0x00

	; Tss
.tss:
	.limit  dw ?
	.base   dw ?
	.baseh0 db ? 
	.access db ?
	.flags  db ?
	.baseh1 db ?

.end:
	
pGDT:
	dw gdt.end - gdt - 1
	dd gdt

gdt_set_tss:
	mov ecx, sizeof.TSS
	mov [gdt.limit], cx
	mov [gdt.base], ax

	shr ecx, 16
	shr eax, 16

	mov [gdt.baseh0], al
	mov [gdt.baseh1], ah
	mov al, 0x89
	mov [gdt.access], al

	and cl, 0xF
	or cl, 0x40
	mov [gdt.flags], cl
	ret

gdt_flush:
	; load kernel gdt 
	lgdt [pGDT]
	jmp 0x08:@f
@@:
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	ret

tss_flush:
	mov ax, 0x2B
	ltr ax
	ret
