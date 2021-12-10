#include "os32.h"



void idle();



struct Process  proc[PROC_COUNT + 1];
struct Process *idle_proc, *prev_proc, *cur_proc;



void init_scheduler()
{
    memset(proc, 0, sizeof(proc));

    // Initialize data for the system idle process

    proc[0].cs              = 0x08;
    proc[0].ds = proc[0].es = proc[0].fs = proc[0].gs = proc[0].ss
                            = 0x10;
    proc[0].esp             = 0x00010000;
    proc[0].eflags          = 0x200;
    proc[0].eip             = idle;

    // Initialize data for user processes

    for (unsigned short i = 0; i < PROC_COUNT; i++) {
        proc[i+1].cs        = ((PROC_SEG_IDX_BASE + i*2 + 0) * 8) | 3;
        proc[i+1].ds = proc[i+1].es = proc[i+1].fs = proc[i+1].gs = proc[i+1].ss
                            = ((PROC_SEG_IDX_BASE + i*2 + 1) * 8) | 3;
        proc[i+1].esp       = PROC_SEG_SIZE;
        proc[i+1].eflags    = 0x200;
        proc[i+1].eip       = 0x0;
    }

    idle_proc = proc;
    cur_proc = prev_proc = 0x0;
}



void schedule_next_proc()
{
    // print("scheduling...");

    if (cur_proc == idle_proc)
        cur_proc = prev_proc;

    prev_proc = cur_proc;

    if (cur_proc == 0x0)
        cur_proc = proc + 1;
    else {
        cur_proc++;
        if (cur_proc - proc >= PROC_COUNT + 1)
            cur_proc = proc + 1;
    }
}



void schedule_idle_proc()
{
    prev_proc = cur_proc;
    cur_proc = idle_proc;
}



void idle()
{
    asm(".loop:");
    asm("    hlt");
    asm("    jmp .loop");
}
