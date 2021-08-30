UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
CC = gcc
SOURCES=./src/boot.o ./src/monitor.o ./src/common.o  ./src/kmain.o ./src/descriptor_tables.o ./src/descriptor_tables_asm.o ./src/isr.o ./src/interrupt.o ./src/keyboard.o ./src/timer.o ./src/kheap.o ./src/paging.o ./src/ordered_array.o ./src/fs.o ./src/initrd.o
CFLAGS=-m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector
LDFLAGS=-m elf_i386 -T ./src/link.ld
ASFLAGS=-f elf32

all: $(SOURCES) link
	 $(shell cp kernel isodir/boot/kernel)
	 $(shell grub-mkrescue -o mx_kernel.iso isodir)
clean:
	-rm ./src/*.o kernel ./isodir/boot/kernel core.iso
link:
	ld $(LDFLAGS) -o kernel $(SOURCES)

.s.o:
	nasm $(ASFLAGS) $<
else
CC = i386-elf-gcc
SOURCES=./src/boot.o ./src/monitor.o ./src/common.o  ./src/kmain.o ./src/descriptor_tables.o ./src/descriptor_tables_asm.o ./src/isr.o ./src/interrupt.o ./src/keyboard.o ./src/timer.o ./src/kheap.o ./src/paging.o ./src/ordered_array.o ./src/fs.o ./src/initrd.o
CFLAGS=-m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector
LDFLAGS=-m elf_i386 -T ./src/link.ld
ASFLAGS=-f elf32

all: $(SOURCES) link 
	 $(shell cp kernel isodir/boot/kernel)
	 $(shell grub-mkrescue -o core.iso isodir)

clean:
	-rm ./src/*.o kernel ./isodir/boot/kernel core.iso
link:
	i386-elf-ld $(LDFLAGS) -o kernel $(SOURCES)

.s.o:
	nasm $(ASFLAGS) $<
endif

