	;; File: mmu.inc
	;;
	;; About: Address Translation
	;;
	;; Since we don't use segmentation, this is how address translation works
	;; > 31       21       11       0
	;; > +--------+--------+--------+
	;; > |   DIR  |  PAGE  | OFFSET |
	;; > +--------+--------+--------+
	;; >               |
	;; >               v
	;; >         Page Translation
	;; >               |
	;; > 31            v            0
	;; > +--------------------------+
	;; > |     Pyshical address     |
	;; > +--------------------------+
	;;
	;; About: Page Translation
	;;
	;; > +--------+--------+--------+             +--------+
	;; > |   DIR  |  PAGE  | OFFSET |-----+       |  Phys  |
	;; > +--------+--------+--------+     +------>|  addr  |
	;; >   |             |                        |        |
	;; >   |             +----+                   +--------+
	;; >   |   Page Dir       |             +---> Page Frame
	;; >   |  +---------+     |  +--------+ |
	;; >   |  |         |     |  |        | |
	;; >   +->| PDE     |-+   +->| PTE    |-+
	;; >      |         | |      |        |
	;; >      +---------+ |      +--------+
	;; >                  +----> Page Table
	;; >
	;;

	;; Defines: Page Directory Flags
	;; PDE_P   - Present
	;; PDE_W   - Writable
	;; PDE_U   - User
	;; PDE_PWT - Write-Through
	;; PDE_PS  - 4MiB page
PDE_P   = 0x001
PDE_W   = 0x002
PDE_U   = 0x004
PDE_PWT = 0x008
PDE_PCD = 0x010
PDE_A   = 0x020
PDE_D   = 0x040
PDE_PS  = 0x080
PDE_G   = 0x100

	;; Defines: Page Table Flags
	;; PTE_P   - Present
	;; PTE_W   - Writable
	;; PTE_U   - User
	;; PTE_PWT - Write-Through
	;; PTE_PCD - Cache Disable
	;; PTE_A   - Accessed
	;; PTE_D   - Dirty
	;; PTE_PAT - TODO
	;; PTE_G   - TODO
PTE_P   = 0x001
PTE_W   = 0x002
PTE_U   = 0x004
PTE_PWT = 0x008
PTE_PCD = 0x010
PTE_A   = 0x020
PTE_D   = 0x040
PTE_PAT = 0x080
PTE_G   = 0x100
