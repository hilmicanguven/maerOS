#ifndef FAT16_H
#define FAT16_H

#include "fs/file.h"

/** @brief initialize FAT16 file system and add to the table */
struct filesystem* fat16_init();
#endif