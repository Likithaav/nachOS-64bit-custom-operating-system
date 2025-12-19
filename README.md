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

1. **Bootloader (16-bit real mode)** - Tests CPU features
   - Sets up A20 line
   - Switches to protected mode and long mode

2. **Loader** - Loads kernel and user programs from FAT16
   - Sets up memory and paging
   - Prepares the CPU for 64-bit execution

3. **Kernel (64-bit)** - Sets up GDT, IDT, and TSS  
   - Implements basic process scheduling  
   - Handles hardware interrupts  
   - Provides system calls for user processes  

4. **User Processes** - Executed in ring 3  
   - Interact with kernel via system calls  
   - Can perform I/O using drivers  

---

## Technologies Used

- **Languages**: C, 8086/x86-64 Assembly  
- **Emulator**: Bochs  
- **Debugger**: GDB  
- **File System**: FAT16  
- **Booting**: Multistage bootloader  

---

## Setup and Usage

### 1. Clone the repository
```bash
git clone [https://github.com/your-username/NachOS.git](https://github.com/your-username/NachOS.git)
cd NachOS


2. Build the OS
Compile assembly and C code using nasm and gcc, then link the kernel and user programs. (Ensure you have a cross-compiler or relevant build tools installed).

3. Run in Emulator
To launch the OS using Bochs:

Bash

bochs -f bochsrc.txt
4. Boot Screen & Kernel
NachOS will display a cyber-themed boot screen.

Kernel initializes and launches user processes.

File System Support
NachOS supports a FAT16-based file system for:

Loading kernel (kernel.bin)

Loading user programs (user.bin)

Simple file read operations

Process Management
Idle Process: Runs when no other processes are ready.

Ready List: Maintains user processes ready to execute.

Context Switching: Saves and restores CPU state (TrapFrames).

System Calls: Enables user processes to request kernel services.

Device Drivers
Keyboard Driver: Buffers typed keys.

UART Driver: Handles serial console communication.

Interrupts: Both drivers utilize interrupt handlers for asynchronous operations.

Future Enhancements
Add multi-level scheduling

Implement advanced file system support (FAT32 / ext2)

Add network drivers

Improve GUI and graphics support

Contributing
Contributions are welcome! Please follow these steps:

Fork the repository

Create a feature branch

Commit your changes with clear messages

Open a pull request
