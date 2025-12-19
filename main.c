// main.c
#include "trap.h"
#include "print.h"
#include "memory.h"
#include "process.h"
#include "syscall.h"
#include "file.h"
#include "debug.h"
#include "stddef.h"

// declare kernel_game so the compiler knows its signature
extern void start_kernel_game(void);

extern char bss_start;
extern char bss_end;

void KMain(void)
{
    // clear BSS
    uint64_t size = (uint64_t)&bss_end - (uint64_t)&bss_start;
    memset(&bss_start, 0, size);

    // init subsystems
    init_idt();
    init_memory();
    init_kvm();
    init_system_call();
    init_fs();
    init_process();

    // debug: confirm init complete and that keyboard/screen should be ready
    printk("KMain: init done — about to start kernel_game()\n");

    // === Option A: run the game in-kernel (blocking) ===
    // The game will start immediately and block the kernel until you quit the game (press 'q').
    start_kernel_game();

    // If the game returns, print a debug message then halt
    printk("KMain: start_kernel_game returned — halting\n");

    for (;;) { __asm__ volatile("hlt"); }
}
