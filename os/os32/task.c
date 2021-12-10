#include "os32.h"



struct TaskStateSegment TSS;



void init_tss();
void setup_tss_descr();



void setup_tss()
{
    init_tss();
    setup_tss_descr();
}



void init_tss()
{
    memset(&TSS, 0, sizeof(TSS));

    TSS.ss0 = 0x10;
    TSS.esp0 = 0x0000fffc;

    TSS.iomap_base_addr = sizeof(TSS) - 1;
}



void setup_tss_descr()
{
    struct SegmentDescriptor* GDT   = get_gdt();

    unsigned short base             = (unsigned int) &TSS;
    unsigned short limit            = base + sizeof(TSS) - 1;

    struct SegmentDescriptor* tss   = &GDT[3];
    tss->seg_lim_bits15to00         = limit;
    tss->base_addr_bits15to00       = base;
    tss->base_addr_bits23to16       = 0x0;
    tss->ctrl                       = 0x00e9;           // 0000_0000_1110_1001
    tss->base_addr_bits31to24       = 0x0;

    asm("mov ax, 0x18 | 3");
    asm("ltr ax");
}
