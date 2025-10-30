	;; File: bootinfo.inc
	;; StupidOS Boot protocol constants and structs

	;; Define: BOOT_HEADER_MAGIC
BOOT_HEADER_MAGIC     = 0x44505453
	;; Define: BOOT_BOOTLOADER_MAGIC
BOOT_BOOTLOADER_MAGIC = 0x53545044

	;; Struc: BootHeader
	;;
	;; .magic         - Must be <BOOT_HEADER_MAGIC>
	;; .load_end_addr - TODO
	;; .bss_end_addr  - TODO
	;; .entry_addr    - Kernel entry point
struc BootHeader {
	.magic         dd ?
	.entry_addr    dd ?
	.load_end_addr dd ?
	.bss_end_addr  dd ?
}

	;; Struc: BootMMapEntry
	;; Memory map entry
	;;
	;; .base   - TODO
	;; .length - TODO
struc BootMMapEntry {
	.base dq ?
	.length dq ?
}
DEFN BootMMapEntry

	;; Define: BOOT_MAX_MMAP_ENTRY
BOOT_MAX_MMAP_ENTRY = 16

	;; Struc: BootInfoRange
struc BootInfoRange {
	.base dd ?
	.length dd ?
}

	;; Struc: BootInfo
	;; StupidOS boot protocol structure
	;;
	;; .mmap         - Free memory map
	;; .kernel_start - Kernel start address
	;; .kernel_size  - Kernel size in bytes
	;; .high_mem     - Free memory under 1MB
	;; .low_mem      - Free memory upper 1MB
struc BootInfo {
	.mmap         dd 4*2*20 dup(0)
	.kernel_start dd ?
	.kernel_size  dd ?
	.high_mem     dd ?
	.low_mem      dd ?
}
DEFN BootInfo
