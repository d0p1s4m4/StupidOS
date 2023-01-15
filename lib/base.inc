extern log_impl

%macro LOG 1
	push %1
	push file

	call log_impl

	add esp, 8
%endmacro
