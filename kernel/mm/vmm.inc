	;; File: vm.inc
	;; Based on <tinyvmem at https://github.com/rdmsr/tinyvmem> and on <Bonwick's paper at https://www.usenix.org/legacy/event/usenix01/full_papers/bonwick/bonwick.pdf>

	;; Function: vmm_init
	;; 
	;; Initializes a Vmem arena
	;;
	;; In:
	;;     EAX     - Pointer to a Vmem dest
	;;     EDX     - Base address
	;;     ECX     - Size
	;;     ESP[4]  - Quantum
	;;     ESP[8]  - Pointer to alloc function
	;;     ESP[12] - Pointer to free function
	;;     ESP[16] - Poiter to Vmem source
	;;     ESP[20] - qcache max
	;;     ESP[24] - Flags
vmm_init:
	push ebp
	mov ebp, esp

	push esi
	push edi 

	mov [eax + Vmem.base], edx
	mov [eax + Vmem.size], ecx

	; copy other param from the stack
	mov ecx, 28
	mov edx, esp
	add edx, 4
	mov esi, edx
	mov edx, eax
	add edx, Vmem.quantum
	mov edi, edx
	rep movsd

	mov ecx, [eax + Vmem.size]
	or ecx, ecx
	jz @f

	mov edx, [eax + Vmem.source]
	or edx, edx
	jnz @f

	mov edx, [eax + Vmem.base]

;	call vmm_add
	add esp, 4

@@:
	pop edi
	pop esi

	leave
	ret

	;; Function: vmm_bootstrap
	;;
vmm_bootstrap:
	push ebp 
	mov ebp, esp

	push ebx

	xor ebx, ebx
@@:
	mov eax, aVmemStaticSegs
	add eax, ebx

	call _vmm_segfree

	add ebx, sizeof.VmSegment
	cmp ebx, STATIC_SEG_COUNT*sizeof.VmSegment
	jl @b

	pop ebx

	leave
	ret


