	;; File: pic.inc
	;;  <Datasheet at https://pdos.csail.mit.edu/6.828/2005/readings/hardware/8259A.pdf>

PIC1_COMMAND = 0x20
PIC1_DATA    = 0x21
PIC2_COMMAND = 0xA0
PIC2_DATA    = 0xA1

PIC_ICW1      = 0x10
PIC_ICW1_IC4  = 0x01
PIC_ICW1_SNGL = 0x02  
PIC_ICW1_ADI  = 0x04
PIC_ICW1_LTIM = 0x08

PIC_ICW4_8086 = 0x01
PIC_ICW4_AEOI = 0x02
PIC_ICW4_MSTR = 0x04
PIC_ICW4_BUF  = 0x08
PIC_ICW4_SFNM = 0x10

PIC_EOI = 0x20


pic_init:
	; ICW 1
	mov al, PIC_ICW1 or PIC_ICW1_IC4
	out PIC1_COMMAND, al
	out PIC2_COMMAND, al

	; ICW 2
	mov al, 0x20
	out PIC1_DATA, al
	mov al, 0x28
	out PIC2_DATA, al

	; ICW 3
	mov al, 0x4
	out PIC1_DATA, al
	mov al, 0x2
	out PIC2_DATA, al

	; ICW 4
	mov al, PIC_ICW4_8086
	out PIC1_DATA, al
	out PIC2_DATA, al

	xor al, al
	out PIC1_DATA, al
	out PIC2_DATA, al

	ret

pic_disable:
	mov al, 0xFF
	out PIC1_DATA, al
	out PIC2_DATA, al
	ret
