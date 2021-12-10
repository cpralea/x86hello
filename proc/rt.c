#include "rt.h"



void pause()
{
    unsigned int syscall_num = 0x1;
    asm("int 0x80");
}



void display(char* msg)
{
    char* _msg = msg;
    unsigned int syscall_num = 0x2;
    asm("int 0x80");
}
