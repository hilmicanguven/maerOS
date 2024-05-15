#ifndef GDT_H
#define GDT_H
#include <stdint.h>

/** @brief The Global Descriptor Table (GDT) is a binary data structure 
 * specific to the IA-32 and x86-64 architectures. 
 * It contains entries telling the CPU about memory segments. 
 * A similar Interrupt Descriptor Table exists containing task and 
 * interrupt descriptors. 
 * 
 * @note The GDT is pointed to by the value in the GDTR register. 
 * This is loaded using the LGDT assembly instruction, 
 * whose argument is a pointer to a GDT Descriptor structure 
 * 
 * @note The first entry in the GDT (Entry 0) should always be null 
 * and subsequent entries should be used instead. */
struct gdt 
{
    uint16_t segment;
    uint16_t base_first;
    uint8_t base;
    uint8_t access;
    uint8_t high_flags;
    uint8_t base_24_31_bits;
} __attribute__((packed));

/** @brief GDT structured is a GDT than we can read, not CPU 
 * 
 * @note Entries in the table are accessed by Segment Selectors, 
 * which are loaded into Segmentation registers either by assembly 
 * instructions or by hardware functions such as Interrupts.
 * 
 * Segment Descriptors: Each entry in the table has a complex structure: 
 * consists of base, limit, flags, access bytes
 * 
 * Base: A 32-bit value containing the linear address where the segment begins.
 * Limit: A 20-bit value, tells the maximum addressable unit, either in 1 byte units, or in 4KiB pages. 
*/
struct gdt_structured
{
    /** @brief Base Address */
    uint32_t base;
    /** @brief Limit Address*/
    uint32_t limit;
    /** @brief Access Byte*/
    uint8_t type;
};

/** @brief load gdt that is CPU can read */
void gdt_load(struct gdt* gdt, int size);

/** @brief convert structured gdt to normal GDT */
void gdt_structured_to_gdt(struct gdt* gdt, struct gdt_structured* structured_gdt, int total_entires);

#endif