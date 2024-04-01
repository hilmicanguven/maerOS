[BITS 32]

section .asm

global paging_load_directory
global enable_paging

paging_load_directory:
    push ebp
    mov ebp, esp
    mov eax, [ebp+8]
    mov cr3, eax    ; cr3 register contains address of page directory
    pop ebp
    ret

enable_paging:
    push ebp
    mov ebp, esp
    mov eax, cr0    ; we can not change cr0 register directly    
    or eax, 0x80000000 
    mov cr0, eax
    pop ebp
    ret