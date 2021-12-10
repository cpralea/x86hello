#include "os32.h"



void __load_gdtr(struct GlobalDescriptorTableRegisterValue* gdtr);
unsigned char __inb(unsigned short port);
void __outb(unsigned short port, unsigned char val);
char hex_digit(unsigned char c);



struct SegmentDescriptor* get_gdt()
{
    struct GlobalDescriptorTableRegisterValue GDTR;
    __load_gdtr(&GDTR);
    return GDTR.gdt;
}



unsigned char inb(unsigned short port)
{
	return __inb(port);
}

void outb(unsigned short port, unsigned char val)
{
    __outb(port, val);
}

void io_wait()
{
    outb(0x0080, 0x00);
}



void memcpy(void* dst, void* src, unsigned short size)
{
    for (unsigned short i = 0; i < size; i++)
        ((char*) dst)[i] = ((char*) src)[i];
}

void memset(void* mem, unsigned char val, unsigned short size)
{
    for (unsigned short i; i < size; i++)
        ((char*) mem)[i] = val;
}



void hlt()
{
    asm("hlt");
}



char* uint2hexstr(unsigned int val, char* buf)
{
    *(buf + 10) = 0;

    for (int i = 9; i > 1; i--) {
        *(buf + i) = hex_digit(val & 0xf);
        val >>= 4;
    }

    *(buf + 1) = 'x';
    *(buf + 0) = '0';

    return buf;
}

char hex_digit(unsigned char c)
{
    c &= 0x0f;
    if (c < 10)
        return '0' + c;
    return 'a' + (c % 10);
}
