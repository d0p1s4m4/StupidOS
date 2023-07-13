%include "sys/i386/mmu.inc"
%include "sys/i386/task.inc"

section .text
global tss_install
tss_install:
	push ebp
	mov ebp, esp
	push esi
	mov esi, [ebp + 8]

	mov eax, tss_size
	lea ebx, tss_entry
	mov [esi + gdt_entry.limit_low], ax
	mov [esi + gdt_entry.base_low], bx
	shr eax, 16
	mov [esi + gdt_entry.base_mid], al

	mov al, 0x9 | (1 << 7)
	mov [esi + gdt_entry.access], al

	shr ebx, 16
	and bl, 0xF
	mov [esi + gdt_entry.flags], bl

	mov [esi + gdt_entry.base_high], ah

	mov dword [tss_entry + tss.ss0], 0x10
	extern stack_top
	mov dword [tss_entry + tss.esp0], stack_top

	leave
	ret

global tss_flush
tss_flush:
	mov ax, (5 * 8) | 0
	ltr ax
	ret

section .data

tss_entry:
	times tss_size db 0
