// nachos_platform.cpp
#include "nachos_platform.h"
#include "nachos_vga.h"
#include <stdint.h>

// Hook into your kernel's tick counter and keyboard buffer.
// I’ll assume you have global uint64_t tick_ms that increments in PIT ISR
extern volatile uint64_t nacho_tick_ms;
extern int kernel_pop_scancode(); // return -1 if none, else scancode

void platform_init() {
    // Ensure that physical 0xB8000 is identity mapped in your page tables.
    // If not, map or run in identity mode for the region.
    vga_clear((0<<4) | 7); // bg black, fg light grey
}

void platform_draw_char(int x,int y,char ch,uint8_t fg, uint8_t bg){
    uint8_t attr = (bg << 4) | (fg & 0x0F);
    vga_putxy(x,y,ch,attr);
}

void platform_write_text(int x,int y,const char* s,int len,uint8_t fg,uint8_t bg){
    for (int i=0;i<len;i++) platform_draw_char(x+i,y,s[i],fg,bg);
}

void platform_clear(uint8_t fg,uint8_t bg){
    uint8_t a = (bg<<4) | (fg & 0x0F);
    vga_clear(a);
}

int platform_get_key(){
    int sc = kernel_pop_scancode(); // you must provide this in keyboard driver
    if (sc < 0) return -1;
    // Map scancode to ascii/virtual keys. Keep mapping minimal (arrows, WASD, space, q, etc.)
    // Example: return sc for now (engine layer will translate).
    return sc;
}

uint64_t platform_get_time_ms(){ return (uint64_t)nacho_tick_ms; }
void platform_sleep_ms(uint64_t ms){
    uint64_t start = platform_get_time_ms();
    while ((platform_get_time_ms() - start) < ms) {
        // optionally call cpu_halt() to save cycles if you have halt instruction support
        asm volatile ("hlt");
    }
}
