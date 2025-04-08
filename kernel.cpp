#include "utils/sysio.h"
#include "utils/sysVGATMstr.h"
#include "drivers/keyboard.h"
#include "drivers/vga_text_mode.h"
#include "utils/syscmd.h"


float time = 0.0;


char commandBuffer[VGA_WIDTH];
int commandBuffer_length = 0;

void clean_commandBuffer(){
    for(int i = 0; i < commandBuffer_length; i++){
        commandBuffer[i] = 0x0;
    }
    commandBuffer_length = 0;
}


char oldKey[2] = {0x0};
extern "C" void main(){
    initKeyboard();
    clearScreen();
    enable_cursor(14, 15);
    cmd_welcome();
    while(1){
        char currentKey[2] = {keyIn()};
        

        if(currentKey[0] == 0x1b) break; // esc key

        
        if(currentKey[0] == 0x0 && currentKey[0] != oldKey[0]){
            oldKey[0] = currentKey[0];
        }
        if(currentKey[0] != 0x0 && currentKey[0] != oldKey[0]){
            oldKey[0] = currentKey[0];

            // special key behaviors
            switch (currentKey[0])
            {
            case 0x8:// backspace
                VIDEO_ADDRESS_OFFSET-=2;
                printOver(" ", VIDEO_ADDRESS_OFFSET/2, 1);
                VIDEO_ADDRESS_OFFSET-=2;
                commandBuffer_length--;
                commandBuffer[commandBuffer_length] = 0x0;
                update_cursor();

                break;
            case 0x1c: // enter
                print("\n");
                findCommand(commandBuffer, commandBuffer_length);
                print("\n");
                clean_commandBuffer();

            default:
                if(currentKey[0] == 0x1c) {break;}
                commandBuffer[commandBuffer_length] = currentKey[0];
                commandBuffer_length ++;
                print(currentKey);
                break;
            }

        }
    }
    return;
}






/*  

    +-----------------------------+---------+
    | welcome to unsatisfactoryOS | dumbass |
    +-----------------------------+---------+

*/