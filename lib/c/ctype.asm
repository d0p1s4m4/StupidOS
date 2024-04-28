	format COFF
	use32

	public isdigit

	section '.code' code

	;; Function: isdigit
	;;
	;; Parameters:
	;;
	;;     [esp+8]  - character
	;;
	;; Returns:
	;; 
	;;     eax - 1 if digit otherwhise 0
	;;
isdigit:
	push ebp
	mov ebp, esp
	mov al, '0'
	mov cl, byte [esp+8]
	cmp al, cl
	jb @f
	mov al, '9'
	cmp al, cl
	jg @f
	mov eax, 1
	jmp .end 
@@:
	xor eax, eax
.end:
	leave
	ret
