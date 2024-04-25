#include "disk.h"
#include "io/io.h"
#include "config.h"
#include "status.h"
#include "memory/memory.h"
#include "fs/file.h"

/** @brief disk structure to represents a disk in system (primary hard disk) */
disk_t disk;

/** @brief low-level function to read blocks from disk */
static int disk_read_sector(int lba, int total, void* buf);



void disk_search_and_init()
{
    memset(&disk, 0, sizeof(disk));
    disk.type = MAEROS_DISK_TYPE_REAL;
    disk.sector_size = MAEROS_SECTOR_SIZE
    disk.id = 0;
    disk.filesystem = fs_resolve(&disk);
}

disk_t* disk_get(int index)
{
    if (index != 0)
        return 0;
    return &disk;
}

int disk_read_block(disk_t* idisk, unsigned int lba, int total, void* buf)
{
    if (idisk != &disk)
    {
        return -EIO;
    }
    return disk_read_sector(lba, total, buf);
}

static int disk_read_sector(int lba, int total, void* buf)
{
    outb(0x1F6 /* Drive / Head Register */, (lba >> 24) | 0xE0);
    outb(0x1F2 /* Sector Count Register*/, total);
    outb(0x1F3 /* Sector Number Register (LBAlo) */, (unsigned char)(lba & 0xff));
    outb(0x1F4 /* Cylinder Low Register / (LBAmid) */, (unsigned char)(lba >> 8));
    outb(0x1F5 /* Cylinder High Register / (LBAhi) */, (unsigned char)(lba >> 16));
    outb(0x1F7 /* Status Register*/, 0x20);

    /* read 2 bytes at a time */
    unsigned short* ptr = (unsigned short*) buf;
    for (int b = 0; b < total; b++)
    {
        // Wait for the buffer to be ready
        char c = insb(0x1F7 /* Status Register*/);
        while(!(c & 0x08 /* is ready bit */))
        {
            c = insb(0x1F7);
        }

        // Copy from hard disk to memory
        for (int i = 0; i < 256; i++)
        {
            *ptr = insw(0x1F0 /* Data Register*/);
            ptr++;
        }

    }
    return 0;
}
