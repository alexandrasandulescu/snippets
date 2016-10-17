SOURCES = asm.S sets.S
OBJECTS = asm.o sets.o
EXECS = asm sets

all: $(EXECS)


asm: asm.o
	gcc -m32  -o $@ $<

sets: sets.o
	gcc -m32  -o $@ $<


%.o: %.S
	nasm -f elf32 -o $@ $<

clean:
	rm -f $(OBJECTS)
	
