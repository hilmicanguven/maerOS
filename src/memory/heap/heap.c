#include "heap.h"
#include "kernel.h"
#include <stdbool.h>
#include "status.h"
#include "memory/memory.h"

/**
 * @brief The function checks whether address is aligned to OS heap block size
*/
static bool heap_validate_alignment(void* ptr)
{
    return ((unsigned int) ptr & MAEROS_HEAP_BLOCK_SIZE) == 0;
}

/**
 * @brief to validate that the pointer they provided and the end address is 
 * correct for thetable they provided
 * 
 * In other words, Does this table know how many blocks we need between pointer and end
*/
static int heap_validate_table(void* ptr, void* end, struct heap_table* table)
{
    int res = 0;

    size_t table_size = (size_t)(end - ptr);
    size_t total_blocks = table_size / MAEROS_HEAP_BLOCK_SIZE;
    if (table->total != total_blocks)
    {
        res = -EINVARG;
        goto out;
    }

out:
    return res;
}

int heap_create(struct heap* heap, void* ptr, void* end, struct heap_table* table)
{
    int res = 0;

    if (!heap_validate_alignment(ptr) || !heap_validate_alignment(end))
    {
        res = -EINVARG;
        goto out;
    }

    memset(heap, 0, sizeof(struct heap));
    heap->saddr = ptr;      //heap starting address
    heap->table = table;    //heap table into heap object

    res = heap_validate_table(ptr, end, table);
    if (res < 0)
    {
        goto out;
    }

    size_t table_size = sizeof(HEAP_BLOCK_TABLE_ENTRY) * table->total;
    memset(table->entries, HEAP_BLOCK_TABLE_ENTRY_FREE, table_size);

out:
    return res;
}
