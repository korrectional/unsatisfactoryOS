#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>
#include "../utils/sysmem.h"
#include "../utils/sysio.h"
#include "../utils/sysstr.h"
#include "../utils/font8x8.h"
#include "../utils/syscmd.h"
#include "../drivers/keyboard.h"
#include "../drivers/vga_video_mode.h"



#define WHITE 0xffffff
#define BLACK 0x000000

__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(3);
__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};
__attribute__((used, section(".limine_requests_start")))
static volatile LIMINE_REQUESTS_START_MARKER;
__attribute__((used, section(".limine_requests_end")))
static volatile LIMINE_REQUESTS_END_MARKER;




char commandBuffer[64];
int commandBuffer_length = 0;




void clean_commandBuffer(){
    for(int i = 0; i < commandBuffer_length; i++){
        commandBuffer[i] = 0x0;
    }
    commandBuffer_length = 0;
}



int shellX = 0;
int shellY = 0;
int shellSizeX = 2;
int shellSizeY = 2;

void print(char str[]){
    int len = strlen(str);

    
    for(int i = 0; i < len; i++){
        if(str[i] == '\n'){
            shellY+=8;
            shellX = 0;
            clean_commandBuffer();
            continue;
        }

        drawLetter(font8x8[str[i]], 1 + shellX, 1 + shellY, shellSizeX, shellSizeY);
        shellX += 8;
        if((shellX/8) >= 64){
            shellY+=8;
            shellX = 0;
            commandBuffer_length = 0;
        }
    }
}


void clearShell(){
    clearScreen(BLACK);
    shellX = 0;
    shellY = 0;
    clean_commandBuffer();
}


char oldkey[] = {0x0};
unsigned int backspaceTimer = 0;
bool backspaceIceBreak = 0;

void kmain(void) {
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        hcf();
    }
    if (framebuffer_request.response == NULL
     || framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }
    framebuffer = framebuffer_request.response->framebuffers[0];
    initKeyboard();

    clearScreen(BLACK);
    cmd_welcome();
    
    while(true){
        char key[] = {keyIn()};
        
        if(key[0] == 0x0 && key[0] != oldkey[0]){
            oldkey[0] = 0x0; // setting oldKey without entering the switch
            //clearScreen(BLACK);
        }
        
        if(key[0] != 0x0 && key[0] != oldkey[0]){
            
            switch(key[0]){
                
                case 0x8:
                    print("backspace");
                    break;
                
                case 0x1c: // enter
                    if(findCommand(commandBuffer, commandBuffer_length)!=5) print("\n");
                    clean_commandBuffer();
                    print(commandBuffer);
                    break;
                    
                default:
                    commandBuffer[commandBuffer_length] = key[0];
                    commandBuffer_length ++;
                    print(key);
                    break;
            }
            
            
            oldkey[0] = key[0];
        }
        
    }


    
    hcf();
}
