nasm -f elf32 kernel.asm -o out/kasm.o && gcc -m32 -c kernel.c -o out/kc.o && ld -m elf_i386 -T link.ld -o kernel out/kasm.o out/kc.o

nasm -f elf32 kernel.asm -o out/kasm.o && i386-elf-gcc -m32 -c kernel.c -o out/kc.o && i386-elf-ld -m elf_i386 -T link.ld -o kernel out/kasm.o out/kc.o

