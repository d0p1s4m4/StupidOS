free_block_head dd 0x0

mm_init:

        ret


kernel_page_directory:
        dd 0x1000 dup 0x0
kernel_page_table:
        dd 0x1000 dup 0x0