#include "os32.h"



// We're keeping these values here, for convenience,
// to avoid passing them arround as params to all/most functions.
// It's safe to do so, since we cannot be prempted.
// Both values are set by the interrupt handler.
unsigned int base_addr;
unsigned int stack_ptr;



void pause();
void display();
void unknown();

unsigned int get_syscall_num();



void syscall()
{
    switch(get_syscall_num()) {
        case 0x1:
            // print("syscall: pause()");
            pause();
            break;
        case 0x2:
            // print("syscall: display()");
            display();
            break;
        default:
            unknown();
            break;
    }
}



void pause()
{
    schedule_idle_proc();
}



void display()
{
    unsigned int relative_msg_addr;
    relative_msg_addr = *((unsigned int*) (base_addr + stack_ptr + 4));

    print((char*) (base_addr + relative_msg_addr));
}



void unknown()
{
    print("unknown syscall number");

    char numstr[11];
    print(uint2hexstr(get_syscall_num(base_addr, stack_ptr),  numstr));

    hlt();
}



unsigned int get_syscall_num()
{
    return *((unsigned int*) (base_addr + stack_ptr));
}
