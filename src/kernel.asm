[BITS 32]

global _start   ; global because it is known for public (otherwise, when creating object file, it is not seen)
global kernel_registers
extern kernel_main

CODE_SEG equ 0x08 ; GDT offsets of these two segments
DATA_SEG equ 0x10

_start:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov ebp, 0x00200000
    mov esp, ebp

    ; Enable the A20 line
    in al, 0x92
    or al, 2
    out 0x92, al

    ; Remap the master PIC (not to conflict processor reserved interrupts 0 - 0x1F)
    mov al, 00010001b
    out 0x20, al ; Tell master PIC

    mov al, 0x20 ; Interrupt 0x20 is where master ISR should start
    out 0x21, al

    mov al, 00000001b
    out 0x21, al
    ; End remap of the master PIC
    
    ;call kernel main function
    call kernel_main
    
    jmp $


; that changes all the segment registers to point to the kernel data segment.
kernel_registers:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov gs, ax
    mov fs, ax
    ret


times 512-($ - $$) db 0