all: elf

elf: asm.o
	gcc -m32  -o elf asm.o

asm.o: asm.S
	nasm -f elf32 -o asm.o asm.S

clean:
	rm -f asm.o elf
	
