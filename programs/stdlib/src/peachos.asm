[BITS 32]

section .asm

global print:function ; print is a function label
global peachos_getkey:function
global maeros_malloc:function
global maeros_free:function
global maeros_putchar:function
global maeros_process_load_start:function
global maeros_process_get_arguments:function 
global maeros_system:function
global maeros_exit:function

; void print(const char* message)
print:
    push ebp ; push base pointer
    mov ebp, esp
    push dword[ebp+8]   ; 8 because we pushed twice previously (ebp and return address)
                        ; thats how we pushed message to stack
    mov eax, 1 ; syscall to Command print
    int 0x80
    add esp, 4 ; add 4 because we push message (4 bytes) and it should be restored back
    pop ebp ; pop base pointer
    ret

; int peachos_getkey() //get key from user via terminal
peachos_getkey:
    push ebp
    mov ebp, esp
    mov eax, 2 ; Command getkey
    int 0x80
    pop ebp
    ret

; void maeros_putchar(char c)
maeros_putchar:
    push ebp
    mov ebp, esp
    mov eax, 3 ; Command putchar
    push dword [ebp+8] ; Variable "c"
    int 0x80
    add esp, 4
    pop ebp
    ret

; void* maeros_malloc(size_t size)
maeros_malloc:
    push ebp
    mov ebp, esp
    mov eax, 4 ; Command 4 malloc (Allocates memory for the process)
    push dword[ebp+8] ; Variable "size"
    int 0x80
    add esp, 4
    pop ebp
    ret

; void maeros_free(void* ptr)
maeros_free:
    push ebp
    mov ebp, esp
    mov eax, 5 ; Command 5 free (Frees the allocated memory for this process)
    push dword[ebp+8] ; Variable "ptr"
    int 0x80
    add esp, 4
    pop ebp
    ret

; void maeros_process_load_start(const char* filename)
maeros_process_load_start:
    push ebp
    mov ebp, esp
    mov eax, 6 ; Command 6 process load start ( stars a process )
    push dword[ebp+8] ; Variable "filename"
    int 0x80
    add esp, 4  ;in contrary to other commands, we do not back from int80h
    pop ebp
    ret

; int maeros_system(struct command_argument* arguments)
maeros_system:
    push ebp
    mov ebp, esp
    mov eax, 7 ; Command 7 process_system ( runs a system command based on the arguments)
    push dword[ebp+8] ; Variable "arguments"
    int 0x80
    add esp, 4
    pop ebp
    ret


; void maeros_process_get_arguments(struct process_arguments* arguments)
maeros_process_get_arguments:
    push ebp
    mov ebp, esp
    mov eax, 8 ; Command 8 Gets the process arguments
    push dword[ebp+8] ; Variable arguments
    int 0x80
    add esp, 4
    pop ebp
    ret

; void maeros_exit()
maeros_exit:
    push ebp
    mov ebp, esp
    mov eax, 9 ; Command 9 process exit
    int 0x80
    pop ebp
    ret