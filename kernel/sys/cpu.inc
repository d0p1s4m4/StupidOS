	;; File: cpu.inc


	;; Structure: tss
	;;
	;; >      31         23         15         7        0
	;; >      +----------|----------+----------|----------+
	;; > 0x64 |     I/O map base    | 00000000   0000000T |
	;; >      +----------|----------+----------|----------+
	;; > 0x60 | 00000000   00000000 |         LDT         |
	;; >      +----------|----------+----------|----------+
	;; > 0x5c | 00000000   00000000 |         GS          |
	;; >      +----------|----------+----------|----------+
	;; > 0x58 | 00000000   00000000 |         FS          |
	;; >      +----------|----------+----------|----------+
	;; > 0x54 | 00000000   00000000 |         DS          |
	;; >      +----------|----------+----------|----------+
	;; > 0x50 | 00000000   00000000 |         SS          |
	;; >      +----------|----------+----------|----------+
	;; > 0x4C | 00000000   00000000 |         CS          |
	;; >      +----------|----------+----------|----------+
	;; > 0x48 | 00000000   00000000 |         ES          |
	;; >      +----------|----------+----------|----------+
	;; > 0x44 |                    EDI                    |
	;; >      +----------|----------+----------|----------+
	;; > 0x40 |                    ESI                    |
	;; >      +----------|----------+----------|----------+
	;; > 0x3C |                    EBP                    |
	;; >      +----------|----------+----------|----------+
	;; > 0x38 |                    ESP                    |
	;; >      +----------|----------+----------|----------+
	;; > 0x34 |                    EBX                    |
	;; >      +----------|----------+----------|----------+
	;; > 0x30 |                    EDX                    |
	;; >      +----------|----------+----------|----------+
	;; > 0x2C |                    ECX                    |
	;; >      +----------|----------+----------|----------+
	;; > 0x28 |                    EAX                    |
	;; >      +----------|----------+----------|----------+
	;; > 0x24 |                  EFLAGS                   |
	;; >      +----------|----------+----------|----------+
	;; > 0x20 |                    EIP                    |
	;; >      +----------|----------+----------|----------+
	;; > 0x1C |                    CR3                    |
	;; >      +----------|----------+----------|----------+
	;; > 0x18 | 00000000   00000000 |         SS2         |
	;; >      +----------|----------+----------|----------+
	;; > 0x14 |                   ESP2                    |
	;; >      +----------|----------+----------|----------+
	;; > 0x10 | 00000000   00000000 |         SS1         |
	;; >      +----------|----------+----------|----------+
	;; > 0x0C |                   ESP1                    |
	;; >      +----------|----------+----------|----------+
	;; > 0x08 | 00000000   00000000 |         SS0         |
	;; >      +----------|----------+----------|----------+
	;; > 0x04 |                   ESP0                    |
	;; >      +----------|----------+----------|----------+
	;; > 0x00 | 00000000   00000000 |  old TSS selector   |
	;; >      +----------|----------+----------|----------+

struc TSS {
	.prev_tss dd ?
	.esp0     dd ?
	.ss0      dd ?
	.esp1     dd ?
	.ss1      dd ?
	.esp2     dd ?
	.ss2      dd ?
	.cr3      dd ?
	.eip      dd ?
	.eflags   dd ?
	.eax      dd ?
	.ecx      dd ?
	.edx      dd ?
	.ebx      dd ?
	.esp      dd ?
	.ebp      dd ?
	.esi      dd ?
	.edi      dd ?
	.es       dd ?
	.cs       dd ?
	.ss       dd ?
	.ds       dd ?
	.fs       dd ?
	.gs       dd ?
	.ldt      dd ?
	.trap     dw ?
	.iomap    dw ?
}

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
defn idt_gate

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