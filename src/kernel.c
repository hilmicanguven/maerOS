#include "kernel.h"
#include <stdint.h>
#include <stddef.h>

#include "idt/idt.h"
#include "memory/heap/kheap.h"
#include "memory/paging/paging.h"
#include "memory/memory.h"

#include "disk/disk.h"
#include "fs/pparser.h"
#include "string/string.h"

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




void kernel_main()
{
    terminal_initialize();
    
    //print("H E L O \n WORLD");

    kheap_init();

    /* search and initialize a disk */
    disk_search_and_init();

    /* interrupt descriptor table init*/
    idt_init();

    // Setup paging
    kernel_chunk = paging_new_4gb(PAGING_IS_WRITEABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);
    
    // Switch to kernel paging chunk
    paging_switch(kernel_chunk);

    // Enable paging
    enable_paging();

    /* Enable interrupts */
    enable_interrupts();

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

    struct path_root* root_path = pathparser_parse("0:/bin/shell.exe", NULL);
    
    if(root_path)
    {

    }
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

