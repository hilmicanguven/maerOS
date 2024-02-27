ORG 0x0 ; the first instruction is at origin
BITS 16

_start:
    jmp short start
    nop

times 33 db 0   ; add 33 bytes after short jump as described in BPB (Boot Parameter Table) table.

start:
    jmp 0x7C0:step2 ; code segment changed to that address



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

    ;move address of message to si register
    mov si, message ; si: rsource index general purpose register
    call print
    jmp $

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