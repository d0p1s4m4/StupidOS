        INCLUDE 'const.inc'

        ORG STAGE1_BASE
        USE16

stage2:
        push cs
        pop ds

        mov si, msg_stage2
        call bios_print

        call a20_enable
        jc .error_a20

        jmp .hang
.error_a20:
        mov si, msg_error_a20
        call bios_print
.hang:
        hlt
        jmp $

        INCLUDE 'a20.inc'
        INCLUDE 'utils.inc'

msg_stage2    db "StupidOS Bootloader (Stage 1)", 0x0D, 0x0A, 0
msg_error_a20 db "ERROR: can't enable a20 line", 0x0D, 0x0A, 0
