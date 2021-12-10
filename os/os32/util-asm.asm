global ___load_gdtr
___load_gdtr:
    push ebp
    mov ebp, esp

    mov eax, [ebp+8]
    sgdt [eax]

    pop ebp
    ret



global ___inb
___inb:
    push ebp
    mov ebp, esp

    mov dx, [ebp+8]
    in al, dx

    pop ebp
    ret

global ___outb
___outb:
    push ebp
    mov ebp, esp

    mov dx, [ebp+8]
    mov al, [ebp+12]
    out dx, al

    pop ebp
    ret
