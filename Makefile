CC = i386-elf-gcc
SOURCES=./src/boot.o ./src/monitor.o ./src/common.o ./src/keyboard.o ./src/kmain.o 
CFLAGS=-m32
LDFLAGS=-m elf_i386 -T ./src/Link.ld
ASFLAGS=-f elf32

all: $(SOURCES) link
clean:
	-rm ./src/*.o kernel
link:
	i386-elf-ld $(LDFLAGS) -o kernel $(SOURCES)

.s.o:
	nasm $(ASFLAGS) $<