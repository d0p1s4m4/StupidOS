MULTIBOOT_HDR_MAGIC = 0x1BADB002
MULTIBOOT_MAGIC     = 0x2BADB002

MULTIBOOT_HDR_ALIGN       = 0x1
MULTIBOOT_HDR_MEMINFO     = 0x2
MULTIBOOT_HDR_VIDEO       = 0x4
MULTIBOOT_HDR_AOUT_KLUDGE = 0x10000
MULTIBOOT_FLAGS = MULTIBOOT_HDR_ALIGN or MULTIBOOT_HDR_MEMINFO or MULTIBOOT_HDR_VIDEO or MULTIBOOT_HDR_AOUT_KLUDGE

struc MultibootHeader addr
{
        .magic    dd MULTIBOOT_HDR_MAGIC
        .flags    dd MULTIBOOT_FLAGS
        .checksum dd -(MULTIBOOT_HDR_MAGIC + MULTIBOOT_FLAGS)

        ; address fields (we'll just skip them)
        .header_addr   dd addr
        .load_addr     dd LOADER_BASE
        .load_end_addr dd _edata
        .bss_end_addr  dd _end
        .entry_addr    dd _start

        ; Video mode
        .mode_type dd 0x0
        .width     dd VIDEO_WIDTH
        .height    dd VIDEO_HEIGHT
        .depth     dd VIDEO_DEPTH
}

struc MultibootData
{
        .flags dd ?

        ; if flags[0] is set
        .mem_lower dd ?
        .mem_upper dd ?

        ; if flags[1] is set
        .boot_device dd ?

        ; if flags[2] is set
        .cmdline dd ?

        ; if flags[3] is set
        .mods_count dd ?
        .mods_addr  dd ?

        ; if flags[4] is set
        .syms dd 4 dup ?

        ; if flags[6] is set
        .mmap_length dd ?
        .mmap_addr   dd ?

        ; if flags[7] is set
        .drives_length dd ?
        .drives_addr   dd ?

        ; if flags[8] is set
        .config_table dd ?

        ; if flags[9] is set
        .bootloader_name dd ?

        ; if flags[10] is set
        .apm_table dd ?

        ; if flags[11] is set
        .vbe_control_info dd ?
        .vbe_mode_info    dd ?
        .vbe_mode         dw ?
        .vbe_if_seg       dw ?
        .vbe_if_off       dw ?
        .vbe_if_length    dw ?

        ; if flags[12] is set
        .fb_addr   dq ?
        .fb_pitch  dd ?
        .fb_width  dd ?
        .fb_height dd ?
        .fb_bpp    db ?
        .fb_type   db ?
        .fb_misc   dw 3 dup ?
}
DEFN MultibootData

MULTIBOOT_DATA_MEM             = 0x0001
MULTIBOOT_DATA_BOOTDEV         = 0x0002
MULTIBOOT_DATA_CMDLINE         = 0x0004
MULTIBOOT_DATA_MODULES         = 0x0008
MULTIBOOT_DATA_MMAP            = 0x0040
MULTIBOOT_DATA_DRIVES          = 0x0080
MULTIBOOT_DATA_BOOTLOADER_NAME = 0x0200
MULTIBOOT_DATA_VBE             = 0x0800
MULTIBOOT_DATA_FB              = 0x1000

struc MultibootMMap
{
        .size   dd ?
        .addr   dq ?
        .length dq ?
        .type   dd ?
}
DEFN MultibootMMap

MULTIBOOT_MEMORY_AVAILABLE = 0x1
MULTIBOOT_MEMORY_RESERVED  = 0x2
MULTIBOOT_MEMORY_ACPI      = 0x3
MULTIBOOT_MEMORY_NVS       = 0x4
MULTIBOOT_MEMORY_BADPARAM  = 0x5

struc MultibootModule
{
	.mod_start dd ?
	.mod_end   dd ?
	.cmdline   dd ?
	.pad       dd ?
}
DEFN MultibootModule
