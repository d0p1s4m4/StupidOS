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
struc E820AddressRange
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
	.Type         dd ?

}

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
	xor ebx, ebx
@@:	
	clc
	xor eax, eax
	mov es, ax
	mov di, pE280AddrRange
	mov ecx, 20
	mov eax, 0xE820
	mov edx, 'PAMS'
	int 0x15
	jc .error
	cmp eax, 'PAMS'
	jne .error
	or ebx, ebx
	jz .end

	cmp [pE280AddrRange.Type], 0x1
	jne @b

	mov ecx, [pE280AddrRange.BaseAddrHigh]
	or ecx, ecx
	jnz @b

	mov ecx, [pE280AddrRange.LengthHigh]
	or ecx, ecx
	jnz @b

	mov edx, [pE280AddrRange.LengthLow]
	mov eax, [pE280AddrRange.BaseAddrLow]
	call boot_info_add_memmap

	jmp @b
.error:
	stc
.end:
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
	clc
	mov ax, 0xE801
	int 0x15
	jc @f

	movzx edx, ax
	mov eax, 0x100000 ; 1MB
	call boot_info_add_memmap

	mov eax, 0x1000000 ; 16MB
	movzx edx, bx
	shl edx, 16 ; * 64KB
	call boot_info_add_memmap
@@:
	ret

	;; Function: memory_get_extended_memory_size
	;;
	;; In:
	;;    AH - Function code 88h
	;;  
    ;; Out:
	;;    CF - Non-Carry - indicates no error
	;;    AX - Number of contiguous KB above 1MB
memory_get_map:
	call memory_e820_get_map
	jnc .end

	call memory_get_for_large_conf
	jnc .end
	clc
	mov ah, 0x88
	int 0x15

	jc .end

	movzx edx, ax
	mov eax, 0x100000
	call boot_info_add_memmap

.end:
	ret


pE280AddrRange E820AddressRange
