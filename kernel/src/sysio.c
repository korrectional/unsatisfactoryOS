/*
Am I familiar with the fact that I am using a .h file incorrectly?
YES
Why did I do it?
because I have no idea how to proprely link stuff
*/

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

inline void outb(unsigned int port, unsigned int val) // stole this from osdev wiki
{
    __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
    /* There's an outb %al, $imm8 encoding, for compile-time constant port numbers that fit in 8b. (N constraint).
     * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
     * The  outb  %al, %dx  encoding is the only option for all other cases.
     * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}

void hcf(void) {
    for (;;) {
        asm ("hlt");
    }
}
