#ifndef DISKSTREAMER_H
#define DISKSTREAMER_H

#include "disk.h"

struct disk_stream
{
    /** @brief byte position at a disk */
    int pos;
    /** @brief disk */
    struct disk* disk;
};

/** @brief create a disk stream structure with initial conditions, pos=0 and disk=main disk*/
struct disk_stream* diskstreamer_new(int disk_id);

/** @brief set position of current streamer */
int diskstreamer_seek(struct disk_stream* stream, int pos);

/** @brief read from disk stream */
int diskstreamer_read(struct disk_stream* stream, void* out, int total);

/** @brief close disk stream */
void diskstreamer_close(struct disk_stream* stream);

#endif