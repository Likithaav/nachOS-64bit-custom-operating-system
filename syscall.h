// #ifndef _SYSCALL_H_
// #define _SYSCALL_H_

// #include "trap.h"

// typedef int (*SYSTEMCALL)(int64_t *argptr);
// void init_system_call(void);
// void system_call(struct TrapFrame *tf);

// #endif

// syscall.h
#ifndef _SYSCALL_H_
#define _SYSCALL_H_

#include "trap.h"

typedef int (*SYSTEMCALL)(int64_t *argptr);
void init_system_call(void);
void system_call(struct TrapFrame *tf);

// new syscall numbers
#define SYS_write           0
#define SYS_sleep           1
#define SYS_exit            2
#define SYS_wait            3
#define SYS_keyboard_read   4
#define SYS_get_total_memory 5
#define SYS_open_file       6
#define SYS_read_file       7
#define SYS_get_file_size   8
#define SYS_close_file      9
#define SYS_fork            10
#define SYS_exec            11
#define SYS_read_root_directory 12
#define SYS_game_start      13   /* NEW: start kernel game */

#endif
