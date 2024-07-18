	;; File: cmos.inc 
	;;
	;; > +------+-------------------------+
	;; > | 0x00 | RTC seconds             |
	;; > +------+-------------------------+
	;; > | 0x01 | RTC seconds alarm       |
	;; > +------+-------------------------+
	;; > | 0x02 | RTC minutes             |
	;; > +------+-------------------------+
	;; > | 0x03 | RTC minutes alarm       |
	;; > +------+-------------------------+
	;; > | 0x04 | RTC hours               |
	;; > +------+-------------------------+
	;; > | 0x05 | RTC hours alarm         |
	;; > +------+-------------------------+
	;; > | 0x06 | RTC day of week         |
	;; > +------+-------------------------+
	;; > | 0x07 | RTC date day            |
	;; > +------+-------------------------+
	;; > | 0x08 | RTC date month          |
	;; > +------+-------------------------+
	;; > | 0x09 | RTC date year           |
	;; > +------+-------------------------+
	;; > | 0x0A | Status register A       |
	;; > +------+-------------------------+
	;; > | 0x0B | Status register B       |
	;; > +------+-------------------------+
	;; > | 0x0C | Status register C       |
	;; > +------+-------------------------+
	;; > | 0x0D | Status register D       |
	;; > +------+-------------------------+
	;; > | 0x0E | Diagnostic Status       |
	;; > +------+-------------------------+
	;; > | 0x0F | CMOS Shutdown Status    |
	;; > +------+-------------------------+
	;; > | 0x10 | Floppy Disk Drive Type  |
	;; > +------+-------------------------+
	;; > | 0x11 | System Conf Settings    |
	;; > +------+-------------------------+
	;; > | 0x12 | Hard Disk Type          |
	;; > +------+-------------------------+
	;; > | 0x13 | Typematic Parameters    |
	;; > +------+-------------------------+
	;; > | 0x14 | Installed Equipment     |
	;; > +------+-------------------------+
	;; > | 0x15 | Base Memory LO          |
	;; > +------+-------------------------+
	;; > | 0x16 | Base Memory HI          |
	;; > +------+-------------------------+
	;; > | 0x17 | Extended Memory LO      |
	;; > +------+-------------------------+
	;; > | 0x18 | Extended Memory HI      |
	;; > +------+-------------------------+
	;; > | 0x19 | Hard Disk Extended Type |
	;; > +------+-------------------------+

CMOS_COMMAND = 0x70
CMOS_DATA    = 0x71

CMOS_REG_SECOND = 0x00
CMOS_REG_MINUTE = 0x02
CMOS_REG_HOUR   = 0x04

CMOS_FLOPPY_TYPE = 0x10

CMOS_INSTALLED_EQUIPMENT = 0x14