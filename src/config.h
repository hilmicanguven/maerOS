#ifndef CONFIG_H
#define CONFIG_H

#define KERNEL_CODE_SELECTOR 0x08
#define KERNEL_DATA_SELECTOR 0x10

/**
 * @brief define total interrupt number by utilizing osdevWiki
 * See 
*/
#define MAEROS_TOTAL_INTERRUPTS 512

/**
 * @brief Fixed heap size, 100MB
*/
#define MAEROS_HEAP_SIZE_BYTES  104857600

/**
 * @brief heap block size, which is considered by allocation for alignment
*/
#define MAEROS_HEAP_BLOCK_SIZE 4096

/**
 * @brief Kernel Heap address, it is determined by osdevwiki memory map
 * it is chosen any proper area for that purpose
*/
#define MAEROS_HEAP_ADDRESS 0x01000000 
/**
 * @brief Kernel Heap Table address, it is determined by osdevwiki memory map
 * it is chosen any proper area for that purpose. i.e. conventional memory
*/
#define MAEROS_HEAP_TABLE_ADDRESS 0x00007E00

/** @brief Sector size of a hard disk */
#define MAEROS_SECTOR_SIZE 512

/** @brief maximum path length of file system */
#define MAEROS_MAX_PATH 108

#define MAEROS_MAX_FILESYSTEMS 12
#define MAEROS_MAX_FILE_DESCRIPTORS 512

#define MAEROS_MAX_PATH 108

/** @brief number of GDT segments*/
#define MAEROS_TOTAL_GDT_SEGMENTS 6

/** @brief Where default registers are there when task used this when it is started initially */
#define MAEROS_PROGRAM_VIRTUAL_ADDRESS 0x400000

/** @brief 16Kb stack size as default */
#define MAEROS_USER_PROGRAM_STACK_SIZE 1024 * 16

/** @brief it is stack address (virtual), it can be same for all task (it physically different) */
#define MAEROS_PROGRAM_VIRTUAL_STACK_ADDRESS_START 0x3FF000

/** @brief in intel, stack grows from from top do down */
#define MAEROS_PROGRAM_VIRTUAL_STACK_ADDRESS_END MAEROS_PROGRAM_VIRTUAL_STACK_ADDRESS_START - MAEROS_USER_PROGRAM_STACK_SIZE

#define MAEROS_MAX_PROGRAM_ALLOCATIONS 1024
#define MAEROS_MAX_PROCESSES 12

/** @brief User data segment which means user stack 
 * @note 0x23 comes from GDT table, named structure "gdt_real"
*/
#define USER_DATA_SEGMENT 0x23
/** @brief User code segment 
* @note 0x23 comes from GDT table, named structure "gdt_real"
*/
#define USER_CODE_SEGMENT 0x1b

#define MAEROS_MAX_ISR80H_COMMANDS 1024

#define MAEROS_KEYBOARD_BUFFER_SIZE 1024

#endif