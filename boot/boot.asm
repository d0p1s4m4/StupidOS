        ORG 0x7C00
        USE16

        jmp short _start
        nop

        ; Boot Record
OEM_identifier      db 'STUPID '
bytes_per_sector    dw 512
sectors_per_cluster db 1
reserved_sectors    dw 1
FAT_count           db 2
root_dir_entries    dw 224
total_sectors       dw 2880
media_desc_type     db 0xF0
sectors_per_FAT     dw 9
sectors_per_track   dw 18
heads_per_cylinder  dw 2
hidden_sectors      dd 0
large_sector_count  dd 0

        ; Extended Boot Record
drive_number db 0
reserved     db 0x90
signature    db 0x29   ; 0x28 or 0x29
volume_id    dd 0xB00B135 ; hope mine will grow :'(
volume_label db 'StupidOS   '
system_id    db 'FAT12  '


_start:
        cli
        cld
        jmp 0x0:.canonicalize_cs

.canonicalize_cs:
        xor ax, ax
        mov ds, ax
        mov ss, ax
        mov es, ax
        mov sp, 0x7c00

        ; clear screen
        mov al, 0x03
        mov ah, 0
        int 0x10

        mov si, msg_hello
        call bios_print

        ; reset floppy disk
@@:
        xor ah, ah
        int 0x13
        jc @b

        mov si, msg_load_stage
        call bios_print

        ; read sector into ES:BX (0100:0000)
        mov ax, 0x100 ; 100 segment
        mov es, ax    ; 0100:0000 (0000:1000)
        xor bx, bx

        mov ah, 0x2 ; read
        mov al, 0x1 ; one sector
        mov ch, 0x0 ; cylinder 0
        mov cl, 0x2 ; second sector
        mov dh, 0x0 ; head 0
        int 0x13
        jc .error

        jmp 0x0:0x1000

.error:
        mov si, msg_error_sector
        call bios_print

        hlt
        jmp $

bios_print:
        lodsb
        or al, al
        jz @f
        mov ah, 0x0E
        int 0x10
        jmp bios_print
@@:
        ret

msg_hello        db "StupidOS Bootloader", 0x0D, 0x0A, 0
msg_load_stage   db "Loading next stage", 0x0D, 0x0A, 0
msg_error_sector db "ERROR: Can't read sector", 0x0D, 0x0A, 0

        rb 0x7C00+512-2-$
        db 0x55, 0xAA

; ---------- stage 1 --------------

        ORG 0x1000
stage2:
        push cs
        pop ds

        mov si, msg_stage2
        call bios_print

        call a20_enable
        jc .error_a20

        xchg bx, bx

        jmp .hang
.error_a20:
        mov si, msg_error_a20
        call bios_print
.hang:
        hlt
        jmp $

        INCLUDE 'a20.inc'

msg_stage2    db "StupidOS Bootloader (Stage 2)", 0x0D, 0x0A, 0
msg_error_a20 db "ERROR: can't enable a20 line", 0x0D, 0x0A, 0
