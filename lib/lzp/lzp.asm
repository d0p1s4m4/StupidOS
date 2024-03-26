	;; Lempel-Ziv + Prediction (a fast, efficient, and memory-use
	;; conservative compression algorithm)
	;; (paper: https://ieeexplore.ieee.org/document/488353)
	format COFF
	use32

	;; https://hugi.scene.org/online/coding/hugi%2012%20-%20colzp.htm

	include 'lzp.inc'

	LZP_HASH_ORDER = 16
	LZP_HASH_SIZE  = (1 shl LZP_HASH_ORDER)

	; hash(h, x) (h = (h << 4) ^ x)

	section '.code' code

	;; xor hash, hash
	;; xor mask, mask
	;; j = 1
	;; while (insz > 0)
	;; {
	;;    if (ecx == 8)
	;;    {
	;;       mov [out], mask
	;;       xor ecx, ecx
	;;       xor mask, mask
	;;       j = 1;
	;;    }
	;;    c = in[inpos++]
	;;    if (c == table[hash])
	;;    {
	;;        mask |= 1 << ecx
	;;    }
	;;    else
	;;    {
	;;        table[hash] = c
	;;        out[j] = c;
	;;    }
	;;    HASH(hash, c)
	;;    ecx++;
	;; }

	;; Function: lzp_compress(void *out, const void *in, int size)
	;; In:
	;;     - [esp+8]:  out buffer address (can be null)
	;;     - [esp+12]:  input buffer address
	;;     - [esp+16]: size of the input buffer
	;; Out:
	;;     - eax: size of compressed data
	param_out   equ [ebp+8]
	param_in    equ [ebp+12]
	param_insz  equ [ebp+16]
	local_buff  equ [ebp-10]
	local_inpos equ [ebp-14]
lzp_compress:
	push ebp
	mov ebp, esp
	sub esp, 14

	push ebx
	push esi
	push edi

	mov edi, param_insz
	test edi, edi
	je .exit

	mov ebx, 0
	mov esi, 0
	mov edi, 0
.loop:
	xor ecx, ecx

	cmp esi, param_insz
	je .end

	
	xor ecx, ecx
	;; fetch data
	mov eax, param_in
	movzx eax, byte [eax+esi]
	inc esi

	cmp al, byte [table + ebx]
	jne .not_in_table
	mov edx, 1
	sal edx, cl
	or ebx, edx
	
.not_in_table:
	mov byte [table + ebx], al

	inc ecx
.check:
	cmp esi, param_insz
	jb .loop
.end:
.exit:
	mov eax, edi				; return compressed data size
	;; restore esi, edi
	pop edi
	pop esi

	leave
	ret

lzp_decompress:
	push ebp
	mov ebp, esp
	mov eax, [esp+8]	; in
	mov edx, [esp+12]	; size
	xor ecx, ecx
.loop:
	cmp ecx, edx
	je .end

	inc ecx
	jmp .loop
.end:
	mov eax, ecx
	leave
	ret

	section '.data' data

table db LZP_HASH_SIZE dup(0)
