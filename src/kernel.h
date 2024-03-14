#ifndef KERNEL_H
#define KERNEL_H

#define VGA_WIDTH 80
#define VGA_HEIGHT 20

void kernel_main();

/**
 * @brief printf function uses VGA resource of BIOS
*/
void print(const char* str);

#endif


