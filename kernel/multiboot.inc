struc mb_info
	.flags: resd 1

	;; flags[0] is set
	.mem_lower: resd 1
	.mem_upper: resd 1

	;; flags[1] is set
	.boot_device: resd 1

	;; flags[2] is set
	.cmdline: resd 1

	;; flags[3] is set
	.mods_count: resd 1
	.mods_addr: resd 1

	;; flags[4] || flags[4] is set
	.syms: resd 4

	;; flags[6] is set
	.mmap_length: resd 1
	.mmap_addr: resd 1

	;; flags[7] is set
	.drives_length: resd 1
	.drives_addr: resd 1

	;; flags[8] is set
	.config_table: resd 1

	;; flags[9] is set
	.bootloader_name: resd 1

	;; flags[10] is set
	.apm_table: resd 1

	;; 
	.vbe_control_info: resd 1
	.vbe_mode_info: resd 1
	.vbe_mode: resw 1
	.vbe_interface_seg: resw 1
	.vbe_interface_off: resw 1
	.vbe_interface_len: resw 1

	.framebuffer_addr: resq 1
	.framebuffer_pitch: resd 1
	.framebuffer_width: resd 1
	.framebuffer_height: resd 1
	.framebuffer_bpp: resb 1
	.framebuffer_type: resb 1
	.framebuffer_misc: resw 3
endstruc

struc mb_mmap
	.size: resd 1
	.addr: resq 1
	.length: resq 1
	.type: resd 1
endstruc

%define MB_MEMORY_AVAILABLE 1
%define MB_MEMORY_RESERVED 2
%define MB_MEMORY_ACPI 3
%define MB_MEMORY_NVS 4
%define MB_MEMORY_BADRAM 5
