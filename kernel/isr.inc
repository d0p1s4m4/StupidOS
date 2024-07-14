	;; File: isr.inc
macro ISR [name,error] {
forward
	dd isr_#name
forward
	local szIntName
 szIntName db  `name#, 0
forward
	isr_#name#:
		cli
		mov esi, szIntName
		call klog
		if error eq 0
		push 0
		end if
		push error
		jmp isr_common
				 
}

idt_setup:
	xor ecx, ecx
@@:
	mov eax, [aISRTable + (ecx * 4)]
	mov [aInterruptGate + (ecx * 8)], ax
	mov [aInterruptGate + (ecx * 8) + 2], word 0x08

	mov [aInterruptGate + (ecx * 8) + 5], word 0x8E
	shr eax, 16
	mov [aInterruptGate + (ecx * 8) + 6], ax
	inc ecx
	cmp ecx, 32
	jb @b
@@:
	mov eax, irq_dummy
	mov [aInterruptGate + (ecx * 8)], ax
	mov [aInterruptGate + (ecx * 8) + 2], word 0x08

	mov [aInterruptGate + (ecx * 8) + 5], word 0x8E
	shr eax, 16
	mov [aInterruptGate + (ecx * 8) + 6], ax
	inc ecx
	cmp ecx, 0x30
	jb @b

	mov ecx, 0x42
	mov eax, isr_syscall
	mov [aInterruptGate + (ecx * 8)], ax
	mov [aInterruptGate + (ecx * 8) + 2], word 0x08

	mov [aInterruptGate + (ecx * 8) + 5], word 0x8E
	shr eax, 16
	mov [aInterruptGate + (ecx * 8) + 6], ax

	lidt [pIDT]
	sti
	ret

pIDT:
	dw aInterruptGate.end-aInterruptGate-1
	dd aInterruptGate

; variable: idt_entries
aInterruptGate:
	dd 256*2 dup(0)
.end:


irq_dummy:
	iret

isr_common:
	pusha

	mov ax, ds
	push eax

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov eax, [esp+IntFrame.intno]
	push eax

	mov esi, szMsgInterrupt
	call klog 

	pop eax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	popa
	add esp, 8
	sti
	iret

aISRTable:
ISR \
	division_by_zero, 0, \
	debug, 0, \
	nmi, 0, \
	breakpoint, 0, \
	overflow, 0, \
	bound_range_exceeded, 0, \
	invalid_opcode, 0, \
	device_not_available, 0, \
	double_fault, 1, \
	coproc_seg_overrun, 0, \
	invalid_tss, 1, \
	seg_not_present, 1, \
	stack_seg_fault, 1, \
	general_protection_fault, 1, \
	page_fault, 1, \
	reserved15, 0, \
	x87_exception, 0, \
	alignment_check, 0, \
	machine_check, 0, \
	simd_exception, 0, \
	virt_exception, 0, \
	reserved21, 0, \
	reserved22, 0, \
	reserved23, 0, \
	reserved24, 0, \
	reserved25, 0, \
	reserved26, 0, \
	reserved27, 0, \
	reserved28, 0, \
	reserved29, 0, \
	security_exception, 0, \
	reserved31, 0

szMsgInterrupt db "Interrupt (int: %x)", 0
