#include "os16.h"



void init()
{
    hide_cursor();
    setup_gdt();
    enter_protected_mode();
}
