        INCLUDE 'const.inc'

        ORG STAGE0_BASE
        USE16

        jmp short _start
        nop

        ; Boot Record
OEM_identifier      db 'STUPID  '
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
drive_number db 0x0
reserved     db 0x0
signature    db 0x29   ; 0x28 or 0x29
volume_id    dd 0xB00B135 ; hope mine will grow :'(
volume_label db 'Stupid Boot'
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

        mov [drive_number], dl

        ; reset floppy disk
@@:
        mov dl, [drive_number]
        xor ah, ah
        int 0x13
        jc @b

        call fat_load_root

        ; search in root directory

        mov si, kernel_file
        call fat_search_root
        jc .error_not_found
        mov [kernel_start], ax

        mov si, stage1_file
        call fat_search_root
        jc .error_not_found
        mov [stage1_start], ax

        ; load fat
        xor ax, ax
        mov al, [FAT_count]
        mul word [sectors_per_FAT]
        mov cx, ax
        mov ax, [reserved_sectors]

        xor bx, bx

        call disk_read_sectors

        ; preload kernel
        mov ax, KERNEL_PRELOAD/0x10
        mov es, ax
        mov ax, [kernel_start]
        xor bx, bx
        call fat_load_binary

        ; load stage 2
        mov ax, STAGE1_BASE/0x10
        mov es, ax
        mov ax, [stage1_start]
        xor bx, bx
        call fat_load_binary

        jmp 0x0:STAGE1_BASE

.error_not_found:
        push si
        mov si, msg_error
        call bios_print
        pop si
        call bios_print
        mov si, msg_not_found
        call bios_print
        hlt
        jmp $

        INCLUDE "utils.inc"
        INCLUDE "fat12.inc"

msg_error                  db "ERROR: ", 0
msg_not_found              db " not found", CR, LF, 0

kernel_file db "VMSTUPIDSYS", 0
stage1_file db "STPDBOOTSYS", 0

kernel_start dw 0x0
stage1_start dw 0x0


        rb 0x7C00+512-2-$
        db 0x55, 0xAA
