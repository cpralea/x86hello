#include "os32.h"



void load_proc(unsigned short i, void* mem, void* storage);
void setup_proc_segments(unsigned short i);



void load_procs()
{
    for (unsigned short i = 0; i < PROC_COUNT; i++) {
        load_proc(i, USER_SPACE_MEM_ADDR, STORAGE_MEM_ADDR);
        setup_proc_segments(i);
    }
}



void load_proc(unsigned short i, void* mem, void* storage)
{
    // Each process is defined to be PROC_MAX_SIZE bytes in size.
    // Each process has at its disposal PROC_SEG_SIZE bytes of memory.
    // Process storage area begins at 'storage'.
    // User space memory starts at 'mem'.
    // Process #i is    loaded from     storage + i * PROC_MAX_SIZE
    //            and   copied to       mem     + i * PROC_SEG_SIZE
    memcpy(
        ((char*) mem)       + (i * PROC_SEG_SIZE),
        ((char*) storage)   + (i * PROC_MAX_SIZE),
        PROC_MAX_SIZE
    );
}



void setup_proc_segments(unsigned short i)
{
    struct SegmentDescriptor* GDT = get_gdt();

    struct SegmentDescriptor* code  = &GDT[PROC_SEG_IDX_BASE + i*2 + 0];
    code->seg_lim_bits15to00        = PROC_SEG_SIZE - 1;
    code->base_addr_bits15to00      = (((unsigned int) USER_SPACE_MEM_ADDR) + i * PROC_SEG_SIZE) % 0x10000;
    code->base_addr_bits23to16      = (((unsigned int) USER_SPACE_MEM_ADDR) + i * PROC_SEG_SIZE) / 0x10000;
    code->ctrl                      = 0x40fa;           // 0100_0000_1111_1010
    code->base_addr_bits31to24      = 0x00;

    struct SegmentDescriptor* data  = &GDT[PROC_SEG_IDX_BASE + i*2 + 1];
    *data                           = *code;
    data->ctrl                      = 0x40f2;           // 0100_0000_1111_0010
}
