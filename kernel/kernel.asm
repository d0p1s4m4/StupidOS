	;; File: kernel.asm
	format binary

	include 'const.inc'

	org KBASE
	use32

	jmp short kmain

db 'STPDKRNL'
db 32 dup(0)

	;; Function: kmain
	;;
	;; Parameters:
	;; 
	;;     EAX - Boot Magic
	;;     EBX - Boot structure address
	;;
kmain:
	mov esp, stack_top

	cmp eax, STPDBOOT_MAGIC
	jne .error_magic

	; Copy boot structure
	mov ecx, sizeof.BootInfo
	mov esi, ebx
	mov edi, boot_structure
	rep movsb

	; print hello world 
	mov [0xC00B8000], dword 0x08740953
	mov [0xC00B8004], dword 0x05700675
	mov [0xC00B8008], dword 0x03640469
	mov [0xC00B800C], dword 0x0153024F

	mov esi, szMsgKernelAlive
	call klog

	; init pmm (kend, 0x400000)
	mov eax, kend
	mov ebx, 0xC0400000
	call pmm_init

	; init vmm
	call mm_init

	mov eax, 0xC0400000
	mov ebx, [boot_structure.high_mem]
	add ebx, KERNEL_VIRT_BASE
	call pmm_free_range

	; load kernel gdt 
	lgdt [pGDT]
	; I don't think i need to reload segment cuz their value are already correct

	call idt_setup


.halt:
	hlt
	jmp $
.error_magic:
	mov esi, szErrorBootProtocol
.error:
	call klog
	jmp .halt

	include 'sys/bootinfo.inc'
	include 'klog.inc'
	include 'dev/vga_console.inc'
	include 'mm/mm.inc'
	include 'lock.inc'
	include 'gdt.inc'
	include 'isr.inc'

szMsgKernelAlive db "Kernel is alive", 0
szErrorBootProtocol db "Error: wrong magic number", 0

boot_structure BootInfo

	align 4096
stack_bottom:
	rb 0x4000
stack_top:

kend:
