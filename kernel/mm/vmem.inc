	;; File: vmem.inc
	;; 
	;; General purpose resource allocator.
	;; Based on <tinyvmem at https://github.com/rdmsr/tinyvmem> and on <Bonwick's paper at https://www.usenix.org/legacy/event/usenix01/full_papers/bonwick/bonwick.pdf>

;;; MARK: macros and structs

VM_BESTFIT    = (1 shl 0)
VM_INSTANTFIT = (1 shl 1)
VM_NEXTFIT    = (1 shl 2)
VM_SLEEP      = (1 shl 3)
VM_NOSLEEP    = (1 shl 4)
VM_BOOTSTRAP  = (1 shl 5)

STATIC_SEG_COUNT = 64

FREELISTS_N = 4 * 8
HASHTABLE_N = 16

SEGMENT_ALLOCATED = 0
SEGMENT_FREE      = 1
SEGMENT_SPAN      = 2

macro GET_LIST reg, N {
	bsr reg, reg
	xor reg, 31
	add reg, -N
	sub reg, 1
}

struc VmSegment {
	.type db ?
	.imported db ?
	.base dd ?
	.size dd ?

	.spanlist ListEntry
	.seglist ListEntry
}
DEFN VmSegment

	;; Struc: Vmem
struc Vmem {
	.name db 16 dup(0)
	.base dd ?
	.size dd ?
	.quantum dd ?
	.alloc dd ?
	.free dd ?
	.source dd ?
	.vmflag dd ?

	.freelist dd FREELISTS_N dup(0)
	.hashtable dd HASHTABLE_N dup(0)
	.spanlist ListHead 0
}
DEFN Vmem

;;; MARK: private functions

	;; Function: _murmur32
	;;
	;; In:
	;;    EAX - Address
	;; 
	;; Out:
	;;    EAX - Hash
	;;
_murmur32:
	; hash ← hash XOR (hash >> 16)
	mov ecx, eax
	shr ecx, 16
	xor eax, ecx
	; hash ← hash × 0x85ebca6b
	mov ecx, 0x85ebca6b
	mul ecx
	; hash ← hash XOR (hash >> 13)
	mov ecx, eax
	shr ecx, 13
	xor eax, ecx
	; hash ← hash × 0xc2b2ae35
	mov ecx, 0xc2b2ae35
	mul ecx
	; hash ← hash XOR (hash >> 16)
	mov ecx, eax
	shr ecx, 16
	xor eax, ecx
	ret

	;; Function: _hash_table_addr
	;;
	;; In:
	;;    EAX - Vmem
	;;    EDX - ADDR 
__hash_table_addr:
	push ebp
	mov ebp, esp
	push esi

	mov esi, eax
	mov eax, edx
	call _murmur32
	and eax, HASHTABLE_N-1 ; x % y (y is a power of two)
	shl eax, 2

	mov edx, esi
	add edx, Vmem.hashtable
	add edx, eax
	mov eax, [edx]

	pop esi
	leave
	ret

	;; Function: _vmm_segfree
	;; 
	;; In:
	;;    EAX - Segment address
_vmm_segfree:
	;mov ecx, eax
	;add ecx, VmSegment.seglist.next

	;mov edx, [pVmmFreeSegHead]
	;or edx, edx
	;jz @f
	;mov [eax + VmSegment.seglist.next], edx
	;mov [edx + VmSegment.seglist.prev], ecx
;@@:
;	mov [pVmmFreeSegHead], eax
;	mov [eax + VmSegment.seglist.prev], ecx
;
;	inc [iVmmFreeSegCount]
	ret
;
;	;; Function: _vmm_segalloc
;_vmm_segalloc:
;	mov eax, [pVmmFreeSegHead]
;	or eax, eax
;	jz @f
;
;	dec [iVmmFreeSegCount]
;@@:
;	ret


;;; MARK: public functions

	;; Function: vmem_create
	;;
	;; Creates a new <Vmem> arena whose initial span is [Base, Base + Size).
	;;
	;; In:
	;;   EAX     - Pointer to a <Vmem> object.
	;;   EDX     - String.
	;;   ECX     - Base.
	;;   ESP[4]  - Size in bytes.
	;;   ESP[8]  - Quantum.
	;;   ESP[12] - Alloc function.
	;;   ESP[16] - Free function.
	;;   ESP[20] - Source.
	;;   ESP[24] - Flags. (<VM_SLEEP> or <VM_NOSLEEP>)
vmem_create:
	push ebp
	mov ebp, esp

	mov [eax + Vmem.base], ecx
	


if CONFIG_TRACE_VMEM
	push esi
	push edi
	push ebx

	mov esi, eax
	mov edi, edx
	mov ebx, ecx

	mov eax, esi
	mov edx, edi
	mov ecx, ebx

	pop ebx
	pop edi
	pop esi
end if

	leave
	ret

	;; Function: vmem_destroy
	;;
	;; Destroys an arena
	;;
	;; In:
	;;    EAX - Pointer to a <Vmem> object.
vmem_destroy:
	ret

	;; Function: vmem_alloc
	;;
	;; Allocates n-bytes from arena.
	;;
	;; In:
	;;    EAX - Pointer to a <Vmem> object.
	;;    EDX - Size in bytes.
	;;    ECX - Flags
	;;
	;; Out:
	;;    EAX - Allocated address on succes, 0 on failure.
vmem_alloc:
	ret

	;; Function: vmem_free
	;; 
	;; Free n-bytes at address to arena
	;; 
	;; In:
	;;    EAX - Pointer to a <Vmem> object.
	;;    EDX - Address.
	;;    ECX - Size in bytes.
vmem_free:
	ret

	;; Function: vmem_xalloc
	;;
	;; In:
	;;    EAX     - Pointer to a <Vmem> object.
	;;    EDX     - Size in bytes.
	;;    ECX     - Alignment boundary.
	;;    ESP[4]  - Phase.
	;;    ESP[8]  - Nocross.
	;;    ESP[12] - Min address.
	;;    ESP[16] - Max address.
	;;    ESP[20] - Flags. (<VM_BESTFIT>, <VM_INSTANTFIT>, <VM_NEXTFIT>, <VM_SLEEP>, <VM_NOSLEEP>)
	;;
	;; Out:
	;;    EAX - Allocated address on succes, 0 on failure.
vmem_xalloc:
	ret

	;; Function: vmem_xfree
	;;
	;; Frees n-bytes at address.
	;;
	;; In:
	;;    EAX - Pointer to <Vmem> object.
	;;    EDX - Address.
	;;    ECX - Size in bytes.
vmem_xfree:
	ret 

	;; Function: vmem_add
	;;
	;; Adds a span [Address, Address + Size) to an arena.
	;;
	;; In:
	;;    EAX    - Pointer to <Vmem> object.
	;;    EDX    - Address.
	;;    ECX    - Size in bytes.
	;;    ESP[4] - Flags. (<VM_SLEEP> or <VM_NOSLEEP>)
	;;
	;; Out:
	;;    EAX - Return Address on success, 0 on failure.
vmem_add:
	ret

;;; MARK: variables

aVmemStaticSegs   db STATIC_SEG_COUNT*sizeof.VmSegment dup(0)
pVmemFreeSegHead  dd 0
iVmemFreeSegCount dd 0

if CONFIG_TRACE_VMM
szTraceVmemCreate  db "Trace(VMEM): Create %s [%x, %x)", 0
szTraceVmemAdd     db "Trace(VMEM): %s: Add span [%x, %x)", 0
szTraceVmemDestroy db "Trace(VMEM): Destroy %s", 0
end if
