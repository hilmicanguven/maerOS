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
 * @brief to validate that the pointer they (who created heap) provided and the end address is 
 * correct for the table they provided
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

    /* Clear all block found at heap at initially */
    size_t table_size = sizeof(HEAP_BLOCK_TABLE_ENTRY) * table->total;
    memset(table->entries, HEAP_BLOCK_TABLE_ENTRY_FREE, table_size);

out:
    return res;
}

static uint32_t heap_align_value_to_upper(uint32_t val)
{
    if ((val % MAEROS_HEAP_BLOCK_SIZE) == 0)
    {
        return val;
    }

    val = (val - ( val % MAEROS_HEAP_BLOCK_SIZE));

    val += MAEROS_HEAP_BLOCK_SIZE;
    return val;
}

/**
 * @brief Find entry type by looking 4 bits, remember entries and bit fields
*/
static int heap_get_entry_type(HEAP_BLOCK_TABLE_ENTRY entry)
{
    return entry & 0x0f;
}

/**
 * @brief Find a proper start block which suits/fits given total block number
 * 
 * @note In order to better understanding, please refer to documentation. In summary,
 * we look consecutive blocks whose amount is equal to the required block number
 * This is very primitive function and it is obvious that it is open to developed
*/
int heap_get_start_block(struct heap* heap, uint32_t total_blocks)
{
    struct heap_table* table = heap->table;
    /* Current block that we are on it */
    int block_current = 0;
    /* Start block that we find for allocation */
    int block_start = -1;

    /* go through entire table block by block */
    for (size_t i = 0; i < table->total; i++)
    {
        if (heap_get_entry_type(table->entries[i]) != HEAP_BLOCK_TABLE_ENTRY_FREE)
        {
            /* Reset blcok info since we can not take i'th block*/
            block_current = 0;
            block_start = -1;
            continue;
        }

        // If this is the first block
        if (block_start == -1)
        {
            block_start = i;
        }
        
        block_current++;
        
        if (block_current == total_blocks)
        {
            /* we can not find suitable block, we do not have wnough block */
            break;
        }
    }

    if (block_start == -1)
    {
        return -ENOMEM;
    }
    
    return block_start;

}

/**
 * @brief Calculate allocated address which is returned to user
*/
void* heap_block_to_address(struct heap* heap, int block)
{
    return heap->saddr + (block * MAEROS_HEAP_BLOCK_SIZE);
}

/**
 * @brief Signed blocks as taken which was allocated at the heap
*/
void heap_mark_blocks_taken(struct heap* heap, int start_block, int total_blocks)
{
    /* end block means end block for allocating operation, not the heap end block*/
    int end_block = (start_block + total_blocks)-1;
    
    /* create entris as one of the followings
        - taken and first block and has next? or not?
        - taken and has next block
        - taken and does not have next block
    */
    HEAP_BLOCK_TABLE_ENTRY entry = HEAP_BLOCK_TABLE_ENTRY_TAKEN | HEAP_BLOCK_IS_FIRST;
    if (total_blocks > 1)
    {
        entry |= HEAP_BLOCK_HAS_NEXT;
    }

    for (int i = start_block; i <= end_block; i++)
    {
        heap->table->entries[i] = entry;
        entry = HEAP_BLOCK_TABLE_ENTRY_TAKEN;
        if (i != end_block -1)
        {
            entry |= HEAP_BLOCK_HAS_NEXT;
        }
    }
}

/**
 * @brief allocate a memory blocks which are aligned and true total block number
 * and it returns a address from memory pool
*/
void* heap_malloc_blocks(struct heap* heap, uint32_t total_blocks)
{
    void* address = 0;

    /* Look through heap entry table and see enough room for these blocks */
    int start_block = heap_get_start_block(heap, total_blocks);
    if (start_block < 0)
    {
        goto out;
    }

    /* Calculate address of given block */
    address = heap_block_to_address(heap, start_block);

    // Mark the blocks as taken
    heap_mark_blocks_taken(heap, start_block, total_blocks);

out:
    return address;
}

/**
 * @brief sign blocks as free when it freed.
*/
void heap_mark_blocks_free(struct heap* heap, int starting_block)
{
    struct heap_table* table = heap->table;
    for (int i = starting_block; i < (int)table->total; i++)
    {
        HEAP_BLOCK_TABLE_ENTRY entry = table->entries[i];
        table->entries[i] = HEAP_BLOCK_TABLE_ENTRY_FREE;
        /* if next block is not ours, then stops mark blocks as free and break the loop*/
        if (!(entry & HEAP_BLOCK_HAS_NEXT))
        {
            break;
        }
    }
}

/** @brief calculate corresponding block number when user want to free an address*/
int heap_address_to_block(struct heap* heap, void* address)
{
    return ((int)(address - heap->saddr)) / MAEROS_HEAP_BLOCK_SIZE;
}

void* heap_malloc(struct heap* heap, size_t size)
{
    /* Align size to block size */
    size_t aligned_size = heap_align_value_to_upper(size);
    /* Calculate how many block is require for given size */
    uint32_t total_blocks = aligned_size / MAEROS_HEAP_BLOCK_SIZE;

    return heap_malloc_blocks(heap, total_blocks);
}

void heap_free(struct heap* heap, void* ptr)
{
    heap_mark_blocks_free(heap, heap_address_to_block(heap, ptr));
}