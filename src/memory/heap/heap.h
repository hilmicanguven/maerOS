#ifndef HEAP_H
#define HEAP_H

#include "config.h"
#include <stdint.h>
#include <stddef.h>

/**
 * @brief The flag states entry in the table is allocated
*/
#define HEAP_BLOCK_TABLE_ENTRY_TAKEN 0x01
/**
 * @brief The flag states entry in the table is not allocated and free
*/
#define HEAP_BLOCK_TABLE_ENTRY_FREE 0x00

/**
 * @brief The bit mask for entry in the table, it masks whether block
 * has another block
*/
#define HEAP_BLOCK_HAS_NEXT 0b10000000

/**
 * @brief The bit mask for entry in the table, it masks whether block
 * is the first block allocated
*/
#define HEAP_BLOCK_IS_FIRST  0b01000000

/**
 * @brief 8 bits entries in the heap table
*/
typedef unsigned char HEAP_BLOCK_TABLE_ENTRY;

/**
 * @brief Heap table holds entries represents allocated memory 
*/
struct heap_table
{
    HEAP_BLOCK_TABLE_ENTRY* entries;
    size_t total;
};

/**
 * @brief Heap structure
*/
struct heap
{
    /** @brief Heap table */
    struct heap_table* table;

    /** @brief Start address of the heap data pool */
    void* saddr;
};

int heap_create(struct heap* heap, void* ptr, void* end, struct heap_table* table);

/**
 * @brief Memory allocate from the heap region in unit of blocks
*/
void* heap_malloc(struct heap* heap, size_t size);

/**
 * @brief Free allocated memory from the heap
*/
void heap_free(struct heap* heap, void* ptr);
#endif