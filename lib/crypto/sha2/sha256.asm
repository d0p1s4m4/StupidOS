	;; File: sha256.asm
	;; SHA-256 cryptographic hash

	; Implementation is based on <Wikipedia's pseudocode at https://en.wikipedia.org/wiki/SHA-2#Pseudocode>
	format COFF
	use32

	section '.data' data
	; Constant: K
	; SHA-256 round constants 
K:
	dd 0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5
	dd 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174
	dd 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da
	dd 0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967
	dd 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85
	dd 0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070
	dd 0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3
	dd 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2

section '.text' code

	;; Function: sha256_compute_block
sha256_compute_block:
	push ebp
	mov ebp, esp

	leave
	ret

	;; Function: sha256_internal
	;; 
	;; Parameters:
	;;
	;;     [esp+8]  - state
	;;     [esp+12] - input buffer
	;;     [esp+16] - size of the input buffer
	;;
sha256_internal:
	push ebp
	push edi
	mov ebp, esp
	sub esp, 64+8

	mov eax, dword [ebp+16]
	xor edx, edx

	; set padlen to (len << 3)
	shld edx, eax, 3 ;  64-bit left shit
	sal eax, 3
	mov dword [ebp-(64+8)], eax
	mov dword [ebp-(64+4)], edx

.for_block:
	cmp dword [ebp+16], 64
	jb .done
	push dword [ebp+12] ; push in
	push dword [ebp+8]  ; push state
	call sha256_compute_block
	add esp, 8
	sub dword [ebp+16], 64 ; len -= 64
	add dword [ebp+12], 64 ; in += 64
	jmp .for_block
.done:

	; ensure padding buffer is full of zero
	lea edi, [ebp-64]
	mov ecx, 16
	xor eax, eax
	rep stosd

	

	pop edi
	leave
	ret

	;; Function: sha256
	;; 
	;; Parameters:
	;;
	;;     [esp+8]  - output buffer
	;;     [esp+12] - input buffer
	;;     [esp+16] - size of the input buffer
	;;
	public sha256
sha256:
	push ebp
	mov ebp, esp
	sub esp, 8*4 ; uint32_t state[8]

	; setup initial state
	mov dword [ebp-8*4], 0x6a09e667
	mov dword [ebp-7*4], 0xbb67ae85
	mov dword [ebp-6*4], 0x3c6ef372
	mov dword [ebp-5*4], 0xa54ff53a
	mov dword [ebp-4*4], 0x510e527f
	mov dword [ebp-3*4], 0x9b05688c
	mov dword [ebp-2*4], 0x1f83d9ab
	mov dword [ebp-1*4], 0x5be0cd19

	; 

	; copy state to uint8_t *out
	mov edx, dword [ebp+8]
	mov eax, dword [ebp-8*4]
	bswap eax
	mov dword [edx], eax      ; out[0] = bswap(state[0])
	mov eax, dword [ebp-7*4] 
	bswap eax
	mov dword [edx+4], eax    ; out[1] = bswap(state[1])
	mov eax, dword [ebp-6*4]
	bswap eax
	mov dword [edx+8], eax    ; out[2] = bswap(state[2])
	mov eax, dword [ebp-5*4]
	bswap eax
	mov dword [edx+12], eax   ; out[3] = bswap(state[3])
	mov eax, dword [ebp-4*4]
	bswap eax
	mov dword [edx+16], eax   ; out[4] = bswap(state[4])
	mov eax, dword [ebp-3*4]
	bswap eax
	mov dword [edx+20], eax   ; out[5] = bswap(state[5])
	mov eax, dword [ebp-2*4]
	bswap eax
	mov dword [edx+24], eax   ; out[6] = bswap(state[6])
	mov eax, dword [ebp-1*4]
	bswap eax
	mov dword [edx+28], eax   ; out[7] = bswap(state[7])
	leave
	ret
