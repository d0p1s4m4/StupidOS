; file: msg_en.s
; English strings

%include "i18n.inc"

.section rodata
global lang_en
lang_en:
	istruc lang_entry
		at lang_entry.code: db "en"
		at lang_entry.data: dd msg_en
		at lang_entry.next: dd 0
	iend

msg_en:
	istruc msg_table
		at msg_table.hello_world: dd msg_hello_world

		at msg_table.cpu_exceptions:
			dd msg_int_division_zero
			dd msg_int_debug
			dd msg_int_nmi
			dd msg_int_breakpoint
			dd msg_int_overflow
			dd msg_int_range_exceeded
			dd msg_int_invalid_opcode
			dd msg_int_device_not_available
			dd msg_int_double_fault
			dd msg_int_coproc_segment_overrun
			dd msg_int_invalid_tss
			dd msg_int_seg_not_present
			dd msg_int_stack_segfault
			dd msg_int_gpf
			dd msg_int_page_fault
			dd msg_int_reserved
			dd msg_int_fp_exception
			dd msg_int_align_check
			dd msg_int_machine_check
			dd msg_int_simd_exception
			dd msg_int_virt_exception
			dd msg_int_reserved
			dd msg_int_reserved
			dd msg_int_reserved
			dd msg_int_reserved
			dd msg_int_reserved
			dd msg_int_reserved
			dd msg_int_reserved
			dd msg_int_reserved
			dd msg_int_reserved
			dd msg_int_reserved
			dd msg_int_reserved
	iend

msg_hello_world:
	db "StupidOS v", STUPID_VERSION, " (built with NASM v", __NASM_VER__, " on ",  __DATE__, " ", __TIME__, ")", 0
msg_boot_info:
	db "Bootloader: %s", 0
msg_pmm_initialized:
	db "PMM initialized", 0
err_invalid_boot_magic:
	db "[ERROR] Invalid boot magic (got: %x, expected: 0x2BADB002)", 0
err_cannot_map_memory:
	db "[ERROR] Can't map memory", 0
warn_no_mmap:
	db "[WARN] mmap flag not set", 0
msg_mmap_entry:
	db "Memory Map Entry:", 0xA
	db 0x9, "Address: (hi): %x (lo): %x", 0xA
	db 0x9, "Length: (hi): %x (lo): %x", 0xA
	db 0x9, "Type: %x", 0
msg_mem_block:
	db "Free Memory:", 0xA
	db 0x9, "Address: %x", 0xA
	db 0x9, "Length: %x", 0
msg_max_mem:
	db "Max memory: %x", 0
msg_bitmap_stored_at:
	db "Bitmap stored at: %x", 0


msg_int_division_zero: db "Division by zero", 0x0
msg_int_debug: db "Debug", 0x0
msg_int_nmi: db "Non-maskable interrupt", 0x0
msg_int_breakpoint: db "Breakpoint", 0x0
msg_int_overflow: db "Overflow", 0x0
msg_int_range_exceeded: db "Bound range exceeded", 0x0
msg_int_invalid_opcode: db "Invalid Opcode", 0x0
msg_int_device_not_available: db "Device not available", 0x0
msg_int_double_fault: db "Double fault", 0x0
msg_int_coproc_segment_overrun: db "Coprocessor segment overrun", 0x0
msg_int_invalid_tss: db "Invalid TSS", 0x0
msg_int_seg_not_present: db "Segment not present", 0x0
msg_int_stack_segfault: db "Stack segment fault", 0x0
msg_int_gpf: db "General Protection Fault", 0x0
msg_int_page_fault: db "Page fault", 0x0
msg_int_reserved: db "Reserved", 0x0
msg_int_fp_exception: db "x87 Floating-Point Exception", 0x0
msg_int_align_check: db "Aligment check", 0x0
msg_int_machine_check: db "Machine check", 0x0
msg_int_simd_exception: db "SIMD Floating-Point exception", 0x0
msg_int_virt_exception: db "Virtualization exception", 0x0
msg_int_sec_exception: db "Security exception", 0x0
