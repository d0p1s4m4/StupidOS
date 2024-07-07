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
CR0_PE = 0x00000001
CR0_MP = 0x00000002
CR0_EM = 0x00000004
CR0_TS = 0x00000008
CR0_ET = 0x00000010
CR0_NE = 0x00000020
CR0_WP = 0x00010000
CR0_AM = 0x00040000
CR0_NW = 0x20000000
CR0_CD = 0x40000000
CR0_PG = 0x80000000

	;; Defines: CR3
	;; CR3_PWT - Page-level Write-Through
	;; CR3_PCD - Page-level Cache Disable
CR3_PWT = 0x08
CR3_PCD = 0x10

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
CR4_VME        = 0x0000001
CR4_PVI        = 0x0000002
CR4_TSD        = 0x0000004
CR4_DE         = 0x0000008
CR4_PSE        = 0x0000010
CR4_PAE        = 0x0000020
CR4_MCE        = 0x0000040
CR4_PGE        = 0x0000080
CR4_PCE        = 0x0000100
CR4_OSDXSR     = 0x0000200
CR4_OSXMMEXCPT = 0x0000400
CR4_UMIP       = 0x0000800
CR4_VMXE       = 0x0002000
CR4_SMXE       = 0x0004000
CR4_FSGSBASE   = 0x0010000
CR4_PCIDE      = 0x0020000
CR4_OSXSAVE    = 0x0040000
CR4_SMEP       = 0x0100000
CR4_SMAP       = 0x0200000
CR4_PKE        = 0x0400000
CR4_CET        = 0x0800000
CR4_PKS        = 0x1000000

