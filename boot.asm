; 16-bit program
bits 16
; essentially a boot loader
; BIOS transfers control here
org 0x7c00


start:
    ; generic initialization
    cli
    cld
    ; save boot drive number
    mov [bootdrv], dl
    ; setup stack
    mov ax, 0x7000
    mov ss, ax
    mov sp, 0xfffe

    ; call BIOS to veryfy extended operations are available
    call chkext

    ; call BIOS to read 2 sectors (1K) from drive (sectors 2 and 3)
    ; and load them at 0x0500 where the 16-bit part of the OS resides
    mov ax, 2
    mov cx, 1
    mov bx, 0x0500
    call load

    ; call BIOS to read 32 sectors (16K) from drive (sectors 4 to 35)
    ; and load them at 0x1000 where the 32-bit main part of the OS resides
    mov ax, 32
    mov cx, 3
    mov bx, 0x1000
    call load

    ; call BIOS to read 16 sectors (8K) from drive (sectors 36 to 51)
    ; and load them at 0x5000 where the task storage resides
    mov ax, 16
    mov cx, 35
    mov bx, 0x5000
    call load

    jmp run


chkext:
    mov ah, 0x41        ; operation code: check extensions present
    mov dl, [bootdrv]
    mov bx, 0x55aa
    int 0x13

    mov ax, noext
    jc abort

    ret


load:
    mov si, dap
    mov [si+2], ax
    mov [si+4], bx
    mov [si+8], cx
    mov dl, [bootdrv]
    mov ah, 0x42
    int 0x13

    mov ax, readfail
    jc abort

    ret


abort:
    call print
    hlt


print:
    mov si, ax
.loop:
    mov bx, si
    mov al, [bx]
    cmp al, 0
    je .done
    mov ah, 0x0e
    mov bx, 0x000f
    int 0x10
    inc si
    jmp .loop
.done:
    ret


run:
    ; set the Data Segment and Extra Segment to 0x0050
    ; to accomodate the address 0x0500 where we loaded the OS
    mov ax, 0x0050
    mov ds, ax
    mov es, ax

    ; enter the OS
    ; option #1: preferred; set Code Segment to 0x0050 and Program Counter to 0x0000
    ;            screws up gdb's ability to step throug the program and/or continue execution
    jmp 0x0050:0x0000
    ; option #2: leave Code Segment as is to value 0x0000 and set the Program Counter to 0x0500
    ;            keeps gdb happy
    ; jmp 0x0500


bootdrv:
    db 0
noext:
    dd 'BIOS extensions not available', 0
readfail:
    dd 'Drive read failure', 0
dap:
    db 0x10
    db 0
    times 14 db 0


    times 510 - ($-$$) db 0
    ; MBR boot signature
    dw 0xaa55
