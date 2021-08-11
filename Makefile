UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
CC = gcc
SOURCES=./src/boot.o ./src/monitor.o ./src/common.o  ./src/kmain.o ./src/descriptor_tables.o ./src/descriptor_tables_asm.o ./src/isr.o ./src/interrupt.o ./src/keyboard.o ./src/timer.o
CFLAGS=-m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector
LDFLAGS=-m elf_i386 -T ./src/link.ld
ASFLAGS=-f elf32

all: $(SOURCES) link
clean:
	-rm ./src/*.o kernel
link:
	ld $(LDFLAGS) -o kernel $(SOURCES)

.s.o:
	nasm $(ASFLAGS) $<
else
CC = i386-elf-gcc
SOURCES=./src/boot.o ./src/monitor.o ./src/common.o  ./src/kmain.o ./src/descriptor_tables.o ./src/descriptor_tables_asm.o ./src/isr.o ./src/interrupt.o ./src/keyboard.o ./src/timer.o
CFLAGS=-m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector
LDFLAGS=-m elf_i386 -T ./src/link.ld
ASFLAGS=-f elf32

all: $(SOURCES) link
clean:
	-rm ./src/*.o kernel
link:
	i386-elf-ld $(LDFLAGS) -o kernel $(SOURCES)

.s.o:
	nasm $(ASFLAGS) $<
endif
