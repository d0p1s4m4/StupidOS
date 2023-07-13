	;; File: pit.s
	;; Programmable Interval Timer (8253/8254)

PIT_CHAN_0 equ 0x40
PIT_CHAN_1 equ 0x41
PIT_CHAN_2 equ 0x42
PIT_CMD    equ 0x43

CMD_CHANNEL_1    equ (0x1 << 6)
CMD_CHANNEL_2    equ (0x2 << 6)
CMD_READ_BACK    equ (0x3 << 6)
CMD_ACCESS_LO    equ (0x1 << 4)
CMD_ACCESS_HI    equ (0x2 << 4)
CMD_ACCESS_LO_HI equ (0x3 << 4)

