#ifndef SYSCMD_H
#define SYSCMD_H

#include "sysio.h"
#include "sysVGATMstr.h"
#include "math.h"
#include "../drivers/keyboard.h"
#include "../drivers/vga_text_mode.h"

// gotta predefine this one
int findCommand(char command[], int length);




// here are the commands that exist as utilities to be used directly in code
void sleep(float sleep_length){ // how long to sleep in seconds
    unsigned int ticks = (unsigned int)(200000000 * sleep_length);
    while(ticks>=0){
        ticks--;
    }
}










// here are defined all the commands in unsatisfactoryOS!!!

void cmd_hello(){
    print("Wasup dumass");
}
void cmd_clear(){
    clearScreen();
}
void cmd_welcome(){
    print("\n+-----------------------------+------------------------+\n");
    print("| welcome to unsatisfactoryOS | The KERNEL has arrived |\n");
    print("+-----------------------------+------------------------+\n");
    print("Type [help] to get help\n\n");
}

void cmd_do(char command[], int length){
    int command_count = 0;
    for(int i = 0; i < length; i++){ // we go though the whole command
        if(command[i-1]==';'){
            char commandBuffer_do[length-i];
            int commandBuffer_do_length = 0;
            for(int j = i; j < length; j++){ // circle through the current command
                if(command[j]!=' ' && command[j]!=';'){
                    commandBuffer_do[commandBuffer_do_length] = command[j];
                    commandBuffer_do_length++;
                }
                if(command[j]==';'){ // reached the end of the command, time to execute!
                    if(findCommand(commandBuffer_do, commandBuffer_do_length)){
                        command_count++;
                    }
                    break;
                }
            }
        }
    }
    if(command_count == 0){
        print("No commands found\n");
    }
}
char cmd_help_hello[] = "Says hi!\n";
char cmd_help_clear[] = "Clears the screen\n";
char cmd_help_welcome[] = "Displays welcome text\n";
char cmd_help_do[] = "Execute commands in a row. \nFor example, (do; write; welcome;) will execute the write and welcome commands. \nIt's important to strictly follow the format: [do;]+(space)+COMMAND_NAME+[;]\n";
char cmd_help_help[] = "lol, you're seriously asking\n";
void cmd_help(char command[], int length){
    if(length<=5) print("\nCommands:\nhello\nwelcome\ndo;\nclear\nFor additional information type help COMMAND_NAME\n");
    if(length>5){
        char commandBuffer_help[length-5];
        for(int i = 5; i < length; i++){
            commandBuffer_help[i-5] = command[i];
        }
        //print(commandBuffer_help);
        if(strCompare(commandBuffer_help, "hello ")) {print(cmd_help_hello); return;}
        if(strCompare(commandBuffer_help, "clear ")) {print(cmd_help_clear); return;}
        if(strCompare(commandBuffer_help, "welcome ")) {print(cmd_help_welcome); return;}
        if(strCompare(commandBuffer_help, "do;")) {print(cmd_help_do); return;}
        if(strCompare(commandBuffer_help, "help")) {print(cmd_help_help); return;}
    }
}
void cmd_sleep(char command[], int length){ // how long to sleep in seconds THIS COMMAND IS A WORK IN PROGRESS
    int time2sleep = 0;
    for(int i = 6; i < length; i++){ // itenerate though the size of the number
        time2sleep += (command[i] - '0') * powi(10, length-i-1);
    }
    print("sleeping for: ");
    printf(time2sleep);
    sleep(time2sleep);
}


// this tries to find the command after it is typed
int findCommand(char command[], int length){
    if(length == 0) {print("\nCommand not found\n"); return 1;}
    if(strCompare(command, "hello ")) {cmd_hello(); return 1;}
    if(strCompare(command, "clear ")) {cmd_clear(); return 1;}
    if(strCompare(command, "welcome ")) {cmd_welcome(); return 1;}
    if(strCompare(command, "do;")) {cmd_do(command, length); return 1;}
    if(strCompare(command, "help")) {cmd_help(command, length); return 1;}
    if(strCompare(command, "sleep ")) {cmd_sleep(command, length); return 1;}
    print("\nCommand not found\n");
    return 0;
}










#endif