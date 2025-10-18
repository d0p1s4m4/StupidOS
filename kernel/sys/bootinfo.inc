	;; File: bootinfo.inc
	;; StupidOS Boot protocol constants and structs

	;; Constant: BOOT_HEADER_MAGIC
BOOT_HEADER_MAGIC     = 0x44505453
	;; Constant: BOOT_BOOTLOADER_MAGIC
BOOT_BOOTLOADER_MAGIC = 0x53545044

	;; Struct: BootHeader
	;;
	;; .magic         - Must be <BOOT_HEADER_MAGIC>
	;; .load_end_addr - TODO
	;; .bss_end_addr  - TODO
	;; .entry_addr    - Kernel entry point
struc BootHeader {
	.magic         dd ?
	.load_end_addr dd ?
	.bss_end_addr  dd ?
	.entry_addr    dd ?
}

	;; Struct: BootMMapEntry
	;; Memory map entry
	;;
	;; .base   - TODO
	;; .length - TODO
struc BootMMapEntry {
	.base dq ?
	.length dq ?
}

	;; Struct: BootInfoRange
struc BootInfoRange {
	.base dd ?
	.length dd ?
}

	;; Struct: BootInfo
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
virtual at 0
  BootInfo BootInfo
  sizeof.BootInfo:
end virtual
