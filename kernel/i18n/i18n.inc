struc lang_entry
	.code: resb 2
	.data: resd 1
	.next: resd 1
endstruc

struc msg_table
	.hello_world: resd 1
	.cpu_exceptions: resd 32
endstruc
