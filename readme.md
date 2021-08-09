# MX Kernel

### Features

- Grub and Multiboot bootloader supported.

- Complete support for 5 entries GDT(null, kernel code, kernel data, user code, user data) on x86.

- Complete support for 256 entries IDT(32 system interrupts and 16 PIC interrupts) on x86.

  ![version 0,05](/Users/minxingchen/Developer/MX_Kernel/version 0,05.png)

### Kernel Build

- Clean the build files first.

  ```makefile
  make clean
  ```

- If using MacOS, install cross-build i386-elf toolchain first.

  ```bash
  brew tap nativeos/i386-elf-toolchain 
  brew install i386-elf-binutils i386-elf-gcc
  ```

- If using MacOS, simply run Make

  ```makefile
  make
  ```

- If using Linux, manually build and link as follow:

  ```shell
  nasm -f elf32 kernel.asm -o out/kasm.o && gcc -m32 -c kernel.c -o out/kc.o && ld -m elf_i386 -T link.ld -o kernel out/kasm.o out/kc.o
  ```

### Run Kernel in QEMU

```shell
qemu-system-i386 -kernel kernel
```

