
        ; 0xE820 Get System Memory Map
        ; EAX=0x0000E820
        ; EDX=0x534D4150
        ; EBX=0x0 or continuation value
        ; ECX=buffer size
        ; ES:SI = buffer
memory_do_E820:

        ret

memory_get_map:
        call memory_do_E820
        jnc .end

        ; try 0x88
        clc
        mov ah, 0x88
        int 0x15
        jc .error
        test ax, ax
        je .error
.end:
        clc
        ret
.error:
        stc
        ret