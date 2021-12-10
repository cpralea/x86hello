void hide_cursor()
{
    // do it in Real Mode, before entering Protected Mode
    // BIOS functions do not work in Protected Mode
    asm("mov ah, 0x01");
    asm("mov ch, 0x3f");
    asm("int 0x10");
}
