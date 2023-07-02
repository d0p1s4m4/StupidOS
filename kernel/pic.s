; file: pic.s
; <Datasheet at https://pdos.csail.mit.edu/6.828/2005/readings/hardware/8259A.pdf>
[BITS 32]

PIC1_INT_START equ 0x20
PIC2_INT_START equ 0x28
PIC_INT_END    equ PIC2_INT_START + 7

PIC1_CMD	equ 0x20
PIC1_DATA	equ 0x21
PIC2_CMD	equ 0xA0
PIC2_DATA	equ 0xA1

; Initialization Command Words (ICWs)
ICW1_INIT      equ 1 << 4
ICW1_ICW4      equ 1 << 0
ICW1_SINGLE    equ 1 << 1
ICW1_INTERVAL4 equ 1 << 2
ICW1_LVL_MODE  equ 1 << 3

ICW4_8086_MODE  equ 1 << 0
ICW4_AUTO_EOI   equ 1 << 1
ICW4_BUF_SLAVE  equ 1 << 3
ICW4_BUF_MASTER equ 1 << 3 | 1 << 2
ICW4_SFNM       equ 1 << 4

; Operation Command Words (OWCs)
OCW2_EOI equ 1 << 5

section .text

	; Function: setup_pic
	; 
	; in:
	;     none
	;
	; out:
	;     none
	;
global pic_setup
pic_setup:
	push ebp
	mov ebp, esp
	sub esp, 2

	in al, PIC1_DATA
	mov [ebp - 2], al
	in al, PIC2_DATA
	mov [ebp - 1], al

	mov al, ICW1_INIT | ICW1_ICW4
	out PIC1_CMD, al
	out PIC2_CMD, al
	
	mov al, PIC1_INT_START
	out PIC1_DATA, al
	mov al, PIC2_INT_START
	out PIC2_DATA, al

	mov al, 4
	out PIC1_DATA, al
	mov al, 2
	out PIC2_DATA, al

	mov al, ICW4_8086_MODE
	out PIC1_DATA, al
	out PIC2_DATA, al

	mov al, [ebp - 2]
	out PIC1_DATA, al
	mov al, [ebp - 1]
	out PIC2_DATA, al

	leave
	ret

	; Function: pic_eoi
	; 
	; in:
	;     none
	;
	; out:
	;     none
	;
global pic_eoi
pic_eoi:
	push ebp
	mov ebp, esp

	mov al, OCW2_EOI

	cmp byte [ebp + 8], PIC1_INT_START
	jb .end
	cmp byte [ebp + 8], PIC_INT_END
	ja .end

	cmp byte [ebp + 8], PIC2_INT_START
	jb .pic1
	out PIC2_CMD, al
	jmp .end
.pic1:
	out PIC1_CMD, al

.end:
	leave
	ret

	; Function: pic_disable
global pic_disable
pic_disable:
	mov al, 0xFF
	out PIC1_DATA, al
	out PIC2_DATA, al
	ret
