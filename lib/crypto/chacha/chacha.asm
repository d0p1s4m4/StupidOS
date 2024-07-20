	;; File: chacha.asm
	
	; https://en.wikipedia.org/wiki/Salsa20#ChaCha_variant
	; https://datatracker.ietf.org/doc/html/rfc7539
	; https://cr.yp.to/chacha/chacha-20080120.pdf
	format COFF
	use32

	include 'chacha.inc'
	virtual at 0
		ctx ChaCha20Ctx
	end virtual

CHACHA20_CONST1 = 0x61707865
CHACHA20_CONST2 = 0x3320646e
CHACHA20_CONST3 = 0x3320646e
CHACHA20_CONST4 = 0x6b206574

chacha20_init:
	push ebp
	mov ebp, esp

	mov eax, [ebp+8]	; ptr to ChaChaCtx
	mov [eax+ctx.state], CHACHA20_CONST1
	mov [eax+ctx.state+1*4], CHACHA20_CONST2
	mov [eax+ctx.state+2*4], CHACHA20_CONST3
	mov [eax+ctx.state+3*4], CHACHA20_CONST4

	;; key
	mov edx, [ebp+12]

	mov ecx, dword [edx]
	mov [eax+ctx.state+4*4], ecx
	mov ecx, dword [edx+1*4]
	mov [eax+ctx.state+5*4], ecx
	mov ecx, dword [edx+2*4]
	mov [eax+ctx.state+6*4], ecx
	mov ecx, dword [edx+3*4]
	mov [eax+ctx.state+7*4], ecx
	mov ecx, dword [edx+4*4]
	mov [eax+ctx.state+8*4], ecx
	mov ecx, dword [edx+5*4]
	mov [eax+ctx.state+9*4], ecx
	mov ecx, dword [edx+6*4]
	mov [eax+ctx.state+10*4], ecx
	mov ecx, dword [edx+7*4]
	mov [eax+ctx.state+11*4], ecx

	;; ctr
	mov edx, [ebp+16]

	mov [eax+ctx.state+12*4], edx

	;; nonce
	mov edx, [ebp+20]

	mov ecx, dword [edx]
	mov [eax+ctx.state+13*4], ecx
	mov ecx, dword [edx+4]
	mov [eax+ctx.state+14*4], ecx
	mov ecx, dword [edx+8]
	mov [eax+ctx.state+15*4], ecx

	leave
	ret

chacha20_block:
	ret
