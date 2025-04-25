#ifndef SYSVGATMSTR_H
#define SYSVGATMSTR_H


// functions for the VGA text mode

#include "../drivers/vga_text_mode.h"


int strlen(char str[]){
    int len = 0;
    while(str[len] != '\0' || str[len] != 0x0){
        len++;
    }
    return len;
}

void print(char str[]){
    int len = strlen(str);
    for(int i = 0; i < len; i++){
        if(str[i] == '\n'){
            VIDEO_ADDRESS_OFFSET += VGA_WIDTH_alt - (VIDEO_ADDRESS_OFFSET % VGA_WIDTH_alt);
            continue;
        }
        VIDEO_POINTER[VIDEO_ADDRESS_OFFSET] = (str[i]);
        VIDEO_POINTER[VIDEO_ADDRESS_OFFSET+1] = (BACKROUND_COLOR<<4)|(0xf);
        VIDEO_ADDRESS_OFFSET+=2;
    }
    update_cursor();
}

// print values
void printf(int value){
    // length
    int len = 0;
    int tmp = value;
    char str[10];
    while(tmp > 0){
        len++;
        tmp /= 10;
    }
    tmp = value;
    for(int i = len-1; i >= 0; i--){
        str[i] = '0' + (tmp % 10);
        tmp /= 10;
    }

    for(int i = 0; i < len; i++){
        VIDEO_POINTER[VIDEO_ADDRESS_OFFSET] = (str[i]);
        VIDEO_POINTER[VIDEO_ADDRESS_OFFSET+1] = (BACKROUND_COLOR<<4)|(0xf);
        VIDEO_ADDRESS_OFFSET+=2;
    }
}


void printOver(char str[], unsigned int offset, int length){
    int len = strlen(str);
    VIDEO_ADDRESS_OFFSET = offset*2;
    for(int i = 0; i < len; i++){
        if(str[i] == '\n'){
            VIDEO_ADDRESS_OFFSET += VGA_WIDTH_alt - (VIDEO_ADDRESS_OFFSET % VGA_WIDTH_alt);
            continue;
        }
        VIDEO_POINTER[VIDEO_ADDRESS_OFFSET] = (str[i]);
        VIDEO_POINTER[VIDEO_ADDRESS_OFFSET+1] = (BACKROUND_COLOR<<4)|(0xf);
        VIDEO_ADDRESS_OFFSET+=2;
    }
}

void printfOver(int value, unsigned int offset, int length){
    VIDEO_ADDRESS_OFFSET = offset*2;
    // length
    int len = 0;
    int tmp = value;
    char str[10];
    while(tmp > 0){
        len++;
        tmp /= 10;
    }
    tmp = value;
    for(int i = len-1; i >= 0; i--){
        str[i] = '0' + (tmp % 10);
        tmp /= 10;
    }

    for(int i = 0; i < len; i++){
        VIDEO_POINTER[VIDEO_ADDRESS_OFFSET] = (str[i]);
        VIDEO_POINTER[VIDEO_ADDRESS_OFFSET+1] = (BACKROUND_COLOR<<4)|(0xf);
        VIDEO_ADDRESS_OFFSET+=2;
    }

    //if(len < length) clearArea((len+1)+offset, (length)+offset);
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





#endif