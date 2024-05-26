#include "kernel.h"
#include <stdint.h>
#include <stddef.h>

#include "idt/idt.h"
#include "memory/heap/kheap.h"
#include "memory/paging/paging.h"
#include "memory/memory.h"

#include "disk/disk.h"
#include "fs/pparser.h"
#include "fs/file.h"

#include "string/string.h"
#include "config.h"
#include "gdt/gdt.h"

#include "task/tss.h"
#include "task/process.h"
#include "status.h"

#include "isr80h/isr80h.h"

#include "keyboard/keyboard.h"

uint16_t* video_mem = 0;
uint16_t terminal_row = 0;
uint16_t terminal_col = 0;

/** @brief 4gb chunk of paging */
static struct paging_4gb_chunk* kernel_chunk = 0;

/**
 * @brief Create proper value of 2 bytes to be printed on the screen
 * 
 * @note endiannes is considered. LSB resides higher address.
*/
uint16_t terminal_make_char(char c, char colour);

/**
 * @brief 
*/
void terminal_initialize();

void terminal_putchar(int x, int y, char c, char colour);
uint16_t terminal_make_char(char c, char colour);
void terminal_backspace();
void terminal_writechar(char c, char colour);

/** @brief the function printing error message to screen and stops kernel 
 * 
 * @note it is used when we can not recover to kernel
*/
void panic(const char* msg);

struct tss tss;
struct gdt gdt_real[MAEROS_TOTAL_GDT_SEGMENTS];
struct gdt_structured gdt_structured[MAEROS_TOTAL_GDT_SEGMENTS] = {
    {.base = 0x00, .limit = 0x00, .type = 0x00},                 // NULL Segment
    {.base = 0x00, .limit = 0xffffffff, .type = 0x9a},           // Kernel code segment
    {.base = 0x00, .limit = 0xffffffff, .type = 0x92},           // Kernel data segment
    {.base = 0x00, .limit = 0xffffffff, .type = 0xf8},           // User code segment
    {.base = 0x00, .limit = 0xffffffff, .type = 0xf2},           // User data segment
    {.base = (uint32_t)&tss, .limit=sizeof(tss), .type = 0xE9}   // TSS Segment
};

void kernel_main()
{
    terminal_initialize();
    print("Kernel Start\n");
    
    //print("H E L O \n WORLD");
    memset(gdt_real, 0x00, sizeof(gdt_real));
    gdt_structured_to_gdt(gdt_real, gdt_structured, MAEROS_TOTAL_GDT_SEGMENTS);

    // Load the gdt
    gdt_load(gdt_real, sizeof(gdt_real));
    print("GDT Loaded \n");

    kheap_init();
    print("kernel heap initialized \n");

    /* initiliaze file systems */
    fs_init();
    print("file system init \n");


    /* search and initialize a disk */
    disk_search_and_init();
    print("disk search and init \n");

    /* interrupt descriptor table init*/
    idt_init();
    print("idt \n");

    // Setup the TSS
    memset(&tss, 0x00, sizeof(tss));
    tss.esp0 = 0x600000;    //where kernel stack is located
    tss.ss0 = KERNEL_DATA_SELECTOR;

    // Load the TSS, 0x28 is because that would be the offset in the GDT
    // in the gdt_real once we pass this structure to the function
    tss_load(0x28);
    print("TSSegment loaded \n");


    // Setup paging
    kernel_chunk = paging_new_4gb(PAGING_IS_WRITEABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);
    
    // Switch to kernel paging chunk
    paging_switch(kernel_chunk);
    print("Paging is set \n");

    // Enable paging
    enable_paging();
    print("Enable paging \n");

    // Add syscall to table
    isr80h_register_commands();
    print("Register kernel syscalls \n");

    // Initialize all keyboard in system
    keyboard_init();
    print("Initialize keyboard \n");



    struct process* process = 0;
    int ret = process_load("0:/blank.bin", &process);
    if(MAEROS_ALL_OK != ret)
    {
        panic("Failed to load process file \n");
    }
    print("user program is loaded \n");

    task_run_first_ever_task();
     print("run first program \n");

    /* Enable interrupts */
    //enable_interrupts(); it is enabled after loading user program

    // void* ptr = kmalloc(50);
    // void* ptr2 = kmalloc(5000);
    // void* ptr3 = kmalloc(5600);
    // kfree(ptr);
    // void* ptr4 = kmalloc(50);
    // if(ptr || ptr2 || ptr3 || ptr4)
    // {
        
    // }

    /* Below code is doing this - map virtual to ptr's addresses
    it means whatever to do virtual, it effect ptr. 
    For the below example, when writing 0x1000 will not effect physical 0x1000,
    it effects physical address of ptr
    */

/*  
    -- mapping/paging test code block --

    uint32_t virtual = 0x1000;
    char* ptr = kzalloc(4096);
    paging_set(paging_4gb_chunk_get_directory(kernel_chunk), 
                (void*)virtual,
                (uint32_t) ptr | PAGING_ACCESS_FROM_ALL | PAGING_IS_PRESENT | PAGING_IS_WRITEABLE
                );
    char* ptr2 = (char*) 0x1000;
    ptr2[0] = 'A';
    ptr2[1] = 'B';
    print(ptr2);    
    print(ptr);
*/

/* -------fopen function, reading a file from disk can be test as shown below--------*/
/*  int fd = fopen("0:/hello.txt", "r");
    if(fd)
    {
        print("We opened hello.txt \n");
        char buf[6];
        fseek(fd , 2 , SEEK_SET);
        fread(buf, 10, 1, fd);
        print(buf);
        struct file_stat s;
        fstat(fd, &s);
        fclose(fd);
    }
*/
    print("Kernel Start");
    while(1);
}

/** @brief this will switch the page directory to the kernel page directory. 
 * And it will also change the registers to the kernel registers.
*/
void kernel_page()
{
    /* changes all the segment registers to point to the kernel data segment */
    kernel_registers();
    paging_switch(kernel_chunk);
}


void panic(const char* msg)
{
    print(msg);
    while(1) {}
}

void print(const char* str)
{
    size_t len = strlen(str);
    for (int i = 0; i < len; i++)
    {
        terminal_writechar(str[i], 15);
    }
}

void terminal_initialize()
{
    video_mem = (uint16_t*)(0xB8000);
    terminal_row = 0;
    terminal_col = 0;
    for (int y = 0; y < VGA_HEIGHT; y++)
    {
        for (int x = 0; x < VGA_WIDTH; x++)
        {
            terminal_putchar(x, y, ' ', 0);
        }
    }   
}

uint16_t terminal_make_char(char c, char colour)
{
    return (colour << 8) | c;
}

void terminal_putchar(int x, int y, char c, char colour)
{
    video_mem[(y * VGA_WIDTH) + x] = terminal_make_char(c, colour);
}

void terminal_backspace()
{
    if (terminal_row == 0 && terminal_col == 0)
    {
        return;
    }

    if (terminal_col == 0)
    {
        terminal_row -= 1;
        terminal_col = VGA_WIDTH;
    }

    terminal_col -=1;
    terminal_writechar(' ', 15);
    terminal_col -=1;
}

void terminal_writechar(char c, char colour)
{
    if (c == '\n')
    {
        terminal_row += 1;
        terminal_col = 0;
        return;
    }

    if (c == 0x08)
    {
        terminal_backspace();
        return;
    }

    terminal_putchar(terminal_col, terminal_row, c, colour);
    terminal_col += 1;
    if (terminal_col >= VGA_WIDTH)
    {
        terminal_col = 0;
        terminal_row += 1;
    }
}

