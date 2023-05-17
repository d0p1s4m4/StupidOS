; file: base.inc
extern log_impl

; macro: LOG
%macro LOG 1-*
%rep %0
%rotate -1
	push %1
%endrep
	push file

	call log_impl

	add esp, 4
%rep %0
	add esp, 4
%endrep
%endmacro
