#include "os16.h"



struct SegmentDescriptor GDT[GDT_SIZE];
struct GlobalDescriptorTableRegisterValue GDTR;



void setup_gdt()
{
    struct SegmentDescriptor* null  = &GDT[0];
    null->seg_lim_bits15to00        = 0x0;
    null->base_addr_bits15to00      = 0x0;
    null->base_addr_bits23to16      = 0x0;
    null->ctrl                      = 0x0;
    null->base_addr_bits31to24      = 0x0;

    struct SegmentDescriptor* code  = &GDT[1];
    code->seg_lim_bits15to00        = 0x00ff;
    code->base_addr_bits15to00      = 0x0000;
    code->base_addr_bits23to16      = 0x00;
    code->ctrl                      = 0xc09a;           // 1100_0000_1001_1010
    code->base_addr_bits31to24      = 0x00;

    struct SegmentDescriptor* data  = &GDT[2];
    *data                           = *code;
    data->seg_lim_bits15to00        = 0x03ff;
    data->ctrl                      = 0xc092;           // 1100_0000_1001_0010

    GDTR.limit                      = sizeof(GDT);
    GDTR.gdt                        = ((char*) GDT)
                                      + 0x0500;         // GDT resides inside the data segment
                                                        // which starts at 0x0500
}
