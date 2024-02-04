        ORG 0x100000
        ALIGN 4
        USE32

        INCLUDE 'boot/multiboot.inc'
        INCLUDE 'const.inc'

mb_header MultibootHeader MB_FLAGS, mb_header, VIDEO_WIDTH, VIDEO_HEIGHT, VIDEO_DEPTH

_start:
        cli
        cmp eax, MULTIBOOT_MAGIC
        jne hang

        ; iterate over memory

        ;

        ;
        xor esi, esi
        mov edi, kernel_page_table - KBASE

        mov ecx, kernel_page_directory - KBASE
        mov cr3, ecx

        mov ecx, cr0


hang:
        hlt
        jmp $-1

       ; SEGMENT readable writable

boot_struct dd 0x0