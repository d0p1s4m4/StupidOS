all: stupid.img

stupid.img: floppy.bin kern.bin
	cat $^ /dev/zero | dd of=$@ bs=512 count=2880

floppy.bin: boot/floppy.s
	nasm -fbin -o $@ $^

kern.bin: kern/kernel.s
	nasm -fbin -o $@ $^

clean:
	rm floppy.bin kern.bin

fclean:
	rm stupid.img

re: fclean all

.PHONY: all clean fclean re
