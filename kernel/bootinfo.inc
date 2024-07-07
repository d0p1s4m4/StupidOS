	;; File: bootinfo.inc

	;; Struct: BootInfoRange
struc BootInfoRange {
	.base dd ?
	.length dd ?
}

	;; Struct: BootInfo
	;;
	;; StupidOS boot protocol structure
struc BootInfo {
	.mmap         dd 4*2*20 dup(0)
	.kernel_start dd ?
	.kernel_size  dd ?
}
virtual at 0
  BootInfo BootInfo
  sizeof.BootInfo:
end virtual
