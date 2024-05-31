#ifndef ELF_H
#define ELF_H

#include <stdint.h>
#include <stddef.h>

/** @file elf.h
 * @brief Almost evety define in this file can be mentioned in ELF Loader Specification
 * Please see this spec in the internet: https://refspecs.linuxfoundation.org/elf/elf.pdf
*/
/***** Program Segment Permission *****/

/**
 * A program to be loaded by the system must have at least one loadable segment (although this
is not required by the file format). When the system creates loadable segments' memory images,
it gives access permissions as specified in the p_flags member.
*/

/** @brief Permission: Execute Flag */
#define PF_X 0x01
/** @brief Permission: Write Flag */
#define PF_W 0x02
/** @brief Permission: Read Flag */
#define PF_R 0x04


/** Program Header
 * An executable or shared object file's program header table is an array of structures, each
describing a segment or other information the system needs to prepare the program for
execution. An object file segment contains one or more sections. Program headers are
meaningful only for executable and shared object files. 
*/

/***** Segment Types**** */

/** @brief The array element is unused; other members' values are undefined. This type lets
the program header table have ignored entries.*/
#define PT_NULL 0

/** @brief The array element specifies a loadable segment, described by p_filesz and
p_memsz. The bytes from the file are mapped to the beginning of the memory
segment. */
#define PT_LOAD 1
#define PT_DYNAMIC 2
#define PT_INTERP 3
#define PT_NOTE 4
#define PT_SHLIB 5
#define PT_PHDR 6

/***** Section Types ******/
#define SHT_NULL 0
#define SHT_PROGBITS 1
#define SHT_SYMTAB 2
#define SHT_STRTAB 3
#define SHT_RELA 4
#define SHT_HASH 5
#define SHT_DYNAMIC 6
#define SHT_NOTE 7
#define SHT_NOBITS 8
#define SHT_REL 9
#define SHT_SHLIB 10
#define SHT_DYNSYM 11
#define SHT_LOPROC 12
#define SHT_HIPROC 13
#define SHT_LOUSER 14
#define SHT_HIUSER 15

/***** ELF Header Type *****/
#define ET_NONE 0
#define ET_REL 1
#define ET_EXEC 2
#define ET_DYN 3
#define ET_CORE 4

#define EI_NIDENT 16
#define EI_CLASS 4
#define EI_DATA 5

#define ELFCLASSNONE 0
#define ELFCLASS32 1
#define ELFCLASS64 2

#define ELFDATANONE 0
#define ELFDATA2LSB 1
#define ELFDATA2MSB 2

#define SHN_UNDEF 0

typedef uint16_t elf32_half;
typedef uint32_t elf32_word;
typedef int32_t elf32_sword;
typedef uint32_t elf32_addr;
typedef int32_t elf32_off;

/** @brief Program Header */
struct elf32_phdr
{
    /** @brief This member tells what kind of segment this array element describes or how to
     * interpret the array element's information. Type values and their meanings appear
     * below.*/
    elf32_word p_type;
    /** @brief This member gives the offset from the beginning of the file at which the first byte
     * of the segment resides*/
    elf32_off p_offset;
    /** @brief This member gives the virtual address at which the first byte of the segment resides
     * in memory.*/
    elf32_addr p_vaddr;
    elf32_addr p_paddr;
    elf32_word p_filesz;
    elf32_word p_memsz;
    elf32_word p_flags;
    elf32_word p_align;
} __attribute__((packed));

/** @brief Segment Header
*/
struct elf32_shdr
{
    elf32_word sh_name;
    elf32_word sh_type;
    elf32_word sh_flags;
    elf32_addr sh_addr;
    elf32_off sh_offset;
    /** @brief segment header size*/
    elf32_word sh_size;
    elf32_word sh_link;
    elf32_word sh_info;
    elf32_word sh_addralign;
    /** @brief total entry size */
    elf32_word sh_entsize;
} __attribute__((packed));

struct elf_header
{
    unsigned char e_ident[EI_NIDENT];
    /** @brief This member identifies the object file type. (relocatable, executable, shared etc)*/
    elf32_half e_type;
    elf32_half e_machine;
    elf32_word e_version;
    elf32_addr e_entry;
    elf32_off e_phoff;
    elf32_off e_shoff;
    elf32_word e_flags;
    elf32_half e_ehsize;
    elf32_half e_phentsize;
    /** @brief number of program header */
    elf32_half e_phnum;
    elf32_half e_shentsize;
    elf32_half e_shnum;
    elf32_half e_shstrndx;
} __attribute__((packed));

/** @brief Dynamic structure */
struct elf32_dyn
{
    elf32_sword d_tag;
    union 
    {
        elf32_word d_val;
        elf32_addr d_ptr;
    } d_un;
    
} __attribute__((packed));

/** @brief symbol definitions */
struct elf32_sym
{
    /** @brief index in the string table */
    elf32_word st_name;
    elf32_addr st_value;
    elf32_word st_size;
    unsigned char st_info;
    unsigned char st_other;
    elf32_half st_shndx;
} __attribute__((packed));

/** @brief the function returns the entry point of a process */
void* elf_get_entry_ptr(struct elf_header* elf_header);

/** @brief the function returns the entry point of a process */
uint32_t elf_get_entry(struct elf_header* elf_header);

#endif