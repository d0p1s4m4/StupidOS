	;; File: vm.inc
	;; Stupid OS virtual Memory manager
	;;
	;; About: Memory Management
	;;
	;; Terminology:
	;; VA  - Virtual Address
	;; PA  - Physical Address
	;; PDE - Page Directory Entry
	;; PTE - Page Table Entry
	;;
	;; Memory Layout:
	;;
	;; Virtual Memory above `0xC0000000` is mapped as kernel only <PTE_U> flags
	;;
	;; >                 Virtual
	;; > 0xFFFFFFFF +---------------+
	;; >            |               |
	;; >            | Device Memory |
	;; >            |               |
	;; >            +---------------+
	;; >            |               |
	;; >            | Kernel Heap   |
	;; >            |               |
	;; >   KERN_END +---------------+
	;; >            |               |
	;; >            | Stupid Kernel |
	;; >            |               |
	;; > 0xC0100000 +---------------+
	;; >            | I/O Space and |
	;; >            | phys memory   |
	;; >            | Lower than 1M | kernel mode only
	;; > 0xC0000000 +---------------+
	;; >            |               | user mode
	;; >            |  userspace    |
	;; >            |               |
	;; > 0x00000000 +---------------+
	;;
	;; Page Frame Allocator
	;;
%define K2P(addr) (addr - KERNBASE)
%define P2K(addr) (addr + KERNBASE)

struc vm_kmap
	.virt: resd 1
	.phys_start: resd 1
	.phys_stop:  resd 1
	.access: resb 1
endstruc

struc vm_map
	.pmap: resd 1

endstruc

struc
	.directory: resd 1
	.ref_count: resd 1
	
endstruc
