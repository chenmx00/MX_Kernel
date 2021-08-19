# MX Kernel

### Features

- Grub and Multiboot bootloader supported.

- Complete support for 5 entries GDT(null, kernel code, kernel data, user code, user data) on x86.

- Complete support for 256 entries IDT(32 system interrupts and 16 PIC interrupts) on x86.

- Complete support for Modern Keyboards input.

- Complete support for 4096 bytes paging, dynamical memory management with heap.

- Modern file system(VFS) and initrd is implemented.

- Useful shell to interact with the system.




  ![](https://github.com/chenmx00/MX_Kernel/blob/master/img/version%200.08.png)

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

- Then, simply run Make

  ```makefile
  make
  ```

### Run Kernel in QEMU

```shell
qemu-system-x86_64 -kernel kernel
```

