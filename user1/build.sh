#!/bin/sh
set -e

# assemble start.asm
nasm -f elf64 -o start.o start.asm

# compile C sources (main.c, game.c, print.c)
gcc -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone -c main.c print.c

# link everything (keep lib.a in the folder or provide path)


# produce raw binary for kernel loader
objcopy -O binary user user.bin

echo "Built: user (ELF) and user.bin (raw)."
