	extern kernel_start
	extern kernel_end
	extern kernel_size

pmm_check_page_free:
	push ebp
	mov ebp, esp

	
	leave
	ret

pmm_setup_from_multiboot_mmap:
	push ebp
	mov ebp, esp
	push edi
	push esi
	push ebx

	mov eax, [esi + mb_info.mmap_addr]
	pop ebx
	pop esi
	pop edi
	leave
	ret
