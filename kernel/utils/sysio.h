/*
Am I familiar with the fact that I am using a .h file incorrectly?
YES
Why did I do it?
because I have no idea how to proprely link stuff
*/
#ifndef SYSIO_H
#define SYSIO_H


void outd(short port, int dword);
char inb(short port);
inline void outb(unsigned int port, unsigned int val); // stole this from osdev wiki
void hcf(void);

#endif