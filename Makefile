all: stupid.img kern.bin

stupid.img: floppy.bin kern.bin
	mkdosfs -F 12 -C $@ 1440
	dd status=noxfer conv=notrunc if=floppy.bin of=$@
	mcopy -i $@ kern.bin ::kern.sys

floppy.bin: boot/floppy.s
	nasm -fbin -o $@ $^

kern.bin: kern/kernel.s
	nasm -fbin -o $@ $^

clean:
	rm -rf floppy.bin kern.bin

fclean: clean
	rm -rf stupid.img

re: fclean all

.PHONY: all clean fclean re
