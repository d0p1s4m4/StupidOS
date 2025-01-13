MM_KERNEL_PTE_BASE = 0xFFC00000

	include '../sys/mmu.inc'
	include 'bootstrap.inc'
	include 'pmm.inc'
	include 'vmm.inc'

mm_init:
	ret
