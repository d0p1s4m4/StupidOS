struc tss
	.link: resd 1
	
endstruc

struc gdt_entry
	.limit_low: resw 1
	.base_low: resw 1
	.base_mid: resb 1
	.access: resb 1
	.flags: resb 1
	.base_high: resb 1
endstruc

struc idt_entry
	.limit_low: resw 1
	.selector: resw 1
	.zero: resb 1
	.attr: resb 1
	.base_high: resw 1
endstruc

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
	.gs: resd 1
	.fs: resd 1
	.es: resd 1
	.ds: resd 1
	.intno: resd 1

	;; by x86 hardware
	.err: resd 1
	.eip: resd 1
	.cs:  resd 1
	.eflags: resd 1

	;; crossring
	.useresp: resd 1
	.ss: resd 1
endstruc

