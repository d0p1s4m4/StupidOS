; file: msg_en
; English strings

.section rodata
msg_hello_world:
	db "StupidOS v", STUPID_VERSION, " (built with ", NASM_VERSION, " on ", BUILD_DATE, ")", 0
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
