	;; Based on <https://github.com/rdmsr/tinyvmem at tinyvm>
VM_BESTFIT    = (1 shl 0)
VM_INSTANTFIT = (1 shl 1)
VM_NEXTFIT    = (1 shl 2)
VM_SLEEP      = (1 shl 3)
VM_NOSLEEP    = (1 shl 4)
VM_BOOTSTRAP  = (1 shl 5)

FREELISTS_N = 4 * 8
HASHTABLE_N = 16

SEGMENT_ALLOCATED = 0
SEGMENT_FREE      = 1
SEGMENT_SPAN      = 2

struc VmSegment {
	.type db ?
	.imported db ?
	.base dd ?
	.size dd ?
}

struc VmObject {
	.tmp dd ?
}

struc VmPager {
	.tmp dd ?
}

struc Vmem {
	.name db 32 dup(0)
	.base dd ?
	.size dd ?
	.quantum dd ?
	.alloc dd ?
	.free dd ?
	.source dd ?
	.qcache_max dd ?
	.vmflag dd ? ;; db ?

	.segqueue dd ?
	.freelist dd FREELISTS_N dup(0)
	.hashtable dd HASHTABLE_N dup(0)
	.spanlist dd ?
}

vmem_init:
	ret
