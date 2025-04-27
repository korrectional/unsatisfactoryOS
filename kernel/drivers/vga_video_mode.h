#ifndef VGA_VIDEO_MODE_H
#define VGA_VIDEO_MODE_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>

struct limine_framebuffer *framebuffer;

void drawPixel(int color, int x, int y){
    volatile uint32_t *fb_ptr = framebuffer->address;
    fb_ptr[x + (framebuffer->width) * y] = color;
}

void clearScreen(int color){
    volatile uint32_t *fb_ptr = framebuffer->address;
    for(int x = 0; x < framebuffer->width; x++){
        for(int y = 0; y < framebuffer->height; y++){
            fb_ptr[x + (framebuffer->width) * y] = color;
        }
    }
}



#endif