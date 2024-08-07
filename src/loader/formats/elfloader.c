#include "elfloader.h"
#include "fs/file.h"
#include "status.h"
#include <stdbool.h>
#include "memory/memory.h"
#include "memory/heap/kheap.h"
#include "string/string.h"
#include "memory/paging/paging.h"
#include "kernel.h"
#include "config.h"

/** @brief it is the signature, The initial bytes of an ELF header (and an object file) 
 * correspond to identification number (e_ident field )*/
const char elf_signature[] = {0x7f, 'E', 'L', 'F'};

/** @brief The function check signature is a valid signature or not
 * it returns zero if signature is valid and file is elf file
*/
static bool elf_valid_signature(void* buffer)
{
    return memcmp(buffer, (void*) elf_signature, sizeof(elf_signature)) == 0;
}
/** @brief the functions check we have valid class or not
 * it return one if it is valid
*/
static bool elf_valid_class(struct elf_header* header)
{
    // We only support 32 bit binaries.
    return header->e_ident[EI_CLASS] == ELFCLASSNONE || header->e_ident[EI_CLASS] == ELFCLASS32;
}
/** @brief the functions check we have valid encoding or not
 * it return one if it is valid. It is only supported Little Endian format
*/
static bool elf_valid_encoding(struct elf_header* header)
{
    return header->e_ident[EI_DATA] == ELFDATANONE || header->e_ident[EI_DATA] == ELFDATA2LSB;
}

/** @brief the functions check file is executable or not
 * it return one if it is valid. It is only supported executable format
 * and not supported shared library
*/
static bool elf_is_executable(struct elf_header* header)
{
    return header->e_type == ET_EXEC && header->e_entry >= MAEROS_PROGRAM_VIRTUAL_ADDRESS;
}

/** @brief */
static bool elf_has_program_header(struct elf_header* header)
{
    return header->e_phoff != 0;
}

/**
 * @brief ELF file memory address as pointer
*/
void* elf_memory(struct elf_file* file)
{
    return file->elf_memory;
}

/** @brief Return ELF Header */
struct elf_header* elf_header(struct elf_file* file)
{
    return file->elf_memory;
}

/** @brief Return section header */
struct elf32_shdr* elf_sheader(struct elf_header* header)
{
    return (struct elf32_shdr*)((int)header+header->e_shoff);
}

/** @brief Return program header */
struct elf32_phdr* elf_pheader(struct elf_header* header)
{
    if(header->e_phoff == 0)
    {
        return 0;
    }

    return (struct elf32_phdr*)((int)header + header->e_phoff);
}

/** @brief Return ELF program header for particular program entry by accepting index */
struct elf32_phdr* elf_program_header(struct elf_header* header, int index)
{
    return &elf_pheader(header)[index];
}

/** @brief Return ELF section header for particular program entry by accepting index */
struct elf32_shdr* elf_section(struct elf_header* header, int index)
{
    return &elf_sheader(header)[index];
}

/** @brief Return program header physical address */
void* elf_phdr_phys_address(struct elf_file* file, struct elf32_phdr* phdr)
{
    return elf_memory(file) + phdr->p_offset;
}

/** @brief Return string table */
char* elf_str_table(struct elf_header* header)
{
    return (char*) header + elf_section(header, header->e_shstrndx)->sh_offset;
}

/** @brief Returns virtual starting address of our file */
void* elf_virtual_base(struct elf_file* file)
{
    return file->virtual_base_address;
}

/** @brief Returns virtual end address of our file*/
void* elf_virtual_end(struct elf_file* file)
{
    return file->virtual_end_address;
}

/** @brief Returns physical starting address of our file */
void* elf_phys_base(struct elf_file* file)
{
    return file->physical_base_address;
}

/** @brief Returns physical end address of our file */
void* elf_phys_end(struct elf_file* file)
{
    return file->physical_end_address;
}

/** @brief Checks ELF file is loaded correctly 
 * @retval MAEROS_ALL_OK File is loaded correctly
*/
int elf_validate_loaded(struct elf_header* header)
{
    return (elf_valid_signature(header) && elf_valid_class(header) && elf_valid_encoding(header) && elf_has_program_header(header)) ? MAEROS_ALL_OK : -EINFORMAT;
}

/** @brief Load program which is a PT_LOAD type. The function calculating the virtual base address and 
 * the physical base */
int elf_process_phdr_pt_load(struct elf_file* elf_file, struct elf32_phdr* phdr)
{
    /* These virtual and physical address can be seen at program header.
    It is also found at .elf file we have seen via dumpelf command */
    if (elf_file->virtual_base_address >= (void*) phdr->p_vaddr || elf_file->virtual_base_address == 0x00)
    {
        elf_file->virtual_base_address = (void*) phdr->p_vaddr;
        elf_file->physical_base_address = elf_memory(elf_file)+phdr->p_offset;
    }

    unsigned int end_virtual_address = phdr->p_vaddr + phdr->p_filesz;
    if (elf_file->virtual_end_address <= (void*)(end_virtual_address) || elf_file->virtual_end_address == 0x00)
    {
        elf_file->virtual_end_address = (void*) end_virtual_address;
        elf_file->physical_end_address = elf_memory(elf_file)+phdr->p_offset+phdr->p_filesz;
    } 
    return 0;
}

/** @brief Load the program by looking program header */
int elf_process_pheader(struct elf_file* elf_file, struct elf32_phdr* phdr)
{
    int res = 0;
    switch(phdr->p_type)
    {
        case PT_LOAD:
            res = elf_process_phdr_pt_load(elf_file, phdr);
        break;
    }
    return res;
}

/** @brief loop through each program headers in the elf file
 * e_phnum field of header is the number of program header
*/
int elf_process_pheaders(struct elf_file* elf_file)
{
    int res = 0;
    struct elf_header* header = elf_header(elf_file);
    for(int i = 0; i < header->e_phnum; i++)
    {
        struct elf32_phdr* phdr = elf_program_header(header, i);
        res = elf_process_pheader(elf_file, phdr);
        if (res < 0)
        {
            break;
        }

    }
    return res;
}

/** @brief load programs from elf file */
int elf_process_loaded(struct elf_file* elf_file)
{
    int res = 0;
    struct elf_header* header = elf_header(elf_file);
    res = elf_validate_loaded(header);
    if (res < 0)
    {
        goto out;
    }

    res = elf_process_pheaders(elf_file);
    if (res < 0)
    {
        goto out;
    }

out:
    return res;
}


int elf_load(const char* filename, struct elf_file** file_out)
{
    /* allocate memory for this elf file */
    struct elf_file* elf_file = kzalloc(sizeof(struct elf_file));
    int fd = 0;
    int res = fopen(filename, "r");
    if (res <= 0)
    {
        res = -EIO;
        goto out;
    }

    fd = res;
    struct file_stat stat;
    res = fstat(fd, &stat);
    if (res < 0)
    {
        goto out;
    }

    elf_file->elf_memory = kzalloc(stat.filesize);
    /* read entire file into memory */
    res = fread(elf_file->elf_memory, stat.filesize, 1, fd);
    if (res < 0)
    {
        goto out;
    }

    res = elf_process_loaded(elf_file);
    if(res < 0)
    {
        goto out;
    }

    *file_out = elf_file;
out:
    fclose(fd);
    return res;
}

void elf_close(struct elf_file* file)
{
    if (!file)
        return;

    kfree(file->elf_memory);
    kfree(file);
}