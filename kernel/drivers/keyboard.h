#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../utils/sysio.h"

unsigned char symbolTable[256];
void initKeyboard(){

    for (int i = 1; i <= 9; i++) {
        symbolTable[i + 1] = '0' + i;
    }
    symbolTable[11] = '0';
    symbolTable[16] = 'q'; symbolTable[17] = 'w'; symbolTable[18] = 'e'; symbolTable[19] = 'r'; 
    symbolTable[20] = 't'; symbolTable[21] = 'y'; symbolTable[22] = 'u'; symbolTable[23] = 'i'; 
    symbolTable[24] = 'o'; symbolTable[25] = 'p'; symbolTable[26] = '['; symbolTable[27] = ']';

    symbolTable[30] = 'a'; symbolTable[31] = 's'; symbolTable[32] = 'd'; symbolTable[33] = 'f'; 
    symbolTable[34] = 'g'; symbolTable[35] = 'h'; symbolTable[36] = 'j'; symbolTable[37] = 'k'; 
    symbolTable[38] = 'l'; symbolTable[39] = ';';

    symbolTable[44] = 'z'; symbolTable[45] = 'x'; symbolTable[46] = 'c'; symbolTable[47] = 'v'; 
    symbolTable[48] = 'b'; symbolTable[49] = 'n'; symbolTable[50] = 'm'; symbolTable[51] = ','; 
    symbolTable[52] = '.'; symbolTable[53] = '/';

    // additional keys
    symbolTable[1] = 0x1b; // esc
    symbolTable[57] = ' '; // space
    symbolTable[14] = 0x8; // backspace
    symbolTable[15] = 0x9; // tab
    symbolTable[28] = 0x1c; // enter
    symbolTable[42] = 0x2a; // left shift
    symbolTable[54] = 0x36; // right shift
    symbolTable[29] = 0x1d; // left ctrl



}




char keyIn(){
    unsigned int key;
    key = inb(0x60);
    if (key < 0x80){
        return symbolTable[key];
    }
    return 0;
}


int DEBUG_keyIn(){
    unsigned int key;
    key = inb(0x60);
    if (key < 0x80){
        return key;
    }
    return 0;
}


#endif