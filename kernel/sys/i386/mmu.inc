	;; File: mmu.inc

%define V2P(addr)   (addr - KERNBASE)
%define P2V(addr)   (addr + KERNBASE)
%define P2PDE(addr) ((addr >> 22) & 0x3FF)
%define PDE2P(addr) (addr << 22)
%define P2PTE(addr) ((addr >> 12) & 0x3FF)

	;; Defines: PDE
	;; PTE_P    - Present
PDE_P   equ 1 << 0
PDE_W   equ 1 << 1
PDE_U   equ 1 << 2
PDE_PWT equ 1 << 3
PDE_PCD equ 1 << 4
PDE_A   equ 1 << 5
PDE_D   equ 1 << 6
PDE_PS  equ 1 << 7
PDE_G   equ 1 << 8

	;; Defines: PTE
	;; PTE_P   - Present
	;; PTE_W   - Writable
	;; PTE_U   - User
	;; PTE_PWT - Write-Through
	;; PTE_PCD - Cache Disable
	;; PTE_A   - Accessed
	;; PTE_D   - Dirty
	;; PTE_PAT -
	;; PTE_G   -
PTE_P   equ 1 << 0
PTE_W   equ 1 << 1
PTE_U   equ 1 << 2
PTE_PWT equ 1 << 3
PTE_PCD equ 1 << 4
PTE_A   equ 1 << 5
PTE_D   equ 1 << 6
PTE_PAT equ 1 << 7
PTE_G   equ 1 << 8

