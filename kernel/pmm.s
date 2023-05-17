[BITS 32]
	;; XXX: align address to page

%include "base.inc"
%include "multiboot.inc"

extern kernel_size
extern kernel_end
	
struc bitmap
	.length: resd 1
	.addr: resd 1
endstruc

section .text

bitmap_count_free_page:
	push ebp
	mov ebp, esp
	push edi
	push esi
	push ebx

	mov edi, [bitmap_info + bitmap.addr]
	xor ebx, ebx

.loop:

	inc edi
	inc ebx
	cmp ebx, [bitmap_info]
	jb .loop
.end:	
	pop ebx
	pop esi
	pop edi
	leave
	ret

bitmap_mark:
	push ebp
	mov ebp, esp
	push edi
	push esi

	mov edi, 1
	mov eax, [ebp + 8]
	and eax, 0x7				; eax % 8
	mov ecx, eax
	shl edi, cl

	mov eax, [ebp + 8]
	shr eax, 0x3				; eax / 8
	mov esi, [bitmap_info + bitmap.addr]
	add esi, eax
	mov dl, byte [esi]

	mov ecx, [ebp + 12]
	test ecx, ecx
	jz .clear

	or edx, edi					; set bit
	jmp .end

.clear:
	not edi
	and edx, edi				; clear bit

.end:
	mov byte [esi], dl

	pop esi
	pop edi
	leave
	ret
	
init_bitmap:
	push ebp
	mov ebp, esp
	push edi

	mov ecx, [bitmap_info]
	mov al, 0xFF
	mov edi, [bitmap_info + bitmap.addr]
	rep stosb

	pop edi
	leave
	ret

bitmap_mark_range_free:
	push ebp
	mov ebp, esp
	push edi
	push esi
	push ebx

	mov edi, [ebp + 8]			; start address
	mov esi, [ebp + 12]			; length
	xor ebx, ebx				; counter (I know I SHOUD use 'ecx' as counter
								; but since the ABI state it's not preserved
								; accross function call) 
	push dword 0				; bitmap_mark(addr, 0) == clear
.loop:
	cmp ebx, esi
	jnb .end_loop
	add edi, ebx
	jc .end_loop				; exit on overflow
	push edi
	call bitmap_mark
	add esp, 4

	add ebx, 0x1000				; add page size
	jmp .loop
.end_loop:
	add esp, 4
	pop ebx
	pop esi
	pop edi
	leave
	ret
	
setup_pmm_mmap:
	push ebp
	mov ebp, esp
	sub esp, 4
	push edi
	push esi
	push ebx

	mov edi, [ebp + 8]			; mb_mmap struct addr
	mov esi, [ebp + 12]			; mmap length
	xor ebx, ebx				; pass
	mov [ebp - 4], ebx			; set max addr to 0
.loop:
	;; TODO: PAE stuff
	;; skip address > 32bit
	mov eax, [edi + mb_mmap.addr + 4]
	and eax, eax
	jnz .next
	mov eax, [edi + mb_mmap.length + 4]
	and eax, eax
	jnz .next

	;; check if first pass
	and ebx, ebx
	jz .first_pass
	mov eax, [edi + mb_mmap.type]
	and eax, 0x1 				; check if free
	jz .next
	;;
	mov eax, [bitmap_info]
	mov ecx, [mb_mmap.length]
	cmp eax, ecx
	ja .next

	cmp ebx, 0x1
	jne .third_pass
	mov eax, [edi + mb_mmap.addr]
	mov [bitmap_info + bitmap.addr], eax
	LOG msg_bitmap_stored_at, eax
	call init_bitmap			; mark all memory as used
	jmp .bitmap_size_already_set
	
	;; TODO: mark as free in bitmap
.third_pass:
	push dword [edi + mb_mmap.length]
	push dword [edi + mb_mmap.addr]
	call bitmap_mark_range_free
	add esp, 8
	jmp .next

	;; first pass then calculate higher address
.first_pass:
	LOG msg_mmap_entry, dword [edi + mb_mmap.addr + 4], \
		dword [edi + mb_mmap.addr], \
		dword [edi + mb_mmap.length + 4], \
		dword [edi + mb_mmap.length], \
		dword [edi + mb_mmap.type]  
	mov eax, [edi + mb_mmap.length]
	add eax, [edi + mb_mmap.addr]
	jnc .no_overflow
	mov eax, -1					; if overflow set eax to (uint32_t)-1
.no_overflow:
	mov edx, [ebp - 4]
	cmp edx, eax
	ja .next
	mov [ebp - 4], eax
	
.next:
	mov eax, [edi + mb_mmap.size]
	add eax, 0x4
	add edi, eax				; jump to next entry
	sub esi, eax
	jnz .loop

	and ebx, ebx
	jnz .bitmap_size_already_set
	LOG msg_max_mem, dword [ebp - 4]

	mov eax, [ebp - 4]
	shr eax, 15					; eax / (4096*8)
	mov [bitmap_info], eax

.bitmap_size_already_set:
	
	inc ebx
	cmp ebx, 2
	jbe .loop
	
	pop ebx
	pop esi
	pop edi
	leave
	ret

setup_pmm_legacy:
	push ebp
	mov ebp, esp
	push edi
	push esi
	push ebx
	xor eax, eax
	mov ecx, [ebp + 8]
	shl ecx, 0xA				; ecx * 1KiB
	mov esi, ecx
	LOG msg_mem_block, eax, ecx
	
	mov eax, 0x100000 
	mov ecx, [ebp + 12]
	shl ecx, 0xA				; ecx * 1KiB
	mov ebx, ecx
	mov edi, eax
	add edi, ecx
	LOG msg_mem_block, eax, ecx

	LOG msg_max_mem, edi

	shr edi, 15					; (edi / (4046*8))
	mov dword [bitmap_info], edi

	cmp edi, esi
	ja .try_high
	mov dword [bitmap_info + bitmap.addr], 0
	jmp .initialize_bitmap

.try_high:
	mov eax, ebx
	sub ebx, kernel_size
	cmp edi, ebx
	ja .err_no_mem
	mov dword [bitmap_info + bitmap.addr], kernel_end

.initialize_bitmap:
	call init_bitmap

	push esi
	push dword 0
	call bitmap_mark_range_free

	push ebx
	push dword 0x100000
	call bitmap_mark_range_free

	xor eax, eax
	jmp .end
	
.err_no_mem:
	mov eax, 1

.end:
	pop ebx
	pop esi
	pop edi
	leave
	ret

global setup_pmm
setup_pmm:
	push ebp
	mov ebp, esp
	push edi
	push esi
	push ebx

	mov edi, [ebp + 8]

	mov eax, [edi]
	and eax, 0x40				; (1 << 6)
	jz .no_mmap

	push dword [edi + mb_info.mmap_length]
	push dword [edi + mb_info.mmap_addr]
	call setup_pmm_mmap
	add esp, 8

	jmp .end_mem_detection

.no_mmap:
	LOG warn_no_mmap
	mov eax, [edi]
	and eax, 0x1
	jz .err

	push dword [edi + mb_info.mem_upper]
	push dword [edi + mb_info.mem_lower]
	call setup_pmm_legacy
	add esp, 8

.end_mem_detection:
	;; mark bitmap as used
	xor ebx, ebx
	mov edi, [bitmap_info + bitmap.addr]
	mov esi, [bitmap_info]
	push dword 1				; bitmap_mark(addr, 1) == set

.loop:
	cmp ebx, edi
	jnb .end_loop
	add edi, ebx
	push edi
	call bitmap_mark
	add esp, 4

	add ebx, 0x1000
	jmp .loop
.end_loop:
	add esp, 4					; previous "push dword 0"

	xor eax, eax
	jmp .end
.err:
	mov eax, 1
.end:
	pop ebx
	pop esi
	pop ebx
	leave
	ret

global alloc_frames
alloc_frames:
	push esp
	mov ebp, esp
	push edi
	push esi

	mov edi, [ebp + 8]	; count

	pop esi
	pop edi
	leave
	ret

global free_frames
free_frames:
	push esp
	mov ebp, esp

	call bitmap_mark_range_free

	leave
	ret
	
section .data
bitmap_info:
	dd 0 ; size
	dd 0 ; ptr

section .rodata
warn_no_mmap db "[WARN] mmap flag not set", 0
msg_mmap_entry db "Memory Map Entry:", 0xA
	db 0x9, "Address: (hi): %x (lo): %x", 0xA
	db 0x9, "Length: (hi): %x (lo): %x", 0xA
	db 0x9, "Type: %x", 0
msg_mem_block db "Free Memory:", 0xA
	db 0x9, "Address: %x", 0xA
	db 0x9, "Length: %x", 0
msg_max_mem db "Max memory: %x", 0
msg_bitmap_stored_at db "Bitmap stored at: %x", 0
file db __FILE__, 0
