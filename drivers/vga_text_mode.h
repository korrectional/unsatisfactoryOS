#ifndef VGA_TEXT_MODE_H
#define VGA_TEXT_MODE_H

#include "../utils/sysio.h"
#define VGA_WIDTH 0x28 // weird value actually
#define VGA_WIDTH_alt 0xa0 // for some reason I first ended up with this number.

int VIDEO_ADDRESS_OFFSET = 0x0; // video related variables
char* VIDEO_POINTER = (char*)0xb8000;
int BACKROUND_COLOR = 0x8;

void enable_cursor(unsigned int cursor_start,unsigned int cursor_end)
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

void update_cursor_tool(int x, int y)
{
	unsigned int pos = y * VGA_WIDTH + x;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (unsigned int) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (unsigned int) ((pos >> 8) & 0xFF));
}

void update_cursor()
{
	unsigned int pos = VIDEO_ADDRESS_OFFSET/2;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (unsigned int) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (unsigned int) ((pos >> 8) & 0xFF));
}


void clearScreen(){
    VIDEO_ADDRESS_OFFSET = 0x0;
    for(int i = 0; i < VGA_WIDTH*(VGA_WIDTH*2); i++){
        VIDEO_POINTER[VIDEO_ADDRESS_OFFSET] = 0;
        VIDEO_POINTER[VIDEO_ADDRESS_OFFSET+1] = (BACKROUND_COLOR<<4)|(0xf);
        VIDEO_ADDRESS_OFFSET+=2;
    }
    VIDEO_ADDRESS_OFFSET = 0x0;
}

void clearArea(int offset, int length){
    VIDEO_ADDRESS_OFFSET = offset*2;
    for(int i = 0; i < length; i++){
        VIDEO_POINTER[VIDEO_ADDRESS_OFFSET] = 0;
        VIDEO_POINTER[VIDEO_ADDRESS_OFFSET+1] = (BACKROUND_COLOR<<4)|(0xf);
        VIDEO_ADDRESS_OFFSET+=2;
    }
    VIDEO_ADDRESS_OFFSET = 0x0;
}




#endif