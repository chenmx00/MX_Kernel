# MX Kernel

### Kernel Build

- Linux

  ```shell
  nasm -f elf32 kernel.asm -o out/kasm.o && gcc -m32 -c kernel.c -o out/kc.o && ld -m elf_i386 -T link.ld -o kernel out/kasm.o out/kc.o
  ```

- MacOS

  First Install Cross i386-ELF toolchain.

  ```shell
  brew tap nativeos/i386-elf-toolchain 
  brew install i386-elf-binutils i386-elf-gcc
  ```

  ```shell
  nasm -f elf32 kernel.asm -o out/kasm.o && i386-elf-gcc -m32 -c kernel.c -o out/kc.o && i386-elf-ld -m elf_i386 -T link.ld -o kernel out/kasm.o out/kc.o
  ```

  

### Run Kernel in QEMU

```shell
qemu-system-i386 -kernel kernel
```

