extern _cur_proc
extern _memcpy



%macro istub 1
    extern _ih_%1
    global _ihs_%1
    _ihs_%1:

        ; save general purpose registers
        pusha

        ; setup OS segments

        ; cs is specified by the descriptor
        ; ss and esp are specified by the TSS
        ; we need to setup ds, es, fs, gs
        mov ax, 0x10
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax

        ; at this point, the stack looks like this
        ;   esp + 48      ss       -|
        ;   esp + 44      esp       |
        ;                 eflags    |-----  pushed by the CPU
        ;                 cs        |
        ;   esp + 32      eip      -|
        ;                 eax      -|
        ;                 ecx       |
        ;                 edx       |
        ;                 ebx       |
        ;                 esp       |-----  pushed by the stub
        ;                 ebp       |
        ;                 esi       |
        ;   esp +  0      edi      -|

        ; save process state

        mov ebx, [_cur_proc]        ; ebx <-- start of process state data

        cmp ebx, 0
        jz .ih_call

        add ebx, 16                 ; ebx <-- start of pusha & int stack frames
        mov ecx, 52                 ; ecx <-- size of pusha & int stack frames
        mov edx, esp                ; edx <-- top of stack

        push ecx                    ; 3rd param: size
        push edx                    ; 2nd param: src
        push ebx                    ; 1st param: dst
        call _memcpy                ; copy saved pusha & int stack frames to stack
        add esp, 12                 ; remove _memcpy() params from stack

    .ih_call:
        ; call handler
        call _ih_%1

        ; restore process state

        mov ebx, [_cur_proc]        ; ebx <-- start of process state data

        cmp ebx, 0
        jz .ihs_ret

        add ebx, 16                 ; ebx <-- start of saved pusha & int stack frames
        mov ecx, 52                 ; ecx <-- size of pusha & int stack frames
        sub esp, ecx                ; make space on stack for saved pusha & int stack frames
        mov edx, esp                ; edx <-- top of stack

        push ecx                    ; 3rd param: size
        push ebx                    ; 2nd param: src
        push edx                    ; 1st param: dst
        call _memcpy                ; copy saved pusha & int stack frames to stack
        add esp, 12                 ; remove _memcpy() params from stack

        mov ebx, [_cur_proc]        ; ebx <-- start of process state data

        mov ax, [ebx+12]
        mov gs, ax
        mov ax, [ebx+ 8]
        mov fs, ax
        mov ax, [ebx+ 4]
        mov es, ax
        mov ax, [ebx+ 0]
        mov ds, ax

    .ihs_ret:
        ; restore general purpose registers
        popa

        ; return
        iret
%endmacro



%macro istub_noret 1
    extern _ih_%1
    global _ihs_%1
    _ihs_%1:
        mov ax, 0x10
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        call _ih_%1
    .loop:
        hlt
        jmp .loop
%endmacro

%macro istub_err_noret 1
    extern _ih_%1
    global _ihs_%1
    _ihs_%1:
        mov ax, 0x10
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        call _ih_%1
        add esp, 4
    .loop:
        hlt
        jmp .loop
%endmacro



istub_noret         divide_error
istub_noret         debug_exception
istub_noret         nmi_interrupt
istub_noret         breakpoint
istub_noret         overflow
istub_noret         bound_range_exceeded
istub_noret         invalid_opcode
istub_noret         device_not_available
istub_err_noret     double_fault
istub_noret         coprocessor_segment_overrun
istub_err_noret     invalid_tss
istub_err_noret     segment_not_present
istub_err_noret     stack_segment_fault
istub_err_noret     general_protection
istub_err_noret     page_fault
istub_noret         x87_fpu_floating_point_error
istub_err_noret     alignment_check
istub_noret         machine_check
istub_noret         simd_floating_point_exception
istub_noret         virtualization_exception

istub_noret         reserved

istub_noret         generic

istub               rtc
istub               keyboard
istub               syscall
