#ifndef ELFLOADER_H
#define ELFLOADER_H

#include <stdint.h>
#include <stddef.h>

#include "elf.h"
#include "config.h"

/** @brief elf file structure*/
struct elf_file
{
    /** @brief filename*/
    char filename[MAEROS_MAX_PATH];

    /** @brief the size of this elf file when it's loaded into memory */
    int in_memory_size;

    /**
     * @brief The physical memory address that this elf file is loaded at
     * whole .elf file loaded into memory
     */
    void* elf_memory;

    /**
     * @brief The virtual base address of this binary, 
     * the virtual base address will point to the first loadable section in memory.
     */
    void* virtual_base_address;

    /**
     * @brief The ending virtual address
     */
    void* virtual_end_address;

    /**
     * @brief The physical base address of this binary
     * it actually points same thing with virt.base address
     */
    void* physical_base_address;

    /**
     * @brief The physical end address of this bunary
     */
    void* physical_end_address;
};

int elf_load(const char* filename, struct elf_file** file_out);
void elf_close(struct elf_file* file);
void* elf_virtual_base(struct elf_file* file);
void* elf_virtual_end(struct elf_file* file);
void* elf_phys_base(struct elf_file* file);
void* elf_phys_end(struct elf_file* file);

struct elf_header* elf_header(struct elf_file* file);
struct elf32_shdr* elf_sheader(struct elf_header* header);
void* elf_memory(struct elf_file* file);
struct elf32_phdr* elf_pheader(struct elf_header* header);
struct elf32_phdr* elf_program_header(struct elf_header* header, int index);
struct elf32_shdr* elf_section(struct elf_header* header, int index);
void* elf_phdr_phys_address(struct elf_file* file, struct elf32_phdr* phdr);

#endif