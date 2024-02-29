ORG 0x7C00 ; the first instruction is at origin
BITS 16

CODE_SEG equ gdt_code - gdt_start   ; code segment offset
DATA_SEG equ gdt_data - gdt_start   ; data segment offset

_start:
    jmp short start
    nop

times 33 db 0   ; add 33 bytes after short jump as described in BPB (Boot Parameter Table) table.

start:
    jmp 0:step2 ; code segment changed to that address

step2:    
    cli ; clear interrupts
    ; ensure the segment registers and stack pointer to show rigth addreses
    mov ax, 0x00
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00
    sti ; enable interrupts

;----------------------------------
;   SET HANDLER FOR ZEROth INTERRUPT
;   mov word[ss:0x00], handler_zero     ;offset
;   mov word[ss:0x02], 0x7C0            ;segment
;   
;   GENERATE INTERRUPT
;   int 0
;OR  
;   mov ax ,0x0
;   div ax
;----------------------------------

.load_protected:    ; 
    cli             ; clear interrupts
    lgdt[gdt_descriptor]    ; load GDT by using gdt_descriptor label address which holds size and offset
                            ; lgdt expect that structure, first size second base addres
    mov eax, cr0            ; read
    or eax, 0x1             ; modify by setting lowest CR bit
    mov cr0, eax            ; update the CR0
    jmp CODE_SEG:load32     ;


; GDT
gdt_start:
gdt_null:
    dd 0x0  ; 64bit null descriptor
    dd 0x0  ; 64bit null descriptor

; offset 0x8
gdt_code:     ; CS SHOULD POINT TO THIS
    dw 0xffff ; Segment limit first 0-15 bits
    dw 0      ; Base first 0-15 bits
    db 0      ; Base 16-23 bits
    db 0x9a   ; Access byte
    db 11001111b ; High 4 bit flags and the low 4 bit flags
    db 0        ; Base 24-31 bits

; offset 0x10
gdt_data:      ; DS, SS, ES, FS, GS
    dw 0xffff ; Segment limit first 0-15 bits
    dw 0      ; Base first 0-15 bits
    db 0      ; Base 16-23 bits
    db 0x92   ; Access byte
    db 11001111b ; High 4 bit flags and the low 4 bit flags
    db 0        ; Base 24-31 bits

gdt_end:

;gdt_start means starting address of the GDT table
;and
;gdt_end represents the end point of the table
;therefore, we can calculate the size of the table as shown in below
gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; define word
    dd gdt_start                ; define double word
    ;move address of message to si register
    ;mov si, message ; si: rsource index general purpose register
    ;call print
    ;jmp $

print:
    mov bx, 0
.loop:
    ;  "loadsb" instruction do this
    ;  -> move si register to al register
    ;  ->  increment the si register value
    ;
    lodsb
    cmp al, 0   ;do I reach to the end of the message
    je .done
    call print_char
    jmp .loop
.done:
    ret

    
    ; print char operation based on interrupt provided by BIOS
    ; see Ralf Brown's Interrupt List
    ; int 10h : interrupt type, video - teletype output
    ; display a character on the screen
    ; AH : OEh
    ; AL : characater to write (this is filled by loadsb instruciton)

print_char:
    mov ah, 0eh
    int 0x10
    ret

message: db 'Hello World!', 0

 [BITS 32]  ; indicates that code written for 32-bit processor arch.
 load32:

    ; enable the A20 line
    in al, 0x92     ; in and out instructions is used to access to processor bus
    or al, 2        ; this is how we communicate with the hardwre and motherboard
    out 0x92, al
    jmp $

;/* pad zeros to code segment of resulted binary file of the program */
times 510-($ - $$) db 0
;/*  Boot signature (0x55AA) found at the end of bootloader
;    Since endiannes considering, we write flipped
; */
dw 0xAA55 



; Set your own interrupt handler, i.e. interrupt zero
;
;handler_zero:
;   mov ah, 0eh
;   mov al, 'A'
;   mov bx, 0x08
;   int 0x10
;   iret
;
;