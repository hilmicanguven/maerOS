section .asm

; load interrupt descriptor table via C files. it is global to be seen by C files
; Notice the first parameter to this function will be idt address
global idt_load
idt_load:
    push ebp        ;push the base pointer
    
    mov ebp, esp    ;move stack pointer to base pointer 
    mov ebx, [ebp+8]    ; 8 means first argument passed to this function
                        ;just for notes -> 
                            ; 0 offset: base pointer itself, 
                            ; 4 offset: return address of the function caller who called us
    lidt [ebx]         ; load interrupt descriptor table 

    pop ebp     ;   pop the base pointer
    ret