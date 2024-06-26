#ifndef KERNEL_H
#define KERNEL_H

#define VGA_WIDTH 80
#define VGA_HEIGHT 20

void kernel_main();

/**
 * @brief printf function uses VGA resource of BIOS
*/
void print(const char* str);

/** @brief The function puts characters to terminal when key is pressed */
void terminal_writechar(char c, char colour);


void panic(const char* msg);
void kernel_page();
void kernel_registers();


#define ERROR(value) (void*)(value)
#define ERROR_I(value) (int)(value)

/** @brief Check return value is error not. It return one if value is error*/
#define ISERR(value) ((int)value < 0)

#endif


