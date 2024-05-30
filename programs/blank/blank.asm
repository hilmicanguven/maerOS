[BITS 32]

section .asm
global _start

_start:
    ;mov eax,0 ; command 0 is for summation operation
    ;int 0x80
    ;add esp ,4

_loop:
    call getkey
    ;push message
    ;mov eax, 1  ; command PRINT
    push eax
    mov eax, 3  ; command putchar
    int 0x80
    add esp,4
    jmp _loop
    

getkey:
    mov eax, 2 ; command GET_KEY
    int 0x80
    cmp eax, 0x00 ; zero means no key is pressed
    je getkey     ; jump back(if zero pressed) and wait until key is pressed
    ret

section .data
message: db 'I can talk to kernel from usr program', 0

