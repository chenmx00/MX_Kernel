CC = i386-elf-gcc
SOURCES=boot.o kmain.o
CFLAGS=-m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector
LDFLAGS=-m elf_i386 -T Link.ld
ASFLAGS=-f elf32

all: $(SOURCES) link
clean:
	-rm *.o kernel
link:
	i386-elf-ld $(LDFLAGS) -o kernel $(SOURCES)

.s.o:
	nasm $(ASFLAGS) $<