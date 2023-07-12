	;; File: pmap.s
	;; Physical Memory Map

	;; Structure: pmap_block
	;; 
	;; .next        - pointer to next pmap_block struct
	;; .phys_start  - start physical address of memory range
	;; .phys_end    - stop address of memory range
	;; .bitmap_size - bitmap size (max: 0xFF0)
	;; .last_alloc  - last allocated frame in bitmap
	;; .bitmap      - bitmap array
	;;
	;; >          pmap_block
	;; > 0x0000 +-------------+
	;; >        | next        |-----> pmap_block
	;; > 0x0004 +-------------+       +----------+
	;; >        | phys start  |       | next     |----> NULL (0x0)
	;; > 0x0008 +-------------+       +----------+
	;; >        | phys end    |       | .....    |
	;; > 0x000C +-------------+       | .....    |
	;; >        | bitmap size |       | .....    |
	;; > 0x000e +-------------+       +----------+
	;; >        | last alloc  |
	;; > 0x0010 +-------------+
	;; >        | bitmap      |
	;; > 0x???? +-------------+ (bitmap size * 4)
	;;
struct pmap_block
	.next:        resd 1
	.phys_start:  resd 1
	.phys_end:    resd 1
	.bitmap_size: resw 1
	.last_alloc:  resw 1
	.bitmap:
endstruc

section .text
	;; Function: pmap_bootstrap
	;;
	;; (see pmap_bootstrap.png)
pmap_bootstrap:
	push esp
	mov esp, ebp

	leave
	ret
