# =========================
# Tools
# =========================
CC      = gcc
AS      = nasm
LD      = ld
OBJCOPY = objcopy

# =========================
# Flags (from your build)
# =========================
CFLAGS = -std=c99 -mcmodel=large -ffreestanding -fno-stack-protector -mno-red-zone
LDFLAGS = -nostdlib -T link.lds

# =========================
# Output files
# =========================
KERNEL_ELF = kernel
KERNEL_BIN = kernel.bin

# =========================
# Assembly sources
# =========================
ASM_SRCS = kernel.asm trap.asm lib.asm
ASM_OBJS = kernel.o trapa.o liba.o

# =========================
# C sources
# =========================
C_SRCS = \
	main.c \
	trap.c \
	print.c \
	debug.c \
	memory.c \
	process.c \
	syscall.c \
	lib.c \
	keyboard.c \
	kernel_game.c \
	file.c

C_OBJS = $(C_SRCS:.c=.o)

# =========================
# Default target
# =========================
all: $(KERNEL_BIN)

# =========================
# Assembly rules
# =========================
kernel.o: kernel.asm
	$(AS) -f elf64 -o $@ $<

trapa.o: trap.asm
	$(AS) -f elf64 -o $@ $<

liba.o: lib.asm
	$(AS) -f elf64 -o $@ $<

# =========================
# C rules
# =========================
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# =========================
# Link kernel (ELF)
# =========================
$(KERNEL_ELF): $(ASM_OBJS) $(C_OBJS)
	$(LD) $(LDFLAGS) -o $(KERNEL_ELF) \
		kernel.o main.o trapa.o trap.o liba.o \
		print.o debug.o memory.o process.o syscall.o \
		lib.o keyboard.o kernel_game.o file.o

# =========================
# Convert to raw binary
# =========================
$(KERNEL_BIN): $(KERNEL_ELF)
	$(OBJCOPY) -O binary $(KERNEL_ELF) $(KERNEL_BIN)

# =========================
# Clean
# =========================
clean:
	rm -f *.o kernel kernel.bin

.PHONY: all clean
