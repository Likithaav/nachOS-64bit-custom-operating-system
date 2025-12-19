
# NachOS: Custom 64-bit Operating System

![NachOS Banner](https://img.shields.io/badge/NachOS-Custom_OS-blue)

**NachOS** is a fully custom-built 64-bit operating system developed from scratch using **C** and **x86/x86-64 Assembly**. This project demonstrates low-level hardware-software interaction, process management, memory management, device drivers, and system-level programming.

---

## Table of Contents

- [Project Overview](#project-overview)
- [Features](#features)
- [Architecture](#architecture)
- [Technologies Used](#technologies-used)
- [Setup and Usage](#setup-and-usage)
- [File System Support](#file-system-support)
- [Process Management](#process-management)
- [Device Drivers](#device-drivers)
- [Future Enhancements](#future-enhancements)
- [Contributing](#contributing)
- [License](#license)

---

## Project Overview

NachOS is designed to provide a minimal yet functional operating system environment. It includes a **bootloader**, **loader**, and a **64-bit kernel**, capable of running user processes in **ring 3 (user mode)** and handling interrupts in **ring 0 (kernel mode)**. The OS demonstrates core operating system concepts including:

- Booting a 64-bit kernel
- Memory management and paging
- Process abstraction and scheduling
- Device driver integration
- File system access via FAT16

This project serves both as a learning platform and a base for building more complex OS features.

---

## Features

- **64-bit Kernel**: Supports x86-64 long mode execution.
- **Custom Bootloader**: Initializes hardware, memory, and transitions to protected and long mode.
- **Process Management**: Basic process abstraction with ready lists, idle process, and context switching.
- **Memory Management**: Implements paging, virtual memory mapping, and memory allocation.
- **Device Drivers**: Keyboard, UART, and basic console output drivers.
- **FAT16 File System**: Ability to read files from a FAT16 disk image.
- **Cyber-Themed Boot Screen**: Custom VGA text mode animation and logo.
- **Interrupt Handling**: Timer and software interrupt handling for process scheduling.

---

## Architecture

NachOS follows a modular architecture:

```
┌─────────────────────────────────────────────┐
│              User Processes (Ring 3)        │
├─────────────────────────────────────────────┤
│               64-bit Kernel (Ring 0)        │
│  • GDT/IDT/TSS Setup                        │
│  • Process Scheduler                        │
│  • Interrupt Handlers                       │
│  • System Calls                             │
├─────────────────────────────────────────────┤
│                  Loader                     │
│  • Loads kernel & user programs             │
│  • Sets up memory and paging                │
│  • Prepares CPU for 64-bit execution        │
├─────────────────────────────────────────────┤
│               Bootloader                    │
│  • 16-bit real mode                         │
│  • CPU feature tests                        │
│  • A20 line enable                          │
│  • Protected/Long mode transition           │
└─────────────────────────────────────────────┘
```

---

## Technologies Used

- **Languages**: C, x86/x86-64 Assembly
- **Assembler**: NASM
- **Compiler**: GCC (cross-compiler recommended)
- **Emulator**: Bochs/QEMU
- **Debugger**: GDB
- **File System**: FAT16
- **Build System**: Make

---

## Setup and Usage

### Prerequisites

```bash
# Install required tools on Ubuntu/Debian
sudo apt-get install nasm gcc qemu-system-x86 bochs bochs-x

# Or for cross-compilation
sudo apt-get install gcc-multilib
```

### 1. Clone the Repository

```bash
git clone https://github.com/your-username/NachOS.git
cd NachOS
```

### 2. Build the OS

```bash
# Build using Makefile
make all
```

Or manually:
```bash
# Assemble bootloader and kernel components
nasm -f bin boot/boot.asm -o boot.bin
nasm -f elf64 kernel/kernel_entry.asm -o kernel_entry.o

# Compile C kernel files
gcc -c kernel/kernel.c -o kernel.o

# Link and create final image
ld -o kernel.bin -Ttext 0x1000 kernel_entry.o kernel.o
cat boot.bin kernel.bin > os-image.bin
```

### 3. Run in Emulator

**Using Bochs:**
```bash
bochs -f bochsrc.txt
```

**Using QEMU:**
```bash
qemu-system-x86_64 -drive format=raw,file=os-image.bin
```

### 4. Boot Process
NachOS will display a cyber-themed boot screen, initialize the kernel, and launch user processes.

---

## File System Support

NachOS supports a FAT16-based file system for:

1. **Loading Kernel**: Locating and executing `kernel.bin`
2. **Loading User Programs**: Fetching `user.bin` for execution in user mode
3. **Simple File Read Operations**: Accessing data stored on the disk image

---

## Process Management

1. **Idle Process**: Runs continuously when no other user processes are ready
2. **Ready List**: Maintains a queue of user processes prepared for execution
3. **Context Switching**: Saves and restores CPU state using TrapFrames
4. **System Calls**: Enables user processes to safely request kernel services

---

## Device Drivers

1. **Keyboard Driver**: Manages user input by buffering typed keys
2. **UART Driver**: Handles serial console communication for debugging and logs
3. **VGA Driver**: Text-mode console output with custom colors and formatting
4. **Interrupt Handling**: Both drivers utilize interrupt handlers for efficient, asynchronous operations

---

## Future Enhancements

1. **Scheduling**: Add multi-level feedback queue scheduling
2. **File System**: Implement advanced file system support (FAT32/ext2)
3. **Networking**: Integrate network drivers for basic TCP/IP support
4. **Graphics**: Improve GUI with high-resolution graphics support
5. **Multithreading**: Add support for kernel and user threads
6. **Virtual Memory**: Enhance paging with demand paging and swap

---

## Contributing

We welcome contributions to NachOS! Please follow these steps:

1. **Fork the repository** to your own account
2. **Create a new feature branch** for your changes
   ```bash
   git checkout -b feature/feature
   ```
3. **Commit your changes** with clear and descriptive messages
   ```bash
   git commit -m "Add feature"
   ```
4. **Push to your branch**
   ```bash
   git push origin feature/amazing-feature
   ```
5. **Open a Pull Request** for review and merging

### Contribution Guidelines
- Follow existing code style and conventions
- Add comments for complex logic
- Update documentation as needed
- Test your changes thoroughly

---

## License

This project is licensed under the GNU General Public License v2.0 - see the [LICENSE](LICENSE) file for details.

---

## Acknowledgments

- Inspired by educational OS projects like xv6 and Minix
- Thanks to the OSDev community for resources and guidance
- Built for learning and experimentation in operating system design

---



