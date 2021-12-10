global _enter_protected_mode
extern _GDTR



bits 16



_enter_protected_mode:
    ; disable maskable interrupts
    cli
    ; disable non-maskable interrupts
    ; ...

    ; load GDT into 'gdtr' register
    ; rely on the fact the C compiler defines the _GDTR symbol to denote the GDTR variable
    lgdt [_GDTR]

    ; enter protected mode
    mov eax, cr0
    or eax, 0x00000001
    mov cr0, eax

    ; configure code area to use the 2nd Segment Descriptor in GDT
    ; and jump to the 32-bit OS entry sequence
    jmp 0x08:os32enter



bits 32



os32enter:
    ; configure data area to use the 3rd Segment Descriptor in GDT
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; configure stack area to use the 3rd Segment Descriptor in GDT
    mov ax, 0x10
    mov ss, ax
    mov esp, 0x00010000

    ; call the OS proper (far call based on 2nd Segment Descriptor)
    call 0x08:0x1000

    ; we are never to return here...


times 512 - ($-$$) db 0
