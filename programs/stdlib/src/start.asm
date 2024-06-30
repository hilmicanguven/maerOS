[BITS 32]

global _start
extern c_start
extern maeros_exit

section .asm

_start:
; it starts main program
    call c_start
    ;if program returns(not running forever), then exis from a program
    call maeros_exit
    ret