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

#endif