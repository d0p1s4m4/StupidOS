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

	; print hello world 
	mov [0xC03B0000], dword 0x08740953
	mov [0xC03B0004], dword 0x05700675
	mov [0xC03B0008], dword 0x03640469
	mov [0xC03B000C], dword 0x0153024F

	mov esi, szMsgKernelAlive
	call klog

	; init pmm (kend, 0x3B0000)
	mov eax, kend
	mov ebx, 0xC03B0000
	call pmm_init

	; init vmm
	call mm_init

	; map whole memory

	;  idt, gdt


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

szMsgKernelAlive db "Kernel is alive", 0
szErrorBootProtocol db "Error: wrong magic number", 0

boot_structure BootInfo

	align 4096
stack_bottom:
	rb 0x4000
stack_top:

kend:
