        INCLUDE 'boot/boot.inc'

        ORG $ + KBASE

        INCLUDE 'mm/mm.inc'

kmain:
       nop

_edata:

        ; BSS
        rb 0x4000

_end: