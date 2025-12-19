// // main.c (fixed trimmed input + snake check)
// #include "lib.h"
// #include "stdint.h"

// // declare game function implemented in game.c
// void run_game(void);

// // tiny helper: trim trailing whitespace (space, tab, CR, LF)
// static void trim_trailing(char *s) {
//     int i = 0;
//     while (s[i] != '\0') i++;
//     // i is length
//     while (i > 0) {
//         char c = s[i-1];
//         if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
//             s[i-1] = '\0';
//             i--;
//         } else break;
//     }
// }

// // very small strcmp helper (null-terminated strings)
// static int my_strcmp(const char *a, const char *b) {
//     int i = 0;
//     while (a[i] != '\0' || b[i] != '\0') {
//         if (a[i] != b[i]) return (unsigned char)a[i] - (unsigned char)b[i];
//         ++i;
//     }
//     return 0;
// }

// static int read_cmd(char *buffer) {
//     char ch[2] = { 0 };
//     int buffer_size = 0;

//     while (1) {
//         ch[0] = keyboard_readu();

//         if (ch[0] == '\n' || ch[0] == '\r' || buffer_size > 80) {
//             // echo newline/carriage (keyboard_readu returns the char)
//             printf("%s", ch);
//             break;
//         } else if (ch[0] == '\b') {
//             if (buffer_size > 0) {
//                 buffer[--buffer_size] = 0;
//                 printf("%s", ch);
//             }
//         } else {
//             buffer[buffer_size++] = ch[0];
//             // keep echoing as before
//             printf("%s", ch);
//         }
//     }

//     // ensure null-termination (defensive)
//     buffer[buffer_size] = '\0';

//     // trim trailing CR/LF/spaces just in case
//     trim_trailing(buffer);

//     return buffer_size;
// }

// int main(void) {
//     char buffer[100] = { 0 };
//     int buffer_size = 0;

//     while (1) {
//         printf("shell# ");
//         // ensure buffer is zeroed out at start
//         memset(buffer, 0, 100);
//         buffer_size = read_cmd(buffer);

//         if (buffer_size == 0) {
//             continue;
//         }

//         // If user typed "snake", run the embedded game directly (no fork/exec).
//         if (my_strcmp(buffer, "snake") == 0) {
//             run_game();
//             continue;
//         }

//         // existing behavior: try opening as file, fork/exec if found
//         int fd = open_file(buffer);
//         if (fd == -1) {
//             printf("Command Not Found\n");
//         } else {
//             close_file(fd);
//             int pid = fork();
//             if (pid == 0) {
//                 exec(buffer);
//             } else {
//                 waitu(pid);
//             }
//         }
//     }

//     return 0;
// }

// main.c (with embedded simple paddle/ball game playable via single-key input)
#include "lib.h"
#include "stdint.h"

// --- forward declarations ---
void run_game(void);   // implemented below

// tiny helper: trim trailing whitespace (space, tab, CR, LF)
static void trim_trailing(char *s) {
    int i = 0;
    while (s[i] != '\0') i++;
    // i is length
    while (i > 0) {
        char c = s[i-1];
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            s[i-1] = '\0';
            i--;
        } else break;
    }
}

// very small strcmp helper (null-terminated strings)
static int my_strcmp(const char *a, const char *b) {
    int i = 0;
    while (a[i] != '\0' || b[i] != '\0') {
        if (a[i] != b[i]) return (unsigned char)a[i] - (unsigned char)b[i];
        ++i;
    }
    return 0;
}

static int read_cmd(char *buffer) {
    char ch[2] = { 0 };
    int buffer_size = 0;

    while (1) {
        ch[0] = keyboard_readu();

        if (ch[0] == '\n' || ch[0] == '\r' || buffer_size > 80) {
            // echo newline/carriage (keyboard_readu returns the char)
            printf("%s", ch);
            break;
        } else if (ch[0] == '\b') {
            if (buffer_size > 0) {
                buffer[--buffer_size] = 0;
                printf("%s", ch);
            }
        } else {
            buffer[buffer_size++] = ch[0];
            // keep echoing as before
            printf("%s", ch);
        }
    }

    // ensure null-termination (defensive)
    buffer[buffer_size] = '\0';

    // trim trailing CR/LF/spaces just in case
    trim_trailing(buffer);

    return buffer_size;
}

int main(void) {
    char buffer[100] = { 0 };
    int buffer_size = 0;

    while (1) {
        printf("shell# ");
        // ensure buffer is zeroed out at start
        memset(buffer, 0, 100);
        buffer_size = read_cmd(buffer);

        if (buffer_size == 0) {
            continue;
        }

        // If user typed "snake", run the embedded game directly (no fork/exec).
        if (my_strcmp(buffer, "snake") == 0) {
            run_game();
            continue;
        }

        // existing behavior: try opening as file, fork/exec if found
        int fd = open_file(buffer);
        if (fd == -1) {
            printf("Command Not Found\n");
        } else {
            close_file(fd);
            int pid = fork();
            if (pid == 0) {
                exec(buffer);
            } else {
                waitu(pid);
            }
        }
    }

    return 0;
}

/* ===========================
   Embedded simple game:
   Paddle / Ball turn-based demo
   =========================== */

/*
 How it works (designed for blocking keyboard_readu()):
  - The screen is represented as a single text 'area' (a few lines).
  - You control a paddle (3 chars wide) using 'a' (left) and 'd' (right).
  - The ball moves automatically one step each "frame".
  - To advance the game one step, press Enter or any key (works like "step").
  - To move the paddle before the step, press 'a' or 'd' (single press).
  - Press 'q' at any input prompt to quit immediately and return to shell.
  - This avoids requiring non-blocking keyboard reads in the kernel.
*/

void run_game(void) {
    const int width = 40;          // playfield width
    const int height = 8;          // playfield height (lines)
    int paddle_x = width/2 - 1;    // paddle left index (3-wide paddle)
    const int paddle_w = 3;
    int ball_x = width/2;
    int ball_y = height/2;
    int ball_dx = 1;               // ball horizontal direction (±1)
    int ball_dy = -1;              // ball vertical direction (±1)
    char linebuf[128];

    // instructions
    printf("\n");
    printf("=== Simple Paddle/Ball Demo ===\n");
    printf("Controls: 'a' = left, 'd' = right, 'q' = quit, Enter/other = step\n");
    printf("Goal: keep the ball from falling past the paddle. Press a key to step.\n");
    printf("Press a key to start...\n");

    // wait for first key to start (blocking)
    char c = keyboard_readu();
    if (c == 'q') { printf("Exiting game.\n"); return; }

    // main loop: run until ball falls below paddle or user quits
    while (1) {
        // draw the playfield as text (clear-ish by printing newlines)
        // a simple method: print a header and then each line
        printf("\n\n"); // push previous content up

        // top border
        for (int i = 0; i < width + 2; ++i) linebuf[i] = '-';
        linebuf[width+2] = '\n';
        linebuf[width+3] = '\0';
        printf("%s", linebuf);

        // playfield
        for (int y = 0; y < height; ++y) {
            int pos = 0;
            linebuf[pos++] = '|';
            for (int x = 0; x < width; ++x) {
                if (x == ball_x && y == ball_y) {
                    linebuf[pos++] = 'O'; // ball
                } else if (y == height-1 && x >= paddle_x && x < paddle_x + paddle_w) {
                    linebuf[pos++] = '='; // paddle (bottom row)
                } else {
                    linebuf[pos++] = ' ';
                }
            }
            linebuf[pos++] = '|';
            linebuf[pos++] = '\n';
            linebuf[pos] = '\0';
            printf("%s", linebuf);
        }

        // bottom border
        for (int i = 0; i < width + 2; ++i) linebuf[i] = '-';
        linebuf[width+2] = '\n';
        linebuf[width+3] = '\0';
        printf("%s", linebuf);

        // status line
        printf("Paddle pos: %d  Ball: (%d,%d)  dx=%d dy=%d\n", paddle_x, ball_x, ball_y, ball_dx, ball_dy);
        printf("Press 'a'/'d' to move paddle, 'q' to quit, Enter/other to step: ");

        // read one char (blocking). This design intentionally uses a single-key step.
        char cmd = keyboard_readu();

        // echo newline for visual
        {
            char tmp[2]; tmp[0] = cmd; tmp[1] = 0;
            printf("%s", tmp);
        }

        if (cmd == 'q') {
            printf("Quitting game, returning to shell...\n");
            return;
        } else if (cmd == 'a') {
            if (paddle_x > 0) paddle_x--;
        } else if (cmd == 'd') {
            if (paddle_x + paddle_w < width) paddle_x++;
        } else {
            // any other key (including Enter) => advance the ball
        }

        // move ball
        ball_x += ball_dx;
        ball_y += ball_dy;

        // bounce off left/right walls
        if (ball_x < 0) { ball_x = 0; ball_dx = -ball_dx; }
        if (ball_x >= width) { ball_x = width-1; ball_dx = -ball_dx; }

        // bounce off top
        if (ball_y < 0) { ball_y = 0; ball_dy = -ball_dy; }

        // if ball hits paddle (bottom row)
        if (ball_y == height-1) {
            if (ball_x >= paddle_x && ball_x < paddle_x + paddle_w) {
                // reflect up; optionally change horizontal direction based on where it hit
                ball_dy = -1;
                // nudge dx depending on where it hit: left, center, right
                int hit_pos = ball_x - paddle_x; // 0,1,2
                if (hit_pos == 0) ball_dx = -1;
                else if (hit_pos == 2) ball_dx = 1;
                // if center, keep current dx
            } else {
                // missed paddle -> game over
                printf("You missed! Game over. Returning to shell.\n");
                return;
            }
        }

        // tiny delay so output doesn't flood if kernel's keyboard_readu somehow isn't blocking
        // (safe, no harm).
        sleepu(1);
    }
}
