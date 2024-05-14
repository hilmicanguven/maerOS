#ifndef FILE_H
#define FILE_H

#include "pparser.h"
#include <stdint.h>

/** @brief Type of seek functions */
typedef unsigned int FILE_SEEK_MODE;
enum
{
    /** @brief Set cursor position from the start by the amount of offset */
    SEEK_SET,
    SEEK_CUR,
    SEEK_END
};

/** @brief File modes chosen with opening */
typedef unsigned int FILE_MODE;
enum
{ 
    FILE_MODE_READ,
    FILE_MODE_WRITE,
    FILE_MODE_APPEND,
    FILE_MODE_INVALID
};

enum
{
    FILE_STAT_READ_ONLY = 0b00000001
};

typedef unsigned int FILE_STAT_FLAGS;

/** @brief */
struct disk;

/** @brief return information about a file*/
struct file_stat
{
    /** @brief flags to show file permission */
    FILE_STAT_FLAGS flags;
    /** @brief size of the file */
    uint32_t filesize;
};

/** @brief Open function prototype */
typedef void*(*FS_OPEN_FUNCTION)(struct disk* disk, struct path_part* path, FILE_MODE mode);

/** @brief file system read function prototype
 * low level system should implement read function according to this prototype
 * 
 * @param size how many bytes we are reading
 * @param nmemb how many of those blocks we want to read (total read bytes is = size*nmemb)
*/
typedef int (*FS_READ_FUNCTION)(struct disk* disk, void* private, uint32_t size, uint32_t nmemb, char* out);

/** @brief The function resolved whether disk has desired file system or no, i.e. FAT16*/
typedef int (*FS_RESOLVE_FUNCTION)(struct disk* disk);

typedef int (*FS_CLOSE_FUNCTION)(void* private);

/** @brief file system seek function*/
typedef int (*FS_SEEK_FUNCTION)(void* private, uint32_t offset, FILE_SEEK_MODE seek_mode);

typedef int (*FS_STAT_FUNCTION)(struct disk* disk, void* private, struct file_stat* stat);

/** @brief The struct which holds file system generic functions */
struct filesystem
{
    // Filesystem should return zero from resolve if the provided disk is using its filesystem
    FS_RESOLVE_FUNCTION resolve;
    FS_OPEN_FUNCTION open;
    FS_READ_FUNCTION read;
    FS_SEEK_FUNCTION seek;
    FS_STAT_FUNCTION stat;
    FS_CLOSE_FUNCTION close;
    /** @brief name of the file system, FAT16, NTFS, ...*/
    char name[20];
};

struct file_descriptor
{
    /** @brief The descriptor index*/
    int index;
    /** @brief file system functions */
    struct filesystem* filesystem;
    /** @brief Private data for internal file descriptor */
    void* private;
    /** @brief The disk that the file descriptor should be used on */
    struct disk* disk;
};



void fs_init();
int fopen(const char* filename, const char* mode_str);
int fseek(int fd, int offset, FILE_SEEK_MODE whence);
int fread(void* ptr, uint32_t size, uint32_t nmemb, int fd);

/** @brief The function shall obtain information about an open file associated with the file descriptor fildes, 
 * and shall write it to the area pointed to by stat.*/
int fstat(int fd, struct file_stat* stat);
int fclose(int fd);

void fs_insert_filesystem(struct filesystem* filesystem);
struct filesystem* fs_resolve(struct disk* disk);
#endif