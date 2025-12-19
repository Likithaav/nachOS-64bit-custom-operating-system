// nachos_platform.h
#pragma once
#include <stdint.h>

void platform_init();                 // call once at startup
void platform_draw_char(int x,int y,char ch,uint8_t fg, uint8_t bg);
void platform_write_text(int x,int y,const char* s,int len,uint8_t fg,uint8_t bg);
void platform_clear(uint8_t fg,uint8_t bg);
bool platform_key_pressed(int key_code); // or pop next key
int platform_get_key();               // returns -1 if none, else a virtual key
uint64_t platform_get_time_ms();      // milliseconds since boot
void platform_sleep_ms(uint64_t ms);
