MM_PTE_BASE = 0xFFC00000
MM_VIRT_TEMP = 0xD0000000

	;; EAX - Physical address to map
mm_map_temp:
	mov ecx, MM_PTE_BASE + (MM_VIRT_TEMP shr 12)
	or eax, 0x3
	mov [ecx], eax

if COMPAT_I386
	mov eax, 0x40000 + (1023 * PAGE_SIZE)
	mov cr3, eax
else
	mov eax, MM_VIRT_TEMP
	invlpg [eax]
end if

	ret

mm_unmap_temp:
	mov ecx, MM_PTE_BASE + (MM_VIRT_TEMP shr 12)
	mov dword [ecx], 0

if COMPAT_I386
	mov eax, 0x40000 + (1023 * PAGE_SIZE)
	mov cr3, eax
else
	mov eax, MM_VIRT_TEMP
	invlpg [eax]
end if
	ret

mm_init:
	ret
