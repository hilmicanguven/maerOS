ORG 0x7C00 ; the first instruction is at origin
BITS 16

CODE_SEG equ gdt_code - gdt_start   ; code segment offset
DATA_SEG equ gdt_data - gdt_start   ; data segment offset

jmp short start
nop

;times 33 db 0   ; add 33 bytes after short jump as described in BPB (Boot Parameter Table) table.

; FAT16 Header
OEMIdentifier           db 'MAEROS  '   ;must be 8 bytes padded by spaces
BytesPerSector          dw 0x200        ;512 bytes, it is fixed for the hardware
SectorsPerCluster       db 0x80
ReservedSectors         dw 200          ;reserved sectors are where our kernel would be and loaded
FATCopies               db 0x02         ;2 FAT copies, original and back-up
RootDirEntries          dw 0x40
NumSectors              dw 0x00
MediaType               db 0xF8
SectorsPerFat           dw 0x100
SectorsPerTrack         dw 0x20
NumberOfHeads           dw 0x40
HiddenSectors           dd 0x00
SectorsBig              dd 0x773594

; Extended BPB Boot Parameter Block (Dos 4.0)
DriveNumber             db 0x80
WinNTBit                db 0x00
Signature               db 0x29
VolumeID                dd 0xD105
VolumeIDString          db 'MAEROS BOOT'    ; must be 11 bytes
SystemIDString          db 'FAT16   '

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


; Later we implement these in C code, struct gdt
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

; print:
;     mov bx, 0
; .loop:
;     ;  "loadsb" instruction do this
;     ;  -> move si register to al register
;     ;  ->  increment the si register value
;     ;
;     lodsb
;     cmp al, 0   ;do I reach to the end of the message
;     je .done
;     call print_char
;     jmp .loop
; .done:
;     ret

    
    ; print char operation based on interrupt provided by BIOS
    ; see Ralf Brown's Interrupt List
    ; int 10h : interrupt type, video - teletype output
    ; display a character on the screen
    ; AH : OEh
    ; AL : characater to write (this is filled by loadsb instruciton)

;print_char:
;    mov ah, 0eh
;    int 0x10
;    ret

;message: db 'Hello World message added with db instruction!', 0

 ;[BITS 32]  ; indicates that code written for 32-bit processor arch.
 ;load32:
 ;
 ;   ; enable the A20 line
 ;   in al, 0x92     ; in and out instructions is used to access to processor bus
 ;   or al, 2        ; this is how we communicate with the hardwre and motherboard
 ;   out 0x92, al
 ;   jmp $

[BITS 32]
load32:
    ; load into kernel, and jump to it
    mov eax, 1          ; starting sector that we want to load from
    mov ecx, 100        ; total number of sectors we want to load
    mov edi, 0x00100000
    call ata_lba_read   ;talk with the driver and actually load sectors into memory
    jmp CODE_SEG:0x00100000

ata_lba_read:
    mov ebx, eax, ; Backup the LBA
    ; Send the highest 8 bits of the lba to hard disk controller
    shr eax, 24
    or eax, 0xE0 ; Select the  master drive
    mov dx, 0x1F6
    out dx, al
    ; Finished sending the highest 8 bits of the lba

    ; Send the total sectors to read
    mov eax, ecx
    mov dx, 0x1F2
    out dx, al
    ; Finished sending the total sectors to read

    ; Send more bits of the LBA
    mov eax, ebx ; Restore the backup LBA
    mov dx, 0x1F3
    out dx, al
    ; Finished sending more bits of the LBA

    ; Send more bits of the LBA
    mov dx, 0x1F4
    mov eax, ebx ; Restore the backup LBA
    shr eax, 8
    out dx, al
    ; Finished sending more bits of the LBA

    ; Send upper 16 bits of the LBA
    mov dx, 0x1F5
    mov eax, ebx ; Restore the backup LBA
    shr eax, 16
    out dx, al
    ; Finished sending upper 16 bits of the LBA

    mov dx, 0x1f7
    mov al, 0x20
    out dx, al

    ; Read all sectors into memory
.next_sector:
    push ecx

; Checking if we need to read
.try_again:
    mov dx, 0x1f7
    in al, dx
    test al, 8
    jz .try_again

; We need to read 256 words at a time
    mov ecx, 256
    mov dx, 0x1F0
    rep insw
    pop ecx
    loop .next_sector
    ; End of reading sectors into memory
    ret

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