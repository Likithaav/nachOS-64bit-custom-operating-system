// kernel_game.c
#include "print.h"      // write_screen, printk
#include "keyboard.h"   // read_key_buffer()
#include "stddef.h"
#include "debug.h"

// screen size used by your kernel
#define WIDTH 80
#define HEIGHT 25

static char frame[HEIGHT][WIDTH];

/* clear frame buffer */
static void clear_frame(void) {
    for (int y = 0; y < HEIGHT; ++y)
        for (int x = 0; x < WIDTH; ++x)
            frame[y][x] = ' ';
}

/* render the whole frame into a contiguous buffer and write to screen */
static void render_frame(void) {
    static char out[HEIGHT * (WIDTH + 1) + 1];
    int p = 0;
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) out[p++] = frame[y][x];
        out[p++] = '\n';
    }
    out[p] = '\0';

    /* write_screen(buf, len, color_attr) — color 0x0f (white on black) */
    write_screen(out, p, 0x0f);
}

/* tiny helper to print short messages (uses write_screen directly) */
static void print_msg(const char *s) {
    int len = 0;
    while (s[len]) len++;
    write_screen((char*)s, len, 0x0f);
}

/* busy-wait delay */
static void busy_delay(volatile unsigned long n) {
    for (volatile unsigned long i = 0; i < n; ++i) {
        __asm__ volatile("nop");
    }
}

/* Entry point called by kernel */
void start_kernel_game(void) {
    /* debug marker so you can see in printk output that game started */
    printk("kernel_game: started\n");

    clear_frame();

    int x = WIDTH / 2;
    int y = HEIGHT / 2;
    int dx = 1, dy = 1;

    for (;;) {
        /* update */
        x += dx; y += dy;
        if (x <= 0) { x = 1; dx = -dx; }
        if (x >= WIDTH - 1) { x = WIDTH - 2; dx = -dx; }
        if (y <= 0) { y = 1; dy = -dy; }
        if (y >= HEIGHT - 1) { y = HEIGHT - 2; dy = -dy; }

        /* draw */
        clear_frame();
        frame[y][x] = 'O';
        render_frame();

        /* input: blocks until a key is available via keyboard handler */
        int ch = read_key_buffer();
        if (ch == 'q' || ch == 'Q') {
            print_msg("\nKernel game: exiting...\n");
            printk("kernel_game: quitting on 'q'\n");
            break;
        } else if (ch == 'p' || ch == 'P') {
            print_msg("\nPaused. Press any key to continue...\n");
            read_key_buffer();
            print_msg("\nResuming...\n");
        }

        busy_delay(200000);
    }

    printk("kernel_game: ended\n");
}
