#ifndef __OS32_H__
#define __OS32_H__


#include "../os.h"



#define VGA_TB                  ((unsigned short*) 0xb8000)
#define VGA_TB_COL              80
#define VGA_TB_ROW              25
#define VGA_DEF_ATTRS           0x0f00      // white fg, black bg, no blink

#define USER_SPACE_MEM_ADDR     (void*) 0x00180000
#define STORAGE_MEM_ADDR        (void*) 0x00005000
#define PROC_COUNT              2
#define PROC_MAX_SIZE           0x1000
#define PROC_SEG_SIZE           0x1000
#define PROC_SEG_IDX_BASE       4

#define PIC1_OFFSET             0x20        // IO base address for master PIC
#define PIC2_OFFSET             0x28        // IO base address for slave PIC
#define PIC1_COMMAND            PIC1_OFFSET
#define PIC1_DATA               (PIC1_COMMAND + 1)
#define PIC2_COMMAND            PIC2_OFFSET
#define PIC2_DATA               (PIC2_COMMAND + 1)

#define ICW1_ICW4               0x01        // ICW4 (not) needed
#define ICW1_SINGLE             0x02		// Single (cascade) mode
#define ICW1_INTERVAL4          0x04		// Call address interval 4 (8)
#define ICW1_LEVEL              0x08		// Level triggered (edge) mode
#define ICW1_INIT               0x10		// Initialization - required!
 
#define ICW4_8086               0x01		// 8086/88 (MCS-80/85) mode
#define ICW4_AUTO               0x02		// Auto (normal) EOI
#define ICW4_BUF_SLAVE          0x08		// Buffered mode/slave
#define ICW4_BUF_MASTER         0x0C		// Buffered mode/master
#define ICW4_SFNM               0x10		// Special fully nested (not)

#define PIC_EOI                 0x20		// End-of-interrupt command code



void clrscr();
void remap_pic();
void send_eoi();
void setup_idt();
void setup_tss();
void load_procs();
void init_scheduler();
void enable_interrupts();
void hlt();
void schedule_next_proc();
void schedule_idle_proc();
void syscall();

void print(char* s);
struct SegmentDescriptor* get_gdt();
unsigned char inb(unsigned short port);
void outb(unsigned short port, unsigned char val);
void io_wait();
void memcpy(void* dst, void* src, unsigned short size);
void memset(void* mem, unsigned char val, unsigned short size);
char* uint2hexstr(unsigned int val, char* buf);



#define istub(name) \
    void ih_##name(); \
    void ihs_##name()

#define istub_err(name) \
    void ih_##name(int err); \
    void ihs_##name()

istub       (divide_error);
istub       (debug_exception);
istub       (nmi_interrupt);
istub       (breakpoint);
istub       (overflow);
istub       (bound_range_exceeded);
istub       (invalid_opcode);
istub       (device_not_available);
istub_err   (double_fault);
istub       (coprocessor_segment_overrun);
istub_err   (invalid_tss);
istub_err   (segment_not_present);
istub_err   (stack_segment_fault);
istub_err   (general_protection);
istub_err   (page_fault);
istub       (x87_fpu_floating_point_error);
istub_err   (alignment_check);
istub       (machine_check);
istub       (simd_floating_point_exception);
istub       (virtualization_exception);

istub       (reserved);

istub       (generic);

istub       (rtc);
istub       (keyboard);
istub       (syscall);


#endif
