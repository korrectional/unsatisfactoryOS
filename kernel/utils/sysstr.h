#ifndef SYSSTR_H
#define SYSSTR_H

#include "../drivers/vga_video_mode.h"


#define WHITE 0xffffff
#define BLACK 0x000000


int strlen(char str[]){
    int len = 0;
    while(str[len] != '\0' || str[len] != 0x0){
        len++;
    }
    return len;
}


int strCompare(char* str1, char* str2){
    unsigned int j = 0;
    unsigned int minLen = 0;
    while(1){
        if(str1[j] == '\0' || str2[j] == '\0'){
            minLen = j;
            break;
        }
        j++;
    }
    for(int i = 0; i < minLen; i++){
        if(str1[i] != str2[i]){
            return 0;
        }
    }
    return 1;
}



void drawBlock(int color, int x, int y, int sizex, int sizey){
    volatile uint32_t *fb_ptr = framebuffer->address;
    for(int i = 0; i < sizex; i++){
        for(int j = 0; j < sizey; j++){
            fb_ptr[(x+i) + (framebuffer->width) * (y+j)] = WHITE;
        }
    }
}



void drawLetter(char *bitmap ,int x, int y, int sizex, int sizey) {
    int i,j;
    int set;

    for (i=0; i < 8; i++) {
        for (j=0; j < 8; j++) {
            set = bitmap[i] & 1 << j;
            set ? drawBlock(WHITE, (j+x)*sizex, (i+y)*sizey, sizex, sizey) : NULL;
        }
    }
}




#endif