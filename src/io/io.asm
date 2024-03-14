section .asm

/* Global declarations to be called by C files */
global insb
global insw
global outb
global outw

insb:
    push ebp
    mov ebp, esp

    xor eax, eax    ; XOR with itself generates "zero"
    mov edx, [ebp+8]
    in al, dx       ; Input byte from I/O port in DX into AL.

    pop ebp
    ret

insw:
    push ebp
    mov ebp, esp

    xor eax, eax
    mov edx, [ebp+8]
    in ax, dx       ; Input word from I/O port in DX into AX.

    pop ebp
    ret

outb:
    push ebp
    mov ebp, esp

    mov eax, [ebp+12]   ; second argument
    mov edx, [ebp+8]    ; first argument
    out dx, al          ; Output byte in AL to I/O port address in DX.

    pop ebp
    ret

outw:
    push ebp
    mov ebp, esp

    mov eax, [ebp+12]
    mov edx, [ebp+8]
    out dx, ax          ; Output word in AX to I/O port address in DX.

    pop ebp
    ret