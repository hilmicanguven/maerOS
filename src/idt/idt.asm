section .asm



; ------------------------------------------------------------------------------
; handler functions implemented in C code
; ------------------------------------------------------------------------------
extern int21_handler        ; interrupt 21h is keyboard handler
extern no_interrupt_handler ; exception occured but no handler is set for this exception
extern isr80h_handler       ; interrupt 80h for printf 
extern interrupt_handler
; ------------------------------------------------------------------------------


; ------------------------------------------------------------------------------
; function implemented in .Asm file (global function prototypes seen by C files)
; ------------------------------------------------------------------------------
global init21h      ; initialize 21h interrupt which is keyboard interrupt
global no_interrupt ; no interrupt routine
global idt_load     ; load interrupt descriptor table via C files. it is global to be seen by C files
                    ; Notice the first parameter to this function will be idt address
global enable_interrupts    ; enable interrupts
global disable_interrupts   ; disable interrupts
global isr80h_wrapper       ; wrapper for handling interrupt 0x80
; ------------------------------------------------------------------------------


enable_interrupts:
    sti     ; Enable the interrupts
    ret

disable_interrupts:
    cli     ; Clear interrupt instruction
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

; @note when idt_set is called, we set some flags and some of them causes disabling interrupts
; Therefore, no need to disabke/enable interrupt starting/end of routine
; It can be also removed at tther interrupt handler routines (int21h, ....)

isr80h_wrapper:
    ; INTERRUPT FRAME START
    ; ALREADY PUSHED TO US BY THE PROCESSOR UPON ENTRY TO THIS INTERRUPT
    ; uint32_t ip
    ; uint32_t cs;
    ; uint32_t flags
    ; uint32_t sp;
    ; uint32_t ss;
    ; Pushes the general purpose registers to the stack
    pushad
    
    ; INTERRUPT FRAME END

    ; Push the stack pointer so that we are pointing to the interrupt frame

    ; Later we use this pointer to cast interrupt frame
    push esp

    ; EAX holds our command lets push it to the stack for isr80h_handler
    ; it will contain the command that our function that our kernel should invoke.
    push eax
    call isr80h_handler
    mov dword[tmp_res], eax ; eax will have the value for the return result from this function call.
    add esp, 8              ; since we push 2 times, restore the original location

    ; Restore general purpose registers for user land
    popad
    mov eax, [tmp_res]  ;tmp_res: just for ensuring not corrupt eax register accidentally
    iretd

section .data
; Inside here is stored the return result from isr80h_handler
tmp_res: dd 0
