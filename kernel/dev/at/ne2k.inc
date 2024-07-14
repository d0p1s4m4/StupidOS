	;; File: ne2k.inc
	;; 
	;; Usefull links:
	;; - <datasheet at https://bitsavers.org/components/national/_dataBooks/1988_National_Data_Communications_Local_Area_Networks_UARTs_Handbook.pdf>

NE2K_IOBASE = 0x300

ne2k_probe:
	ret

ne2k_irq:
	iret
