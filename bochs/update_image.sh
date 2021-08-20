#!/bin/bash

sudo /sbin/losetup /dev/loop0 mx_kernel.img
sudo mount /dev/loop0 /mnt
sudo cp ../kernel /mnt/kernel
sudo cp ../initrd_generator/initrd.img /mnt/initrd
sudo umount /dev/loop0
sudo /sbin/losetup -d /dev/loop0
