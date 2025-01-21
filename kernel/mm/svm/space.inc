	;; File: space
	;;

SVM_PROT_READ  = PTE_P
SVM_PROT_WRITE = PTE_W
SVM_PROT_ALL   = (SVM_PROT_READ or SVM_PROT_WRITE)

struc SvmSpaceEntry {
	.start dd ?
	.end dd ?
	.object dd ?
	.protections dd ?
	.page_dir dd ?

	.link ListEntry
}

struc SvmSpace {
	.vmem Vmem
	.entries ListHead

	; TODO: SvmStats
}

	;; Function: svm_map
svm_map:
	ret

	;; Function: svm_unmap
	;;
	;; In:
	;;   EAX - Pointer to <SvmSpace> object.
	;;   EDX - Address
svm_unmap:
	or eax, eax
	jnz @f 
	ret
@@:
	push ebp
	mov ebp, esp

	push esi
	push edi
	; lock
	; remove element
	; unlock
	; drop object ref

	pop edi
	pop esi

	leave
	ret

if CONFIG_TRACE_SVM
szSvmTraceMap db "Trace(SVM): map ", 0
szSvmTraceUnmap db "Trace(SVM): unmap %x", 0
end if

