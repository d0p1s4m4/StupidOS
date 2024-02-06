        ; copy/pasted from https://wiki.osdev.org/A20_Line
        ; .... sorry I was lazy :x

a20_get_state:
        pushf
        push si
        push di
        push ds
        push es

        cli

        xor ax, ax
        mov ds, ax
        mov si, 0x500

        not ax
        mov es, ax
        mov di, 0x0510

        mov al, [ds:si]  ; save old values
        mov byte [.BufferBelowMB], al
        mov al, [es:di]
        mov byte [.BufferOverMB], al
 
        mov ah, 1 ; check byte [0x00100500] == byte [0x0500]
        mov byte [ds:si], 0
        mov byte [es:di], 1
        mov al, [ds:si]
        cmp al, [es:di]
        jne .exit
        dec ah
.exit:
        mov al, [.BufferBelowMB]
        mov [ds:si], al
        mov al, [.BufferOverMB]
        mov [es:di], al
        shr ax, 8
        sti
        pop es
        pop ds
        pop di
        pop si
        popf
        ret
 
.BufferBelowMB: db 0
.BufferOverMB   db 0


a20_query_support:
        push bx
        clc
 
        mov ax, 0x2403
        int 0x15
        jc .error
 
        test ah, ah
        jnz .error
 
        mov ax, bx
        pop bx
        ret
.error:
        stc
        pop bx
        ret
 
a20_enable_keyboard_controller:
        cli
 
        call .wait_io1
        mov al, 0xad
        out 0x64, al
 
        call .wait_io1
        mov al, 0xd0
        out 0x64, al
 
        call .wait_io2
        in al, 0x60
        push eax
 
        call .wait_io1
        mov al, 0xd1
        out 0x64, al
 
        call .wait_io1
        pop eax
        or al, 2
        out 0x60, al
 
        call .wait_io1
        mov al, 0xae
        out 0x64, al
 
        call .wait_io1
        sti
        ret
.wait_io1:
        in al, 0x64
        test al, 2
        jnz .wait_io1
        ret
.wait_io2:
        in al, 0x64
        test al, 1
        jz .wait_io2
        ret
 
;       out:
;               cf - set on error
a20_enable:
        clc                                                                     ;       clear cf
        pusha
        mov bh, 0                                                       ;       clear bh
 
        call a20_get_state
        jc .fast_gate
 
        test ax, ax
        jnz .done
 
        call a20_query_support
        mov bl, al
        test bl, 1                                                      ;       enable A20 using keyboard controller
        jnz .keybord_controller
 
        test bl, 2                                                      ;       enable A20 using fast A20 gate
        jnz .fast_gate
.bios_int:
        mov ax, 0x2401
        int 0x15
        jc .fast_gate
        test ah, ah
        jnz .failed
        call a20_get_state
        test ax, ax
        jnz .done
.fast_gate:
        in al, 0x92
        test al, 2
        jnz .done
 
        or al, 2
        and al, 0xfe
        out 0x92, al
 
        call a20_get_state
        test ax, ax
        jnz .done
 
        test bh, bh                                                     ;       test if there was an attempt using the keyboard controller
        jnz .failed
.keybord_controller:
        call a20_enable_keyboard_controller
        call a20_get_state
        test ax, ax
        jnz .done
 
        mov bh, 1                                                       ;       flag enable attempt with keyboard controller
 
        test bl, 2
        jnz .fast_gate
        jmp .failed
.failed:
        stc
.done:
        popa
        ret