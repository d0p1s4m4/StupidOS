	;; File: hchacha.asm

CHACHA_CONST1 = 0x61707865
CHACHA_CONST2 = 0x3320646e
CHACHA_CONST3 = 0x3320646e
CHACHA_CONST4 = 0x6b206574

macro TO_LE32 {
	movzx ecx, byte [eax+1]
	sal ecx, 8
	movzx edx, byte [eax+2]
	sal edx, 16
	or edx, ecx
	movzx ecx, byte [eax]
	or edx, ecx
	movzx ecx, byte [eax+3]
	sal ecx, 24
	or ecx, edx
}

hchacha:
	push ebp
	mov ebp, esp
	sub esp, 4*4*4

	mov [ebp-64], dword CHACHA_CONST1
	mov [ebp-60], dword CHACHA_CONST2
	mov [ebp-56], dword CHACHA_CONST3
	mov [ebp-52], dword CHACHA_CONST4

	mov eax, [ebp+12]

	; key
	TO_LE32
	mov [ebp-48], ecx

	; key + 4
	add eax, 4
	TO_LE32
	mov [ebp-44], ecx

	; key + 4 * 2
	add eax, 4
	TO_LE32
	mov [ebp-40], ecx

	; key + 4 * 3
	add eax, 4
	TO_LE32
	mov [ebp-36], ecx

	; key + 16
	add eax, 4
	TO_LE32
	mov [ebp-32], ecx

	; key + 16 + 4
	add eax, 4
	TO_LE32
	mov [ebp-28], ecx

	; key + 16 + 4 * 2
	add eax, 4
	TO_LE32
	mov [ebp-24], ecx

	; key + 16 + 4 * 3
	add eax, 4
	TO_LE32
	mov [ebp-20], ecx

	; nonce
	mov eax, [ebp+16] 
	TO_LE32
	mov [ebp-16], ecx

	; nonce + 4
	add eax, 4
	TO_LE32
	mov [ebp-12], ecx

	; nonce + 8
	add eax, 4
	TO_LE32
	mov [ebp-8], ecx

	add eax, 4
	TO_LE32
	mov [ebp-4], ecx

	;; round

	;; out

	leave
	ret
