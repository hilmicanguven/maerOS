#include "idt.h"
#include "config.h"
#include "kernel.h"
#include "memory/memory.h"

/**
 * @brief The table (IDT Interrupt Descriptor Table) which holds 
 * interrupts with their features
*/
struct idt_desc idt_descriptors[MAEROS_TOTAL_INTERRUPTS];

/**
 * @brief The descriptor to show where IDT is and how many interrupts 
 * in it. This descriptor will be given to processor by `lidt` 
 * instruction. See idt.asm for details.
*/
struct idtr_desc idtr_descriptor;

/**
 * @brief load interrupt descriptor table via assembly instruction
*/
extern void idt_load(struct idtr_desc* ptr);

/**
 * @brief Handler for interrupt number zero
*/
void idt_zero()
{
    print("Divide by zero error\n");
}

void idt_set(int interrupt_no, void* address)
{
    struct idt_desc* desc = &idt_descriptors[interrupt_no];
    desc->offset_1 = (uint32_t) address & 0x0000ffff; //lower address of the handler
    desc->selector = KERNEL_CODE_SELECTOR;
    desc->zero = 0x00;
    desc->type_attr = 0xEE; //attributes for 
    desc->offset_2 = (uint32_t) address >> 16; //higher address of the handler
}

void idt_init()
{
    memset(idt_descriptors, 0, sizeof(idt_descriptors));
    idtr_descriptor.limit = sizeof(idt_descriptors) -1;
    idtr_descriptor.base = (uint32_t) idt_descriptors;

    // for (int i = 0; i < MAEROS_TOTAL_INTERRUPTS; i++)
    // {
    //     idt_set(i, interrupt_pointer_table[i]);
    // }

    idt_set(0, idt_zero);
    // idt_set(0x80, isr80h_wrapper);


    // for (int i = 0; i < 0x20; i++)
    // {
    //     idt_register_interrupt_callback(i, idt_handle_exception);
    // }
    

    // idt_register_interrupt_callback(0x20, idt_clock);

    // Load the interrupt descriptor table
    idt_load(&idtr_descriptor);
}