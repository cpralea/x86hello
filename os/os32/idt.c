#include "os32.h"



union InterruptHandlerStub {
    void (*stub)();
    #pragma pack(1)
    struct {
        unsigned short bits15to00;
        unsigned short bits31to16;
    } bits;
};



void install_ihs(int num, void (*stub)());



struct InterruptGateDescriptor IDT[IDT_SIZE];
struct InterruptDescriptorTableRegisterValue IDTR;



void setup_idt()
{
    for (int i = 0; i < IDT_SIZE; i++)
        install_ihs(i, ihs_generic);

    install_ihs(  0, ihs_divide_error);
    install_ihs(  1, ihs_debug_exception);
    install_ihs(  2, ihs_nmi_interrupt);
    install_ihs(  3, ihs_breakpoint);
    install_ihs(  4, ihs_overflow);
    install_ihs(  5, ihs_bound_range_exceeded);
    install_ihs(  6, ihs_invalid_opcode);
    install_ihs(  7, ihs_device_not_available);
    install_ihs(  8, ihs_double_fault);
    install_ihs(  9, ihs_coprocessor_segment_overrun);
    install_ihs( 10, ihs_invalid_tss);
    install_ihs( 11, ihs_segment_not_present);
    install_ihs( 12, ihs_stack_segment_fault);
    install_ihs( 13, ihs_general_protection);
    install_ihs( 14, ihs_page_fault);
    install_ihs( 15, ihs_reserved);
    install_ihs( 16, ihs_x87_fpu_floating_point_error);
    install_ihs( 17, ihs_alignment_check);
    install_ihs( 18, ihs_machine_check);
    install_ihs( 19, ihs_simd_floating_point_exception);
    install_ihs( 20, ihs_virtualization_exception);
    install_ihs( 21, ihs_reserved);
    install_ihs( 22, ihs_reserved);
    install_ihs( 23, ihs_reserved);
    install_ihs( 24, ihs_reserved);
    install_ihs( 25, ihs_reserved);
    install_ihs( 26, ihs_reserved);
    install_ihs( 27, ihs_reserved);
    install_ihs( 28, ihs_reserved);
    install_ihs( 29, ihs_reserved);
    install_ihs( 30, ihs_reserved);
    install_ihs( 31, ihs_reserved);

    install_ihs( 32, ihs_rtc);
    install_ihs( 33, ihs_keyboard);

    install_ihs(128, ihs_syscall);
    // set DPL to 3 to make sure we can execute int 0x80 from ring 3 and invoke the handler
    IDT[128].ctrl               = 0xee00;                   // 1110_1110_0000_0000

    IDTR.limit                  = sizeof(IDT);
    IDTR.idt                    = IDT;

    asm("lidt [_IDTR]");
}



void install_ihs(int num, void (*stub)())
{
    union InterruptHandlerStub ihs;

    ihs.stub = stub;
    IDT[num].offset_bits15to00  = ihs.bits.bits15to00;
    IDT[num].segment_selector   = 0x08;
    IDT[num].ctrl               = 0x8e00;                   // 1000_1110_0000_0000
    IDT[num].offset_bits31to16  = ihs.bits.bits31to16;
}
