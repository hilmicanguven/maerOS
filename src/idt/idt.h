#ifndef IDT_H
#define IDT_H

#include <stdint.h>

struct interrupt_frame;

/** @brief function prototypes for different commands */
typedef void*(*ISR80H_COMMAND)(struct interrupt_frame* frame);
typedef void(*INTERRUPT_CALLBACK_FUNCTION)();

/**
 * @brief interrupt vector descriptor, where each member refers to
 * bit fields. For example, higher and lower offset members creates interrupt handler etc.
 *
 * @note packed attribute is used to keep order of members same in case of
 * compiler optimize and changes order for alignment issues
*/
struct idt_desc
{
    uint16_t offset_1; // Offset bits 0 - 15
    uint16_t selector; // Selector thats in our GDT
    uint8_t zero; // Does nothing, unused set to zero
    uint8_t type_attr; // Descriptor type and attributes
    uint16_t offset_2; // Offset bits 16-31
} __attribute__((packed));

/**
 * @brief Interrupt vector table descriptor
*/
struct idtr_desc
{
    uint16_t limit; // Size of descriptor table -1
    uint32_t base; // Base address of the start of the interrupt descriptor table
} __attribute__((packed));

/** @brief interrupt frame is the structure which holds required registers
 * during communication/traveling between user and kernel land
*/
struct interrupt_frame
{
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t reserved;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    uint32_t ip;
    uint32_t cs;
    uint32_t flags;
    uint32_t esp;
    uint32_t ss;
} __attribute__((packed));

/**
 * @brief interrupt descriptor table initialization function. Descriptor table
 * base and limit address are set. Some fundamental interrupts may be added to
 * table. Finally, table is loaded to CPU
*/
void idt_init();

/**
 * @brief create descriptor for the given interrupt number at interrupt table
 * descriptor fields are filled according to idt entry format.
*/
void idt_set(int interrupt_no, void* address);

/** @brief Enabling interrupts (in asm file) */
void enable_interrupts();
/** @brief Disabling interrupts (in asm file) */
void disable_interrupts();

/** @brief add 0x80 command to the command array */
void isr80h_register_command(int command_id, ISR80H_COMMAND command);
int idt_register_interrupt_callback(int interrupt, INTERRUPT_CALLBACK_FUNCTION interrupt_callback);

#endif