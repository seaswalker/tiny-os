#!/bin/sh

mkdir build

echo "Creating disk.img..."
bximage -mode=create -hd=10M -q disk.img

echo "Compiling..."
nasm -I include/ -o build/mbr.bin mbr.asm
nasm -I include/ -o build/loader.bin loader.asm
/usr/local/i386elfgcc/bin/i386-elf-gcc -I lib/ -I kernel/ -c -fno-builtin -o build/main.o kernel/main.c
nasm -f elf -o build/print.o lib/kernel/print.asm
nasm -f elf -o build/kernel.o kernel/kernel.asm
/usr/local/i386elfgcc/bin/i386-elf-gcc -I lib/ -I kernel/ -c -fno-builtin -o build/interrupt.o kernel/interrupt.c
/usr/local/i386elfgcc/bin/i386-elf-gcc -I lib/ -I kernel/ -c -fno-builtin -o build/init.o kernel/init.c
/usr/local/i386elfgcc/bin/i386-elf-ld -Ttext 0xc0001500 -e main -o build/kernel.bin build/main.o build/init.o build/interrupt.o build/print.o build/kernel.o

echo "Writing mbr, loader and kernel to disk..."
dd if=build/mbr.bin of=disk.img bs=512 count=1 conv=notrunc
dd if=build/loader.bin of=disk.img bs=512 count=4 seek=2 conv=notrunc
dd if=build/kernel.bin of=disk.img bs=512 count=200 seek=9 conv=notrunc

echo "Now start bochs and have fun!"
bochs -f bochsrc 
