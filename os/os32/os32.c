#include "os32.h"



void run()
{
    clrscr();

    remap_pic();
    setup_idt();
    setup_tss();
    load_procs();
    init_scheduler();
    enable_interrupts();

    while(1)
        hlt();
}
