	;; Struc: PMMFreeRange
struc PMMFreeRange {
	.size dd ?
	.next dd ?
}

	;; 
pPMMFreeListHead dd 0

pmm_alloc:
	ret

pmm_alloc_page:
	ret

pmm_free_page:
	ret

	;;
	;; kend - 4Mb
	;; 8Mb - max ram
pmm_init:
	ret
