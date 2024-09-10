	;; File: bootinfo.inc

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
