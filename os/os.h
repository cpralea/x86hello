#ifndef __OS_H_
#define __OS_H_


/*
    GDT sized to accomodate the following descriptors:
    +   1 reserved by the system
        2 for OS code, data and stack
        1 for TSS
        2 for user process #1 code, data and stack
        2 for user process #2 code, data and stack
    -----
    =   8 in total
*/
#define GDT_SIZE 8
/*
    IDT sized to accomodate 128 + 1  descriptors
    + 128 general interrupt handlers
        1 sycall interrupt handler
    -----
    = 129 in total
*/
#define IDT_SIZE 129



#pragma pack(1)
struct SegmentDescriptor {
    unsigned short seg_lim_bits15to00;
    unsigned short base_addr_bits15to00;
    unsigned char  base_addr_bits23to16;
    unsigned short ctrl;
    unsigned char  base_addr_bits31to24;
};

#pragma pack(1)
struct GlobalDescriptorTableRegisterValue {
    unsigned short limit;
    struct SegmentDescriptor* gdt;
};



#pragma pack(1)
struct InterruptGateDescriptor {
    unsigned short offset_bits15to00;
    unsigned short segment_selector;
    unsigned short ctrl;
    unsigned short offset_bits31to16;
};

#pragma pack(1)
struct InterruptDescriptorTableRegisterValue {
    unsigned short limit;
    struct InterruptGateDescriptor* idt;
};



#pragma pack(1)
struct TaskStateSegment {
    unsigned int prev_task;
    unsigned int esp0;
    unsigned int ss0;
    unsigned int esp1;
    unsigned int ss1;
    unsigned int esp2;
    unsigned int ss2;
    unsigned int cr3;
    unsigned int eip;
    unsigned int eflags;
    unsigned int eax;
    unsigned int ecx;
    unsigned int edx;
    unsigned int ebx;
    unsigned int esp;
    unsigned int ebp;
    unsigned int esi;
    unsigned int edi;
    unsigned int es;
    unsigned int cs;
    unsigned int ss;
    unsigned int ds;
    unsigned int fs;
    unsigned int gs;
    unsigned int ldtss;
    unsigned short reserved;
    unsigned short iomap_base_addr;
};



#pragma pack(1)
struct Process {
    unsigned int ds;
    unsigned int es;
    unsigned int fs;
    unsigned int gs;

    unsigned int edi;
    unsigned int esi;
    unsigned int ebp;
    unsigned int orig_esp;
    unsigned int ebx;
    unsigned int edx;
    unsigned int ecx;
    unsigned int eax;
    unsigned int eip;
    unsigned int cs;
    unsigned int eflags;
    unsigned int esp;
    unsigned int ss;
};



extern struct SegmentDescriptor GDT[GDT_SIZE];
extern struct GlobalDescriptorTableRegisterValue GDTR;

extern struct InterruptGateDescriptor IDT[IDT_SIZE];
extern struct InterruptDescriptorTableRegisterValue IDTR;


#endif
