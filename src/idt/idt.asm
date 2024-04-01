section .asm

; handler functions implemented in C code
extern int21_handler
extern no_interrupt_handler

; initialize 21h interrupt which is keyboard interrupt
global init21h

; no interrupt routine
global no_interrupt

; load interrupt descriptor table via C files. it is global to be seen by C files
; Notice the first parameter to this function will be idt address
global idt_load

; enable and disable interrupts
global enable_interrupts
global disable_interrupts

enable_interrupts:
    ;Enable the interrupts
    sti
    ret

disable_interrupts:
    cli
    ret

idt_load:
    push ebp        ;push the base pointer
    
    mov ebp, esp    ;move stack pointer to base pointer 
    mov ebx, [ebp+8]    ; 8 means first argument passed to this function
                        ;just for note -> 
                            ; 0 offset: base pointer itself, 
                            ; 4 offset: return address of the function caller who called us
    lidt [ebx]         ; load interrupt descriptor table 

    pop ebp     ;   pop the base pointer
    ret

; keyboard interrupt
init21h:
    cli     ; clear interrupts
    pushad  ; push all general purpose register

    call int21_handler

    popad   ; pop all general purpose register
    sti     ; enable interrupts
    iret    ; interrupt return

no_interrupt:
    pushad
    call no_interrupt_handler
    popad
    iret