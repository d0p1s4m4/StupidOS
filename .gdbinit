set disassembly-flavor intel

add-symbol-file .build/syms/stpdldr.sym
add-symbol-file .build/syms/vmstupid.sym

target remote localhost:1234
