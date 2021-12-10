#include "os32.h"



unsigned short rtc_ticks = 0;
#define RTC_DIV 16



void ih_rtc()
{
    send_eoi();

    rtc_ticks++;
    if (rtc_ticks == RTC_DIV) {
        // print("int #32: rtc");
        rtc_ticks = 0;

        schedule_next_proc();

    }
}



void ih_keyboard()
{
    send_eoi();

    unsigned char scan_code = inb(0x60);
    if (!(scan_code & 0x80))                // ignore key release
        print("int #33: keyboard");
}



void ih_syscall()
{
    // print("int #128: syscall");

    /*
        At this point, the stack looks like this
            esp + 64        ss         -|
            esp + 60        esp         |
                            eflags      |-----  pushed by the CPU
                            cs          |
            esp + 48        eip        -|
                            eax        -|
                            ecx         |
                            edx         |
                            ebx         |
                            esp         |-----  pushed by the stub
                            ebp         |
                            esi         |
            esp + 16        edi        -|
                            ret addr   -|
                            ebp         |
                            SS          |----- courtesy of compiler
            esp +  0        GDT        -|
    */

    extern unsigned int base_addr;
    extern unsigned int stack_ptr;

    unsigned int SS;
    struct SegmentDescriptor* GDT = get_gdt();

    base_addr = stack_ptr = 0;
    asm("mov eax, [esp+60]");
    asm("mov [_stack_ptr], eax");
    asm("mov eax, [esp+64]");
    asm("mov [ebp-4], eax");

    SS &= 0x0000fff8;
    base_addr += GDT[SS/8].base_addr_bits31to24 << 24;
    base_addr += GDT[SS/8].base_addr_bits23to16 << 16;
    base_addr += GDT[SS/8].base_addr_bits15to00;

    syscall();
}



void ih_divide_error()
{
    print("int #0: divide error");
    hlt();
}

void ih_debug_exception()
{
    print("int #1: debug exception");
    hlt();
}

void ih_nmi_interrupt()
{
    print("int #2: nmi interrupt");
    hlt();
}

void ih_breakpoint()
{
    print("int #3: breakpoint");
    hlt();
}

void ih_overflow()
{
    print("int #4: overflow");
    hlt();
}

void ih_bound_range_exceeded()
{
    print("int #5: bound range exceeded");
    hlt();
}

void ih_invalid_opcode()
{
    print("int #6: invalid opcode");
    hlt();
}

void ih_device_not_available()
{
    print("int #7: device not available");
    hlt();
}

void ih_double_fault(int err)
{
    print("int #8: double fault");
    hlt();
}

void ih_coprocessor_segment_overrun()
{
    print("int #9: coprocessor segment overrun");
    hlt();
}

void ih_invalid_tss(int err)
{
    print("int #10: invalid tss");
    hlt();
}

void ih_segment_not_present(int err)
{
    print("int #11: segment not present");
    hlt();
}

void ih_stack_segment_fault(int err)
{
    print("int #12: stack segment fault");
    hlt();
}

void ih_general_protection(int err)
{
    print("int #13: general protection fault");
    char errstr[11];
    print(uint2hexstr(err, errstr));
    hlt();
}

void ih_page_fault(int err)
{
    print("int #14: page fault");
    hlt();
}

void ih_x87_fpu_floating_point_error()
{
    print("int #16: x87 fpu floating point error");
    hlt();
}

void ih_alignment_check(int err)
{
    print("int #17: alignment check");
    hlt();
}

void ih_machine_check()
{
    print("int #18: machine check");
    hlt();
}

void ih_simd_floating_point_exception()
{
    print("int #19: simd floating point exception");
    hlt();
}

void ih_virtualization_exception()
{
    print("int #20: virtualization exception");
    hlt();
}

void ih_reserved()
{
    print("int #{15,21-31}: reserved");
    hlt();
}

void ih_generic()
{
    print("int #?: generic");
}



void enable_interrupts()
{
    asm("sti");
}
