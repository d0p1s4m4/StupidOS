	;; File: cmos.inc 
	;;
	;; > +--------------------------------
	;; > | 0x00 | RTC seconds
	;; > +---------------
	;; > | 0x01 | RTC seconds alarm
	;; > +--------------------
	;; > | 0x02 | RTC minutes
	;; > +------------------------


CMOS_COMMAND = 0x70
CMOS_DATA    = 0x71

CMOS_FLOPPY_TYPE = 0x10
