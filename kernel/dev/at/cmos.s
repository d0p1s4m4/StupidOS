	;; File: cmos.s
	;; Real/Time Clock/CMOS RAM
	;;
	;; > +-------------+------------------------------+
	;; > | 0x00 - 0x0D | *Real-time clock information |
	;; > +-------------+------------------------------+
	;; > | 0x0E        | *Diagnostic status byte      |
	;; > +-------------+------------------------------+
	;; > | 0x0F        | *Shutdown status byte        |
	;; > +-------------+------------------------------+
	;; > | 0x10        | Diskette drive type byte     |
	;; > +-------------+------------------------------+
	;; > | 0x12        | Fixed disk type byte         |
	;; > +-------------+------------------------------+
	;; > | 0x14        | Equipment byte               |
	;; > +-------------+------------------------------+
	;; > | 0x15        | Low base memory byte         |
	;; > +-------------+------------------------------+
	;; > | 0x16        | High base memory byte        |
	;; > +-------------+------------------------------+
	;; > | 0x17        | Low expansion memory byte    |
	;; > +-------------+------------------------------+
	;; > | 0x18        | High expansion memory byte   |
	;; > +-------------+------------------------------+
	;; > | 0x19        | Disk C extended byte         |
	;; > +-------------+------------------------------+
	;; > | 0x1A        | Disk D extended byte         |
	;; > +-------------+------------------------------+
	;; > | 0x2E - 0x2F | CMOS checksum                |
	;; > +-------------+------------------------------+
	;; > | 0x30        | *Low expansion memory byte   |
	;; > +-------------+------------------------------+
	;; > | 0x31        | *High expansion memory byte  |
	;; > +-------------+------------------------------+
	;; > | 0x32        | *Date century byte           |
	;; > +-------------+------------------------------+
	;; > | 0x33        | *Flags                       |
	;; > +-------------+------------------------------+

CMOS_RTC equ 0x00

RTC_SECONDS       equ 0x00
RTC_SECOND_ALRM   equ 0x01
RTC_MINUTES       equ 0x02
RTC_MINUTE_ALRM   equ 0x03
RTC_HOURS         equ 0x04
RTC_HOUR_ALRM     equ 0x05
RTC_WEEKDAY       equ 0x06
RTC_DATE_OF_MONTH equ 0x07
RTC_MONTH         equ 0x08
RTC_YEAR          equ 0x09
RTC_STATUS_REGA   equ 0x0A
RTC_STATUS_REGB   equ 0x0B
RTC_STATUS_REGC   equ 0x0C
RTC_STATUS_REGD   equ 0x0D

STATUS_REGA_UIP equ (1 << 7)

STATUS_REGB_PIE  equ (1 << 6)
STATUS_REGB_AIE  equ (1 << 5)
STATUS_REGB_UIE  equ (1 << 4)
STATUS_REGB_SQWE equ (1 << 3)
STATUS_REGB_DM   equ (1 << 2)
STATUS_REGB_24H  equ (1 << 1)
STATUS_REGB_DSE  equ (1 << 0)

CMOS_CENTURY equ 0x32




	
