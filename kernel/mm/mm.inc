MM_KERNEL_PTE_BASE = 0xFFC00000

	include '../sys/mmu.inc'
	include 'bootstrap.inc'
	include 'pmm.inc'
	include 'vmem.inc'

mm_init:
	call pmm_init

	call vmem_bootstrap

	mov eax, stVmemKernel
	push dword 4
	push dword 0
	push dword 0
	push dword 0
	push dword 4096

	mov edx, PMM_VIRT_TEMP
	mov ecx, kend
	sub edx, ecx
	push edx
	mov edx, szVmemKernelName
	call vmem_create
	add esp, 24

	mov eax, stVmemKernel
	mov edx, 4096
	mov ecx, VMEM_VM_BESTFIT
	call vmem_alloc

	ret

szVmemKernelName db "kmem", 0
stVmemKernel Vmem
