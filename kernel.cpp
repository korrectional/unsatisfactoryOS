#define VGA_WIDTH 0x28 // weird value actually
#define VGA_WIDTH_alt 0xa0 // for some reason I first ended up with this number.

int VIDEO_ADDRESS_OFFSET = 0x0; // video related variables
char* VIDEO_POINTER = (char*)0xb8000;
int BACKROUND_COLOR = 0x8;

//function declarations
int findCommand(char command[], int length);



// hardware commands


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

static inline void outb(unsigned int port, unsigned int val) // stole this from osdev wiki
{
    __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
    /* There's an outb %al, $imm8 encoding, for compile-time constant port numbers that fit in 8b. (N constraint).
     * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
     * The  outb  %al, %dx  encoding is the only option for all other cases.
     * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}

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
    while(str[len] != '\0' || str[len] != 0x0){
        len++;
    }
    return len;
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



float time = 0.0;

void sleep(float sleep_length){ // how long to sleep in seconds
    unsigned int ticks = (unsigned int)(200000000 * sleep_length);
    while(ticks>0){
        ticks--;
    }
}

char commandBuffer[VGA_WIDTH];
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


int findCommand(char command[], int length){
    if(length == 0) {print("\nCommand not found\n"); return 1;}
    if(strCompare(command, "hello ")) {cmd_hello(); return 1;}
    if(strCompare(command, "clear ")) {cmd_clear(); return 1;}
    if(strCompare(command, "welcome ")) {cmd_welcome(); return 1;}
    if(strCompare(command, "do;")) {cmd_do(command, length); return 1;}
    if(strCompare(command, "help")) {cmd_help(command, length); return 1;}
    print("Command not found");
    return 0;
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