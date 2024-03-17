#include "kheap.h"
#include "heap.h"
#include "config.h"
#include "kernel.h"
#include "memory/memory.h"

/**
 * @brief kernel heap structure
*/
struct heap kernel_heap;

/**
 * @brief kernel heap table
*/
struct heap_table kernel_heap_table;


void kheap_init()
{
    int total_table_entries = MAEROS_HEAP_SIZE_BYTES / MAEROS_HEAP_BLOCK_SIZE;
    kernel_heap_table.entries = (HEAP_BLOCK_TABLE_ENTRY*)(MAEROS_HEAP_TABLE_ADDRESS);
    kernel_heap_table.total = total_table_entries;

    void* end = (void*)(MAEROS_HEAP_ADDRESS + MAEROS_HEAP_SIZE_BYTES);
    int res = heap_create(&kernel_heap, (void*)(MAEROS_HEAP_ADDRESS), end, &kernel_heap_table);
    if (res < 0)
    {
        //Kernel Panic, actually
        print("Failed to create heap\n");
    }

}
