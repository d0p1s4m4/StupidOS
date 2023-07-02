	;; File: feat.s
%include "machdep.inc"
%include "sys/i386/registers.inc"
%include "sys/i386/cpuid.inc"
%include "sys/i386/mmu.inc"

section multiboot.text

extern machdep

	;; Function: cpuid_detect
	;; check if cpuid is avaible.
	;;
	;; ripped from <osdev wiki at https://wiki.osdev.org/CPUID#Checking_CPUID_availability>
cpuid_detect:
	pushfd
	pushfd
	xor dword [esp], EFLAGS_ID
	popfd
	pushfd
	pop eax
	xor eax, [esp]
	popfd
	and eax, EFLAGS_ID
	ret

	;; Function: feat_detect
global feat_detect
feat_detect:
	push ebp
	mov ebp, esp

	call cpuid_detect
	test eax, eax
	jz .has_cpuid
	jmp .end
.has_cpuid:
	mov byte [V2P(machdep) + machinfo.has_cpuid], 1
	mov eax, CPUID_GETFEATURES
	cpuid

	;; test if PSE is available
	mov eax, edx
	and eax, CPUID_FEAT_EDX_PSE
	jz .end
	mov byte [V2P(machdep) + machinfo.has_pse], 1

	;; test if PAE is available (we won't support PAE for now)
	mov eax, edx
	and eax, CPUID_FEAT_EDX_PAE
	jz .end
	mov byte [V2P(machdep) + machinfo.has_pae], 1
.end:
	leave
	ret
