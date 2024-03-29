#include "os32.h"



void remap_pic()
{
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
    io_wait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC1_DATA, PIC1_OFFSET);               // ICW2: Master PIC vector offset
	io_wait();
	outb(PIC2_DATA, PIC2_OFFSET);               // ICW2: Slave PIC vector offset
	io_wait();
	outb(PIC1_DATA, 4);                         // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	io_wait();
	outb(PIC2_DATA, 2);                         // ICW3: tell Slave PIC its cascade identity (0000 0010)
	io_wait();
 
	outb(PIC1_DATA, ICW4_8086);
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();

    outb(PIC1_DATA, 0xff);                      // mask all interrupts
	io_wait();
	outb(PIC2_DATA, 0xff);
	io_wait();

	outb(PIC1_DATA, 0xfc);                      // enable RTC and keyboard
	io_wait();
}



void send_eoi()
{
    outb(PIC2_COMMAND,PIC_EOI);
	io_wait();
    outb(PIC1_COMMAND,PIC_EOI);
	io_wait();
}
