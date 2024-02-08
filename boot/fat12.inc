        ATTR_READ_ONLY = 0x01
        ATTR_HIDDEN    = 0x02
        ATTR_SYSTEM    = 0x04
        ATTR_VOLUME_ID = 0x08
        ATTR_DIRECTORY = 0x10
        ATTR_ARCHIVE   = 0x20

        struc fat_entry
        {
                .name          db 8 dup ?
                .ext           db 3 dup ?
                .attrs         db ?
                .reserved      dw ?
                .creation_time dw ?
                .creation_date dw ?
                .access_date   dw ?
                .reserved2     dw ?
                .mod_time      dw ?
                .mod_date      dw ?
                .start         dw ?
                .size          dd ?
        }

        virtual at di

                fat_entry fat_entry
        end virtual


        ; CHS to LBA
        ; LBA = (C * HPC + H) * SPT + (S - 1)

        ;;; Read sectors from disk to buffer
        ;;;
        ;;; @param AX LBA starting sector
        ;;; @param CX sector count
        ;;; @param ES:BX buffer
        ;;;
disk_read_sectors:
        ; https://en.wikipedia.org/wiki/Logical_block_addressing
        ; convert LBA to CHS
        ; HPC = Head per Cluster
        ; SPT = Sector per Track

        ; S = (LBA % SPT) + 1
        push ax
        push bx
        push cx
        xor dx, dx
        div word [sectors_per_track]
        inc dx
        mov [S], dx

        ; H = (LBA / SPT) % HPC
        ; C =  LBA  / (HPC * SPT)
        xor dx, dx
        div word [heads_per_cylinder]
        mov [C], ax
        mov [H], dx

        ; read sectors
        mov ah, 0x2
        mov al, 0x1
        mov ch, byte [C]
        mov cl, byte [S]
        mov dh, byte [H]
        mov dl, [drive_number]

        int 0x13
        jc @f
        pop cx
        pop bx
        pop ax
        add bx, word [bytes_per_sector]
        inc ax
        loop disk_read_sectors
        ret
@@:
        mov si, msg_error_sector
        call bios_print
        ret

C dw 0x00
H dw 0x00
S dw 0x00

fat_load_root:
        mov ax, DISK_BUFFER/0x10
        mov es, ax

        ; load root directory
        mov ax, [sectors_per_FAT]
        xor cx, cx
        mov cl, [FAT_count]
        mul cx
        add ax, [reserved_sectors]
        push ax

        mov bx, [bytes_per_sector]
        mov cl, 0x5
        shr bx, cl
        mov ax, [root_dir_entries]
        xor dx, dx
        div bx
        mov cx, ax
        pop ax

        mov [data_start], ax
        add [data_start], cx

        xor bx, bx

        call disk_read_sectors

        ret

        ;; @param SI filename to search
fat_search_root:
        mov cx, [root_dir_entries]
        mov di, 0x0
@@:
        push si
        push cx
        mov cx, 0xB ; name(8) + ext(3)
        push di
        rep cmpsb
        pop di
        pop cx
        pop si
        je .file_found
        add di, 0x20
        loop @b
        ; set carry if not found
        stc
        ret
.file_found:
        mov ax, [es:fat_entry.start]
        clc
        ret

        ;;; @param AX cluster
        ;;; @param ES:BX buffer
fat_load_binary:
        push ax
        sub ax, 0x2
        xor cx, cx
        mov cl, [sectors_per_cluster]
        mul cx

        add ax, [data_start]
        xor cx, cx
        mov cl, [sectors_per_cluster]
        call disk_read_sectors

        pop ax
        mov cx, ax
        mov dx, ax
        shr dx, 0x1
        add cx, dx
        push bx
        mov bx, DISK_BUFFER
        add bx, cx
        mov dx, [bx]
        pop bx

        test ax, 0x1
        jnz .odd_cluster
.even_cluster:
        and dx, 0xFFF
        jmp .end
.odd_cluster:
        shr dx, 0x4
.end:
        mov ax, dx
        cmp dx, 0xFF0
        jb fat_load_binary
        ret


msg_error_sector           db "ERROR: reading sector", CR, LF, 0
data_start dw 0x0