	;; File: vmem.inc
	;; 
	;; General purpose resource allocator.
	;; Based on <tinyvmem at https://github.com/rdmsr/tinyvmem> and on <Bonwick's paper at https://www.usenix.org/legacy/event/usenix01/full_papers/bonwick/bonwick.pdf>

;;; MARK: Macros and structs

	;; Constant: VMEM_VM_BESTFIT
VMEM_VM_BESTFIT    = (1 shl 0)
	;; Constant: VMEM_VM_INSTANTFIT
VMEM_VM_INSTANTFIT = (1 shl 1)
	;; Constant: VMEM_VM_SLEEP
VMEM_VM_SLEEP      = (1 shl 3)
	;; Constant: VMEM_VM_NOSLEEP
VMEM_VM_NOSLEEP    = (1 shl 4)
	;; Constant: VMEM_VM_BOOTSTRAP
VMEM_VM_BOOTSTRAP  = (1 shl 5)
	;; Constant: VMEM_VM_POPULATING
VMEM_VM_POPULATING = (1 shl 6)

	;; Constant: VMEM_VM_FIT_MASK
VMEM_VM_FIT_MASK = (VMEM_VM_BESTFIT or VMEM_VM_INSTANTFIT)

	;; Constant: VMEM_STATIC_SEG_COUNT
VMEM_STATIC_SEG_COUNT = 64

	;; Constant: VMEM_MIN_RESERVE
VMEM_MIN_RESERVE = 4

	;; Constant: VMEM_FREELISTS_N
VMEM_FREELISTS_N = 4 * 8
	;; Constant: VMEM_HASHTABLE_N
VMEM_HASHTABLE_N = 16

	;; Constant: VMEM_SEGMENT_ALLOCATED
VMEM_SEGMENT_ALLOCATED   = 0
	;; Constant: VMEM_SEGMENT_FREE
VMEM_SEGMENT_FREE        = 1
	;; Constant: VMEM_SEGMENT_SPAN
VMEM_SEGMENT_SPAN        = 2
	;; Constant: VMEM_SEGMENT_STATIC_SPAN
VMEM_SEGMENT_STATIC_SPAN = 3

	;; Constant: VMEM_ALLOC
VMEM_ALLOC = (1 shl 0)
	;; Constant: VMEM_FREE
VMEM_FREE  = (1 shl 1)

	;; Constant: VMEM_MIN_ADDR
VMEM_ADDR_MIN = 0
	;; Constant: VMEM_MAX_ADDR
VMEM_ADDR_MAX = -1

	;; macro: VMEM_GET_LIST_FROM_SIZE
macro VMEM_GET_LIST_FROM_SIZE reg, N {
	bsr reg, reg
	xor reg, 31
	add reg, -N
	neg reg
	sub reg, 1
}

	;; Struc: VmemSegment
struc VmemSegment {
	.type dd ? ; TODO: rework VmemSegment struct
	.base dd ?
	.size dd ?

	.segqueue TailQEntry
	.seglist ListEntry
}
DEFN VmemSegment

	;; Struc: Vmem
struc Vmem {
	.name db 16 dup(0)
	.base dd ?
	.size dd ?
	.quantum dd ?
	.alloc dd ?
	.free dd ?
	.source dd ?
	.flags dd ?

	.in_use dd ?

	.segqueue TailQHead
	.freelist dd VMEM_FREELISTS_N dup(0)
	.hashtable dd VMEM_HASHTABLE_N dup(0)
}
DEFN Vmem

;;; MARK: Private functions

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
	and eax, VMEM_HASHTABLE_N-1 ; x % y (y is a power of two)
	shl eax, 2

	mov edx, esi
	add edx, Vmem.hashtable
	add edx, eax
	mov eax, [edx]

	pop esi
	leave
	ret

	;; Function: _vmem_segfree
	;;
	;; In:
	;;    EAX - Segment address
	;;
_vmem_segfree:
	; XXX: lock 
	add eax, VmemSegment.seglist
	mov edx, pVmemFreeSegHead
	xchg eax, edx
	call list_insert_head

	inc [iVmemFreeSegCount]
	; XXX: unlock

	ret

	;; Function: _vmem_segalloc
	;;
	;; Out:
	;;    EAX - Pointer to <VmemSegment> object on success, 0 on error
_vmem_segalloc:
	push ebp
	mov ebp, esp
	; XXX: lock
	
	mov eax, [pVmemFreeSegHead]
	or eax, eax
	jz .end

	push ebx
	mov ebx, eax
	
	call list_remove

	sub ebx, VmemSegment.seglist
	dec [iVmemFreeSegCount]

	mov eax, ebx

	pop ebx

.end:
	; XXX: unlock
	leave
	ret

	;; Function: _vmem_refill
	;;
_vmem_refill:
	push ebp 
	mov ebp, esp

	xor eax, eax
	; TODO: allocate page
	;call kalloc()
	or eax, eax
	jz .end

	push esi
	push edi

	mov esi, eax
	xor edi, edi
@@:
	mov eax, [esi]
	call _vmem_segfree

	inc [iVmemFreeSegCount]

	add esi, sizeof.VmemSegment
	add edi, sizeof.VmemSegment

	cmp edi, PAGE_SIZE
	jl @b

	pop edi 
	pop esi

.end:
	leave
	ret

	;; Function: _vmem_segfit
	;;
	;; Check if a <VmemSegment> meet restrictions.
	;;
	;; In: 
	;;    EAX     - seg
	;;    EDX     - size
	;;    ECX     - align
	;;    ESP[4]  - phase
	;;    ESP[8]  - nocross
	;;    ESP[12] - minaddr
	;;    ESP[16] - macaddr
	;;    ESP[20] - addrp 
	;;
_vmem_segfit:
	xor eax, eax
	ret

	;; Function: _vmem_add
	;;
	;; In:
	;;    EAX    - Pointer to <Vmem> object.
	;;    EDX    - Address.
	;;    ECX    - Size in bytes.
	;;    ESP[4] - Flags. (<VMEM_VM_SLEEP> or <VMEM_VM_NOSLEEP>)
	;;    ESP[8] - Span type (<VMEM_SEGMENT_SPAN> or <VMEM_SEGMENT_STATIC_SPAN>)
	;;
	;; Out:
	;;    EAX - 0 on success. 
_vmem_add:
	push ebp
	mov ebp, esp
	sub esp, 8

	push esi
	push edi
	push ebx

	mov esi, eax
	mov edi, edx
	mov ebx, ecx

	; create new span
	call _vmem_segalloc
	or eax, eax
	jz .end ; TODO: error handling

	mov [eax + VmemSegment.base], edi
	mov [eax + VmemSegment.size], ebx
	mov edx, [ebp + 12]	; get span type
	mov [eax + VmemSegment.type], edx

	mov [ebp - 8], eax
	
	; create free segment
	call _vmem_segalloc
	or eax, eax
	jz .end ; TODO: error handling

	mov [eax + VmemSegment.base], edi
	mov [eax + VmemSegment.size], ebx

	mov [eax + VmemSegment.type], VMEM_SEGMENT_FREE

	mov [ebp - 4], eax

	mov edx, [ebp - 8]
	add edx, VmemSegment.segqueue
	mov eax, esi
	add eax, Vmem.segqueue
	push eax
	push edx
	call tailq_insert_tail
	pop edx
	pop eax
	mov ecx, [ebp - 4]
	add ecx, VmemSegment.segqueue
	call tailq_insert_after

	VMEM_GET_LIST_FROM_SIZE ebx, VMEM_FREELISTS_N
	shl ebx, 2
	mov eax, esi
	add eax, Vmem.freelist
	add eax, ebx

	mov edx, [ebp - 4]
	add edx, VmemSegment.seglist
	call list_insert_head

.end:
	pop ebx
	pop edi
	pop esi

	leave
	ret

	;; Function: _vmem_import
	;;
_vmem_import:
	ret


;;; MARK: Public functions

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
	;;   ESP[24] - Flags. (<VMEM_VM_SLEEP> or <VMEM_VM_NOSLEEP>)
vmem_create:
	push ebp
	mov ebp, esp
	sub esp, 8

	push esi
	push edi

	mov [ebp - 8], eax
	mov [ebp - 4], ecx


	; zero Vmem object
	cld
	mov edi, eax
	xor eax, eax
	mov ecx, sizeof.Vmem shr 2
	cld
	rep stosd

	; copy name 
	mov esi, edx
	mov edx, [ebp - 8]
	xor cl, cl
@@:
	lodsb
	or al, al
	jz @f
	mov [edx], al
	inc edx
	inc cl
	cmp cl, 15
	jl @b
@@:

	mov edi, [ebp - 8]
	mov ecx, [ebp - 4]
	mov [edi + Vmem.base], ecx

	; copy other from stack to struct
	mov ecx, 28 shr 2
	mov esi, ebp
	add esi, 8
	mov edi, [ebp - 8]
	add edi, Vmem.size
	rep movsd

	; initialize segqueue
	mov eax, [ebp - 8]
	add eax, Vmem.segqueue
	mov [eax + 4], eax

if CONFIG_TRACE_VMEM
	mov esi, [ebp - 8]
	mov edx, [esi + Vmem.base]
	mov ecx, [esi + Vmem.size]
	add ecx, edx
	TRACE szTraceVmemCreate, esi, edx, ecx
end if

	mov eax, [ebp - 8]
	mov edx, [ebp + 24]
	or edx, edx
	jnz @f
	mov ecx, [eax + Vmem.size]
	or ecx, ecx
	jz @f
	mov edx, [ebp - 4]
	push dword [eax + Vmem.flags]
	call vmem_add
	add esp, 4
@@:

	pop edi
	pop esi

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
	push ecx
	push dword VMEM_ADDR_MAX
	push dword VMEM_ADDR_MIN
	push dword 0
	push dword 0
	xor ecx, ecx
	call vmem_xalloc
	add esp, 20
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
	;;    ESP[20] - Flags. (<VMEM_VM_BESTFIT>, <VMEM_VM_INSTANTFIT>, <VMEM_VM_SLEEP>, <VMEM_VM_NOSLEEP>)
	;;
	;; Out:
	;;    EAX - Allocated address on succes, 0 on failure.
vmem_xalloc:
	push ebp
	mov ebp, esp

	sub esp, 20

	push esi
	push edi
	push ebx

	mov esi, eax
	mov edi, edx
	mov ebx, ecx

	; refill segment if needed
	mov eax, [ebp + 24]
	and eax, VMEM_VM_POPULATING
	jz @f
	cmp [iVmemFreeSegCount], VMEM_MIN_RESERVE
	jg @f

	call _vmem_refill

@@:

	xor eax, eax
	mov [ebp - 8], eax
	mov [ebp - 4], eax 

	call _vmem_segalloc
	or eax, eax
	jz .fail
	mov [ebp - 8], eax

	call _vmem_segalloc
	or eax, eax
	jz .fail
	mov [ebp - 4], eax

	; set alignment to quantum if not set
	or ebx, ebx
	jnz @f
	mov ebx, [esi + Vmem.quantum]

	mov eax, [ebp + 24]
	and eax, VMEM_VM_FIT_MASK
	jz .fail ; TODO: error

.retry_strat:
	; get freelist
	mov edx, edi
	VMEM_GET_LIST_FROM_SIZE edx, VMEM_FREELISTS_N
	shl edx, 2

	mov eax, esi
	add eax, Vmem.freelist
	add eax, edx
	mov [ebp - 12], eax

.retry:
	mov eax, [ebp + 24]
	and eax, VMEM_VM_INSTANTFIT
	jz .best_fit

	; if not power of two use next
	mov eax, edi
	mov edx, eax
	dec edx
	and eax, edx
	jz @f
	add dword [ebp - 12], 4
@@:
	mov eax, esi
	add eax, Vmem.hashtable
	cmp [ebp - 12], eax
	jge .next

	mov eax, [ebp - 12]
	mov eax, [eax]
	or eax, eax
	jz .instant_fit_next

	sub eax, VmemSegment.seglist
	; save segment
	mov [ebp - 16], eax

	; block found
	lea edx, [ebp - 20]
	push edx ; &start
	push dword [ebp + 20] ; maxaddr
	push dword [ebp + 16] ; min addr
	push dword [ebp + 12] ; nocross 
	push dword [ebp + 8]  ; phase
	mov edx, edi
	mov ecx, ebx
	call _vmem_segfit
	add esp, 20
	or eax, eax
	jz .found

.instant_fit_next:
	add dword [ebp - 12], 4
	jmp @b

.best_fit:
	; iterate each item in list
	xchg bx, bx
	mov eax, [ebp - 12]
.best_fit_loop:
	mov eax, [eax]
	or eax, eax
	jz .best_fit_next
	sub eax, VmemSegment.seglist
	mov [ebp - 16], eax

	cmp edi, [eax + VmemSegment.size]
	jge @f

	; block found
	lea edx, [ebp - 20]
	push edx ; &start
	push dword [ebp + 20] ; maxaddr
	push dword [ebp + 16] ; min addr
	push dword [ebp + 12] ; nocross 
	push dword [ebp + 8]  ; phase
	mov edx, edi
	mov ecx, ebx
	call _vmem_segfit
	add esp, 20
	or eax, eax
	jz .found

@@:
	mov eax, [ebp - 16]
	add eax, VmemSegment.seglist.next
	jmp .best_fit_loop
.best_fit_next:
	add dword [ebp - 12], 4
	mov eax, esi
	add eax, Vmem.hashtable
	cmp [ebp - 12], eax
	jl .best_fit
.next:
if 0
	; NetBSD does this
	; https://github.com/NetBSD/src/blob/trunk/sys/kern/subr_vmem.c#L1296-L1301
	mov eax, [ebp + 24]
	and eax, VMEM_VM_INSTANTFIT
	jz @f
	or eax, VMEM_VM_BESTFIT
	mov edx, VMEM_VM_INSTANTFIT
	not edx
	and eax, edx 
	jmp .retry_strat
@@:
end if

	; return error and free allocated segment if needed 
.fail:
	mov eax, [ebp - 8]
	or eax, eax
	jz @f
	call _vmem_segfree
@@:
	mov eax, [ebp - 4]
	or eax, eax
	jz @f
	call _vmem_segfree
@@:
	xor eax, eax
	mov edx, ENOMEM
	jmp .end

.found:
	; remove seg from freelist
	mov eax, [ebp - 16]
	add eax, VmemSegment.seglist
	call list_remove

	; split left
	mov eax, [ebp - 16]
	mov eax, [eax + VmemSegment.base]
	cmp eax, [ebp - 20]
	je @f

	mov edx, [ebp - 8]
	mov dword [edx + VmemSegment.type], VMEM_SEGMENT_FREE
	mov [edx + VmemSegment.base], eax
	mov ecx, [ebp - 20]
	sub ecx, eax
	mov [edx + VmemSegment.size], ecx
	mov eax, [ebp - 16]
	mov dword [eax + VmemSegment.base], [ebp - 20]
	mov edx, [eax + VmemSegment.size]
	sub edx, ecx
	mov [eax + VmemSegment.size], edx

	; insert to freelist
	VMEM_GET_LIST_FROM_SIZE ecx, VMEM_FREELISTS_N
	shl ecx, 2
	mov eax, esi
	add eax, Vmem.freelist
	add eax, ecx

	mov edx, [ebp - 8]
	add edx, VmemSegment.seglist
	call list_insert_head

	mov dword [ebp - 8], 0
@@:

	; split right


.end:
	pop ebx
	pop esi
	pop edi
	pop ebx

	leave
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
	;;    ESP[4] - Flags. (<VMEM_VM_SLEEP> or <VMEM_VM_NOSLEEP>)
	;;
	;; Out:
	;;    EAX - Return Address on success, 0 on failure.
vmem_add:
	push ebp
	mov ebp, esp

if CONFIG_TRACE_VMEM
	sub esp, 12
	mov [ebp - 12], eax
	mov [ebp - 8], edx
	mov [ebp - 4], ecx
end if

	push edi
	; save flag to edi
	mov edi, [ebp + 8]

	push dword VMEM_SEGMENT_STATIC_SPAN
	push edi
	call _vmem_add
	add esp, 8

	pop edi

if CONFIG_TRACE_VMEM
	mov edx, [ebp - 8]
	mov ecx, [ebp - 4]
	add edx, ecx

	TRACE szTraceVmemAdd, [ebp - 12], [ebp - 8], edx
end if

	leave
	ret

	;; Function: vmem_size
	;;
	;; Return information about arenas size
	;;
	;; In:
	;;    EAX - Pointer to <Vmem> object.
	;;    EDX - Type mask.
	;;
	;; Out:
	;;    EAX - Free/allocated size in arena.
vmem_size:
	xchg edx, eax
	and eax, (VMEM_ALLOC or VMEM_FREE)
	cmp eax, VMEM_ALLOC
	jne @f
	mov eax, [edx + Vmem.in_use]
	ret
@@:
	cmp eax, VMEM_FREE
	mov eax, [edx + Vmem.size]
	jne @f
	mov edx, [edx + Vmem.in_use]
	sub eax, edx
	ret
@@:
	ret

if CONFIG_TRACE_VMEM
	;; Function: vmem_dump
	;;
vmem_dump:
	ret
end if

	;; Function: vmem_bootstrap
	;;
	;; Initialize static segments
	;; 
vmem_bootstrap:
	push ebp
	mov ebp, esp

if CONFIG_TRACE_VMEM
	TRACE szTraceVmemBootstrap
end if

	push ebx

	xor ebx, ebx
@@:
	mov eax, aVmemStaticSegs
	add eax, ebx

	call _vmem_segfree

	add ebx, sizeof.VmemSegment
	cmp ebx, VMEM_STATIC_SEG_COUNT*sizeof.VmemSegment
	jl @b

	pop ebx
	leave
	ret

;;; MARK: Variables

	;; Variable: aVmemStaticSegs
aVmemStaticSegs   db VMEM_STATIC_SEG_COUNT*sizeof.VmemSegment dup(0)
	;; Variable: pVmemFreeSegHead
pVmemFreeSegHead  dd 0
	;; Variable: iVmemFreeSegCount
iVmemFreeSegCount dd 0

szVmemSegmentAllocated db "allocated", 0
szVmemSegmentFree      db "free", 0
szVmemSegmentSpan      db "span", 0

	;; Group: Debug
if CONFIG_TRACE_VMM
szTraceVmemBootstrap db "Trace(VMEM): Bootstrap vmem", 0
szTraceVmemCreate    db "Trace(VMEM): Create %s [%x, %x)", 0
szTraceVmemAdd       db "Trace(VMEM): %s: Add span [%x, %x)", 0
szTraceVmemDestroy   db "Trace(VMEM): Destroy %s", 0
end if
