	;; File: registers.inc


	;; =========================================================================
	;;  Control Registers
	;; =========================================================================

	;; Defines: CR0
	;; CR0_PE - Protected Mode Enable
	;; CR0_MP - Monitor co-processor
	;; CR0_EM - x87 FPU Emulation
	;; CR0_TS - Task switched
	;; CR0_ET - Extension type
	;; CR0_NE - Numeric error
	;; CR0_WP - Write protect
	;; CR0_AM - Alignment mask
	;; CR0_NW - Not-write through
	;; CR0_CD - Cache disable
	;; CR0_PG - Paging
CR0_PE equ 1 << 0
CR0_MP equ 1 << 1
CR0_EM equ 1 << 2
CR0_TS equ 1 << 3
CR0_ET equ 1 << 4
CR0_NE equ 1 << 5
CR0_WP equ 1 << 16
CR0_AM equ 1 << 18
CR0_NW equ 1 << 29
CR0_CD equ 1 << 30
CR0_PG equ 1 << 31

	;; Defines: CR3
	;; CR3_PWT - Page-level Write-Through
	;; CR3_PCD - Page-level Cache Disable
CR3_PWT equ 1 << 3
CR3_PCD equ 1 << 4

	;; Defines: CR4
	;; CR4_VME        - Virtual 8086 Mode Extensions
	;; CR4_PVI        - Protected-mode Virtual Interrupts
	;; CR4_TSD        - Time Stamp Disable
	;; CR4_DE         - Debugging Extensions
	;; CR4_PSE        - Page Size Extension
	;; CR4_PAE        - Physical Address Extension
	;; CR4_MCE        - Machine Check Exception
	;; CR4_PGE        - Page Global Enabled
	;; CR4_PCE        - Performance-Monitoring Counter enable
	;; CR4_OSFXSR     - Operating system support for FXSAVE and FXRSTOR
	;;                  instructions
	;; CR4_OSXMMEXCPT - Operating System Support for Unmasked SIMD
	;;                  Floating-Point Excepions
	;; CR4_UMIP       - User-Mode Instruction Prevention
	;; CR4_VMXE       - Virtual Machine Extensions Enable
	;; CR4_SMXE       - Safer Mode Extensions Enable
	;; CR4_FSGSBASE   -
	;; CR4_PCIDE      - PCID Enable
	;; CR4_OSXSSAVE   - XSAVE and Processor Extended States Enable
	;; CR4_SMEP       - Supervisor Mode Execution Protection Enable
	;; CR4_SMAP       - Supervisor Mode Access Prevention Enable
	;; CR4_PKE        - Protection Key Enable
	;; CR4_CET        - Control-flow Enforcement Technology
	;; CR4_PKS        - Enable Protection Keys for Supervisor-Mode Pages
CR4_VME        equ 1 << 0
CR4_PVI        equ 1 << 1
CR4_TSD        equ 1 << 2
CR4_DE         equ 1 << 3
CR4_PSE        equ 1 << 4
CR4_PAE        equ 1 << 5
CR4_MCE        equ 1 << 6
CR4_PGE        equ 1 << 7
CR4_PCE        equ 1 << 8
CR4_OSDXSR     equ 1 << 9
CR4_OSXMMEXCPT equ 1 << 10
CR4_UMIP       equ 1 << 11
CR4_VMXE       equ 1 << 13
CR4_SMXE       equ 1 << 14
CR4_FSGSBASE   equ 1 << 16
CR4_PCIDE      equ 1 << 17
CR4_OSXSAVE    equ 1 << 18
CR4_SMEP       equ 1 << 20
CR4_SMAP       equ 1 << 21
CR4_PKE        equ 1 << 22
CR4_CET        equ 1 << 23
CR4_PKS        equ 1 << 24

	;; =========================================================================
	;; eflags
	;; =========================================================================

	;; Defines: EFLAGS
	;; EFLAGS_CF    - Carry flag
	;; EFLAGS_PF    - Parity flag
	;; EFLAGS_AF    - Auxiliary flag
	;; EFLAGS_ZF    - Zero flag
	;; EFLAGS_SF    - Sign flag
	;; EFLAGS_TF    - Trap flag
	;; EFLAGS_IF    - Interrupt enable flag
	;; EFLAGS_DF    - Direction flag
	;; EFLAGS_OF    - Overflow flag
	;; EFLAGS_IOPL1 - I/O privilege flag
	;; EFLAGS_IOPL2 - I/O privilege flag
	;; EFLAGS_NT    - Nested task flag
	;; EFLAGS_RF    - Resume flag
	;; EFLAGS_VM    - Virtual 8086 mode flag
	;; EFLAGS_AC    - Alignment check
	;; EFLAGS_VIF   - Virtual Interrupt flag
	;; EFLAGS_VIP   - Virtual Interrupt pending
	;; EFLAGS_ID    - CPUID instruction available
EFLAGS_CF    equ 1 << 0
EFLAGS_PF    equ 1 << 2
EFLAGS_AF    equ 1 << 4
EFLAGS_ZF    equ 1 << 6
EFLAGS_SF    equ 1 << 7
EFLAGS_TF    equ 1 << 8
EFLAGS_IF    equ 1 << 9
EFLAGS_DF    equ 1 << 10
EFLAGS_OF    equ 1 << 11
EFLAGS_IOPL1 equ 1 << 12
EFLAGS_IOPL2 equ 1 << 13
EFLAGS_NT    equ 1 << 14
EFLAGS_RF    equ 1 << 16
EFLAGS_VM    equ 1 << 17
EFLAGS_AC    equ 1 << 18
EFLAGS_VIF   equ 1 << 19
EFLAGS_VIP   equ 1 << 20
EFLAGS_ID    equ 1 << 21
