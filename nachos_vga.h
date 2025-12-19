// nachos_vga.h
#pragma once
#include <stdint.h>
#include <stddef.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_BUFFER ((volatile uint16_t*)0xB8000)

static inline void vga_putxy(int x, int y, char ch, uint8_t attr) {
    if (x < 0 || x >= VGA_WIDTH || y < 0 || y >= VGA_HEIGHT) return;
    size_t idx = (size_t)y * VGA_WIDTH + (size_t)x;
    VGA_BUFFER[idx] = (uint16_t)((attr << 8) | (uint8_t)ch);
}

static inline void vga_clear(uint8_t attr) {
    for (int y=0;y<VGA_HEIGHT;y++){
        for (int x=0;x<VGA_WIDTH;x++){
            vga_putxy(x,y,' ',attr);
        }
    }
}
