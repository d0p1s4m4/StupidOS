	;; File: cpu.inc

	;; Structure: idt_gate
	;; .offset_low  - TODO
	;; .selector    - TODO
	;; .zero        - TODO
	;; .attributes  - TODO
	;; .offset_high - TODO
	;;
struc idt_gate
	.offset_low:  resw 1
	.selector:    resw 1
	.zero:        resb 1
	.attributes:  resb 1
	.offset_high: resw 1
endstruc

	;; About: Gates
	;; - Task Gate
	;; >  31               23               15                7                0
	;; > +----------------|----------------+-----------------|-----------------+
	;; > |            (NOT USED)           | P DPL 0 0 1 0 1      (NOT USED)   |
	;; > +----------------|----------------+-----------------|-----------------+
	;; > |            SELECTOR             |             (NOT USED)            |
	;; > +----------------|----------------+-----------------|-----------------+
	;;
	;; - Interrupt Gate
	;; >  31               23               15                7                0
	;; > +----------------|----------------+-----------------|-----------------+
	;; > |          OFFSET 31..16          | P DPL 0 1 1 1 0   0 0 0 0 0 0 0 0 |
	;; > +----------------|----------------+-----------------|-----------------+
	;; > |            SELECTOR             |           OFFSET 15..0            |
	;; > +--------------- |----------------+-----------------|-----------------+
	;;
	;; - Trap Gate
	;; >  31               23               15                7                0
	;; > +----------------|----------------+-----------------|-----------------+
	;; > |          OFFSET 31..16          | P DPL 0 1 1 1 1   0 0 0 0 0 0 0 0 |
	;; > +----------------|----------------+-----------------|-----------------+
	;; > |            SELECTOR             |           OFFSET 15..0            |
	;; > +--------------- |----------------+-----------------|-----------------+

struc intframe
	;; registers
	.edi: resd 1
	.esi: resd 1
	.ebp: resd 1
	.esp: resd 1
	.ebx: resd 1
	.edx: resd 1
	.ecx: resd 1
	.eax: resd 1

	;;
	.gs:    resd 1
	.fs:    resd 1
	.es:    resd 1
	.ds:    resd 1
	.intno: resd 1

	;; by x86 hardware
	.err:    resd 1
	.eip:    resd 1
	.cs:     resd 1
	.eflags: resd 1

	;; crossring
	.useresp: resd 1
	.ss:      resd 1
endstruc
