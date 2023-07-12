; File: multiboot.inc

; Define: MB_HDR_MAGIC
; Multiboot Header Magic
MB_HDR_MAGIC equ 0x1BADB002

; Define: MB_MAGIC
; Multiboot Magic passed by the bootloader.
MB_MAGIC equ 0x2BADB002

; Structure: mb_header
;
; .magic       - todo
; .flags       - See <flags>
; .checksum    - todo
; .aout_kludge - todo
; .mode_type   - todo
; .width       - todo
; .height      - todo
; .depth       - todo
struc mb_header
	.magic: resd 1
	.flags: resd 1
	.checksum: resd 1
	.aout_kludge: resd 5
	.mode_type: resd 1
	.width: resd 1
	.height: resd 1
	.depth: resd 1
endstruc

; Defines: flags
;
; MB_HDR_ALIGN   - todo
; MB_HDR_MEMINFO - <mb_info.mem_lower>
; MB_HDR_VIDEO   - todo
MB_HDR_ALIGN equ 1 << 0
MB_HDR_MEMINFO equ 1 << 1
MB_HDR_VIDEO equ 1 << 2

; Structure: mb_info
; 
; Fields:
; .flags       - See <flags>
; .mem_lower   - todo
; .mem_upper   - todo
; .boot_device - todo
;    .
struc mb_info

	;; flags
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
	; <mb_info.mmap_length>
	; see <mb_mmap>
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

; Defines: flags
;
; MB_INFO_MEM     - todo
; MB_INFO_BOOTDEV - todo
MB_INFO_MEM             equ 1 << 0
MB_INFO_BOOTDEV         equ 1 << 1
MB_INFO_CMDLINE         equ 1 << 2
MB_INFO_MODULES         equ 1 << 3
MB_INFO_MMAP            equ 1 << 6
MB_INFO_DRIVES          equ 1 << 7
MB_INFO_BOOTLOADER_NAME equ 1 << 9
MB_INFO_VBE             equ 1 << 11
MB_INFO_FRAMEBUFFER     equ 1 << 12

; Structure: mb_mmap
struc mb_mmap
	.size: resd 1
	.addr: resq 1
	.length: resq 1
	.type: resd 1
endstruc

; Defines: types
;
; MB_MEMORY_AVAILABLE - 1
; MB_MEMORY_RESERVED  - 2
; MB_MEMORY_ACPI      - 3
; MB_MEMORY_NVS       - 4
; MB_MEMORY_BADRAM    - 5
%define MB_MEMORY_AVAILABLE 1
%define MB_MEMORY_RESERVED 2
%define MB_MEMORY_ACPI 3
%define MB_MEMORY_NVS 4
%define MB_MEMORY_BADRAM 5
