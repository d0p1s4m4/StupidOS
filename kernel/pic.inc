	;; File: pic.inc
	;;  <Datasheet at https://pdos.csail.mit.edu/6.828/2005/readings/hardware/8259A.pdf>

PIC1_COMMAND = 0x20
PIC1_DATA    = 0x21
PIC2_COMMAND = 0xA0
PIC2_DATA    = 0xA1

PIC_EOI = 0x20

pic_init:
	ret

