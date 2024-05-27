[BITS 32]

section .asm
global _start

_start:
    mov eax,0 ; command 0 is for summation operation
    int 0x80
    

label:
    jmp label