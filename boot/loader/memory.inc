	;; File: memory.inc
	;; Detect available memory 
	;; 

	;; Constants: Address type
	;;
	;; See <AddressRange.Type>
	;;
	;; ADDRESS_RANGE_MEMORY   - Available and usable RAM.
	;; ADDRESS_RANGE_RESERVED - Reserved or in use.
ADDRESS_RANGE_MEMORY   = 1
ADDRESS_RANGE_RESERVED = 2

	;; Struct: AddressRange
struc AddressRange
{
	;; Variable: BaseAddrLow
	;; Low 32 Bits of Base Address
	.BaseAddrLow  dd ?
	;; Variable: BaseAddrHigh
	;; High 32 Bits of Base Address
	.BaseAddrHigh dd ?
	;; Variable: LengthLow
	;; Low 32 Bits of Length in Bytes
	.LengthLow    dd ?
	;; Variable: LengthHigh
	;; High 32 Bits of Length in Bytes
	.LengthHigh   dd ?
	;; Variable: Type
	;; Address type of this range. <Address type>
	.Type         db ?
}
defn AddressRange

    ;; Function: memory_e820_get_mmap_entry
	;; 
	;; In:
	;;    EAX   - Function code
	;;    EBX   - Continuation
	;;    ES:DI - Buffer Pointer
	;;    ECX   - Buffer size
	;;    EDX   - Signature 'SMAP'
	;;
	;; Out:
	;;    CF    - Carry Flag
	;;    EAX   - Signature 'SMAP'
	;;    ES:DI - Buffer Pointer
	;;    ECX   - Buffer Size
	;;    EBX   - Continuation
memory_e820_get_map:
	mov eax, 0xE820
	mov ebx, 0x0
	mov ecx, 'SMAP'

	ret

	;; Function: memory_get_for_large_conf
	;;
	;; In:
	;;     AX - Function code E801h
	;;
	;; Out:
	;;     CF - Non-Carry - indicates no error
	;;     AX - Extended 1
	;;     BX - Extended 2
	;;     CX - Configured 1
	;;     DX - Configured 2
memory_get_for_large_conf:
	mov ax, 0xE801
	int 0x15

	;; Function: memory_get_extended_memory_size
	;;
	;; In:
	;;    AH - Function code 88h
	;;  
    ;; Out:
	;;    CF - Non-Carry - indicates no error
	;;    AX - Number of contiguous KB above 1MB
memory_get_map:
	clc
	ret
