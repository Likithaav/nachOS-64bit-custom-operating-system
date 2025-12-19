// user_game_call.c
// user wrapper that invokes syscall index 13 to start the kernel game

#include <stdint.h>

/* This wrapper uses int $0x80. If your kernel uses a different syscall trap,
   change the int vector accordingly. */

int user_start_kernel_game(void)
{
    int ret;
    __asm__ volatile (
        "movq $13, %%rax\n\t"   /* syscall number SYS_game_start */
        "movq $0, %%rdi\n\t"    /* param_count = 0 */
        "movq $0, %%rsi\n\t"    /* argptr = NULL */
        "int $0x80\n\t"         /* trap into kernel */
        : "=a"(ret)             /* output: return value in rax -> ret */
        :                       /* no input operands */
        : "rdi", "rsi"
    );
    return ret;
}
