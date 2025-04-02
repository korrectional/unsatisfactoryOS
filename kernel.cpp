int VIDEO_ADDRESS_OFFSET = 0x0;
char* VIDEO_POINTER = (char*)0xb8000;
int BACKROUND_COLOR = 0x8;


// hardware commands

// doesnt work
static inline int interrupts_enabled() {
    unsigned long flags;
    asm volatile ( "pushf\n\t"
        "pop %0"
        : "=g"(flags)
    );
    return (flags & (1 << 9)) ? 1 : 0;
}


void outd(short port, int dword)
{
    asm("mov %1, %%eax;"
        "mov %0, %%dx;"
        "out %%eax, %%dx"
        : : "r" (port), "r" (dword)
        : "eax", "edx");
}

char inb(short port)
{
    char res;

    asm("mov %1, %%dx;"
        "in %%dx, %%al;"
        "mov %%al, %0"
        : "=r" (res)
        : "r" (port)
        : "eax", "edx");

    return res;
}

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




int strlen(char str[]){
    int len = 0;
    while(str[len] != '\0'){
        len++;
    }
    return len;
}


void clearScreen(){
    VIDEO_ADDRESS_OFFSET = 0x0;
    for(int i = 0; i < 0xa0*0xa0; i++){
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


void print(char str[]){
    int len = strlen(str);
    for(int i = 0; i < len; i++){
        if(str[i] == '\n'){
            VIDEO_ADDRESS_OFFSET += 0xa0 - (VIDEO_ADDRESS_OFFSET % 0xa0);
            continue;
        }
        VIDEO_POINTER[VIDEO_ADDRESS_OFFSET] = (str[i]);
        VIDEO_POINTER[VIDEO_ADDRESS_OFFSET+1] = (BACKROUND_COLOR<<4)|(0xf);
        VIDEO_ADDRESS_OFFSET+=2;
    }
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
            VIDEO_ADDRESS_OFFSET += 0xa0 - (VIDEO_ADDRESS_OFFSET % 0xa0);
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



float time = 0.0;

void sleep(float sleep_length){ // how long to sleep in seconds
    unsigned int ticks = (unsigned int)(200000000 * sleep_length);
    while(ticks>0){
        ticks--;
    }
}

char commandBuffer[0xa0];
int commandBuffer_length = 0;

void clean_commandBuffer(){
    for(int i = 0; i < commandBuffer_length; i++){
        commandBuffer[i] = 0x0;
    }
    commandBuffer_length = 0;
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

void cmd_hello(){
    print("Wasup dumass");
}
void cmd_clear(){
    clearScreen();
}
void cmd_welcome(){
    print("+-----------------------------+------------------------+\n");
    print("| welcome to unsatisfactoryOS | The KERNEL has arrived |\n");
    print("+-----------------------------+------------------------+\n");
}


void findCommand(char command[], int length){
    if(length == 0) {print("Command not found"); return;}
    if(strCompare(command, "hello ")) {cmd_hello(); return;}
    if(strCompare(command, "clear ")) {cmd_clear(); return;}
    print("Command not found");
}




char oldKey[2] = {0x0};
extern "C" void main(){
    initKeyboard();
    clearScreen();
    cmd_welcome();
    //print("The KERNEL has arrived\n");
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
                print("BACK");
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
        
        
        //printOver(currentKey, 40, 2);
        //printfOver(DEBUG_keyIn(), 23, 2);
        //sleep(1);
        //clearScreen();
    }
    return;
}






/*  

    +-----------------------------+---------+
    | welcome to unsatisfactoryOS | dumbass |
    +-----------------------------+---------+

*/