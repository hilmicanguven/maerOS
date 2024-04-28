#ifndef DISK_H
#define DISK_H

//#include "fs/file.h"

/** @brief Dist type -> partitions, hard disks, virtual file systems */
typedef unsigned int MAEROS_DISK_TYPE;


/** @brief  Represents a real physical hard disk */
#define MAEROS_DISK_TYPE_REAL 0

/** @brief Dist structure has some features */
typedef struct disk
{
    /** @brief type of the disk */
    MAEROS_DISK_TYPE type;
    /** @brief how many sectors does disk have */
    int sector_size;
    /** @brief  The id of the disk */
    int id;    
    /** @brief filesytem found at that disk*/
    struct filesystem* filesystem;
    /** @brief The private data of our filesystem */
    void* fs_private;
}disk_t;


/** @brief it searches and initialize a disk descriptor struct 
 * It also search for filesystem 
 * @note it is pretend to search for now, it assumes only one disk
*/
void disk_search_and_init();

/** @brief return disk descriptor by given index
 * @note for now, only one disk (defined by us as global) is declared
 * therefore, the function returns it
*/
struct disk* disk_get(int index);

/** @brief read block from the specific disk */
int disk_read_block(struct disk* idisk, unsigned int lba, int total, void* buf);

#endif