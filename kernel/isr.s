[BITS 32]

%include "cpu.inc"
%include "base.inc"

%macro ISR_NO_ERR 1
global isr%1
isr%1:
	xchg bx, bx
	push dword 0
	push dword %1
	jmp isr_common
%endmacro

%macro ISR_ERR 1
global isr%1
isr%1:
	xchg bx, bx
	push dword %1
	jmp isr_common
%endmacro	
	
section .text

ISR_NO_ERR 0
ISR_NO_ERR 1
ISR_NO_ERR 2
ISR_NO_ERR 3
ISR_NO_ERR 4
ISR_NO_ERR 5
ISR_NO_ERR 6
ISR_NO_ERR 7
ISR_ERR 8
ISR_NO_ERR 9
ISR_ERR 10
ISR_ERR 11
ISR_ERR 12
ISR_ERR 13
ISR_ERR 14
ISR_NO_ERR 15
ISR_NO_ERR 16
ISR_NO_ERR 17
ISR_NO_ERR 18
ISR_NO_ERR 19
ISR_NO_ERR 20
ISR_NO_ERR 21
ISR_NO_ERR 22
ISR_NO_ERR 23
ISR_NO_ERR 24
ISR_NO_ERR 25
ISR_NO_ERR 26
ISR_NO_ERR 27
ISR_NO_ERR 28
ISR_NO_ERR 29
ISR_NO_ERR 30
ISR_NO_ERR 31

panic:
	LOG msg_interrupt
	;htl
	;jmp panic
	ret


isr_handler:
	push ebp
	mov ebp, esp

.end:
	leave
	ret

isr_common:
	push ds
	push es
	push fs
	push gs
	pusha

	mov ax, 0x10				; data segment
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	;LOG msg_interrupt
	call isr_handler
	;extern pic_eoi
	;call pic_eoi
	
	popa
	pop gs
	pop fs
	pop es
	pop ds
	add esp, 8					; int no & error code

	iret

section .rodata

msg_int_division_zero: db "Division by zero", 0x0
msg_int_debug: db "Debug", 0x0
msg_int_nmi: db "Non-maskable interrupt", 0x0
msg_int_breakpoint: db "Breakpoint", 0x0
msg_int_overflow: db "Overflow", 0x0
msg_int_range_exceeded: db "Bound range exceeded", 0x0
msg_int_invalid_opcode: db "Invalid Opcode", 0x0
msg_int_device_not_available: db "Device not available", 0x0
msg_int_double_fault: db "Double fault", 0x0
msg_int_coproc_segment_overrun: db "Coprocessor segment overrun", 0x0
msg_int_invalid_tss: db "Invalid TSS", 0x0
msg_int_seg_not_present: db "Segment not present", 0x0
msg_int_stack_segfault: db "Stack segment fault", 0x0
msg_int_gpf: db "General Protection Fault", 0x0
msg_int_page_fault: db "Page fault", 0x0
msg_int_reserved: db "Reserved", 0x0
msg_int_fp_exception: db "x87 Floating-Point Exception", 0x0
msg_int_align_check: db "Aligment check", 0x0
msg_int_machine_check: db "Machine check", 0x0
msg_int_simd_exception: db "SIMD Floating-Point exception", 0x0
msg_int_virt_exception: db "Virtualization exception", 0x0
msg_int_sec_exception: db "Security exception", 0x0

msg_interrupt db "interrupt", 0xA
	db "edi: %x esi: %x ebp: %x esp: %x", 0xA
	db "ebx: %x edx: %x ecx: %x eax: %x", 0xA
	db "gs:  %x fs:  %x es:  %x ds:  %x", 0xA
	db "int: %x err: %x eip: %x cs:  %x", 0xA
	db "flg: %x usp: %x ss:  %x", 0x0
file db __FILE__, 0
