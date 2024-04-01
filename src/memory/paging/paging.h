#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/** @brief Mask value for cache disable bit in table entry structure (register), PCD bit is for disabling cache */
#define PAGING_CACHE_DISABLED  0b00010000

/** @brief Mask value for write through bit in table entry structure (register), PWT bit is for write-through ability
 * 
 * Write Through: 
 */
#define PAGING_WRITE_THROUGH   0b00001000

/** @brief Mask value for User/Supervisor bit in table entry structure (register)
 * if this bit is set, this page is accessible from each ring levels. If it is not set, this page is only accessible
 * from supervisor ring level */ 
#define PAGING_ACCESS_FROM_ALL 0b00000100

/** @brief Mask value for Read/Write writeable  bit in table entry structure (register)
 * If the bit is set, the page is read/write. Otherwise when it is not set, the page is read-only. 
 */
#define PAGING_IS_WRITEABLE    0b00000010

/** @brief P, or 'Present'. If the bit is set, the page is actually in physical memory at the moment. */
#define PAGING_IS_PRESENT      0b00000001

/** @brief Total entries per table*/
#define PAGING_TOTAL_ENTRIES_PER_TABLE 1024

/** @brief Each page takes that amount of byte */
#define PAGING_PAGE_SIZE 4096

/** @brief The topmost paging structure is the page directory. It is essentially an array of page directory entries 
 * that take the following form. (Page Directory Entry) */
struct paging_4gb_chunk
{
    /** @brief pointer to the directory */
    uint32_t* directory_entry;
};

struct paging_4gb_chunk* paging_new_4gb(uint8_t flags);
void paging_switch(struct paging_4gb_chunk* directory);
void enable_paging();

int paging_set(uint32_t* directory, void* virt, uint32_t val);
bool paging_is_aligned(void* addr);

uint32_t* paging_4gb_chunk_get_directory(struct paging_4gb_chunk* chunk);
void paging_free_4gb(struct paging_4gb_chunk* chunk);

int paging_map_to(struct paging_4gb_chunk *directory, void *virt, void *phys, void *phys_end, int flags);
int paging_map_range(struct paging_4gb_chunk* directory, void* virt, void* phys, int count, int flags);
int paging_map(struct paging_4gb_chunk* directory, void* virt, void* phys, int flags);
void* paging_align_address(void* ptr);
uint32_t paging_get(uint32_t* directory, void* virt);
void* paging_align_to_lower_page(void* addr);
void* paging_get_physical_address(uint32_t* directory, void* virt);

#endif