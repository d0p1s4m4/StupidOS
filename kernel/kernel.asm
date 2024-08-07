	;; File: kernel.asm
	format binary

	include 'const.inc'
	include 'sys/macro.inc'
	include 'sys/bootinfo.inc'
	include 'sys/cpu.inc'
	include 'sys/errno.inc'
	include 'sys/process.inc'

	org KBASE
	use32

	jmp short kmain

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
	mov esi, szMsgBuildDate
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

	;mov eax, [boot_structure.low_mem]
	;call heap_init

	call pic_init

	; clear tss
	mov ecx, sizeof.TSS
	xor ax, ax
	mov edi, kTSS
	rep movsb

	mov cx, 0xdfff
	mov eax, kTSS
	mov [eax + TSS.iomap], cx

	call gdt_set_tss
	call gdt_flush
	call tss_flush

	call idt_setup

	mov ax, 100 ; 100Hz
	call pit_init

	call bio_init

	call proc_init

	call dev_init

	call vfs_init

	mov eax, SYSCALL_EXIT
	int 0x42

	;mov al, 'X'
	;call cga_putc

.halt:
;	hlt
	jmp $
.error_magic:
	mov esi, szErrorBootProtocol
.error:
	call klog
	jmp .halt

	include 'klog.inc'
	include 'dev/console.inc'
	include 'dev/dev.inc'
	include 'mm/mm.inc'
	include 'lock.inc'
	include 'gdt.inc'
	include 'syscall.inc'
	include 'isr.inc'
	include 'idt.inc'
	include 'pic.inc'
	;include 'heap.inc'
	include 'vfs.inc'
	include 'fs/fat.inc'
	include 'fs/stpdfs.inc'
	include 'fs/xv6fs.inc'
	include 'proc.inc'
	include 'bio.inc'


szMsgKernelAlive db "Kernel (", VERSION_FULL , ") is alive", 0
szMsgBuildDate db "Built ", BUILD_DATE, 0
szErrorBootProtocol db "Error: wrong magic number", 0

boot_structure BootInfo

kTSS TSS

	align 4096
aProcs rb 64*sizeof.Process
	align 4096
aBuffers rb 30*sizeof.Buffer

	align 4096
stack_bottom:
	rb 0x4000
stack_top:

kend:
