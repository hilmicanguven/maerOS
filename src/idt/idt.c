#include "idt.h"
#include "config.h"
#include "kernel.h"
#include "memory/memory.h"
#include "io/io.h"
#include "task/task.h"

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

extern void* interrupt_pointer_table[MAEROS_TOTAL_INTERRUPTS];

static INTERRUPT_CALLBACK_FUNCTION interrupt_callbacks[MAEROS_TOTAL_INTERRUPTS];

/** @brief The array where we add 0x80 interrupt commands */
static ISR80H_COMMAND isr80h_commands[MAEROS_MAX_ISR80H_COMMANDS];



/**
 * @brief load interrupt descriptor table via assembly instruction
*/
extern void idt_load(struct idtr_desc* ptr);

/**
 * @brief The function responsible for calling the 0x21 interrupt handler function 
*/
extern void int21h();

/**
 * @brief when there is no interrupt set, decide what will happen in Assembly code
*/
extern void no_interrupt();

/** @brief .asm routine triggered when 0x80 interrupt occurs */
extern void isr80h_wrapper();

/** @brief handler for interrupt 0x21 -> keyboard interrupt */
void int21_handler()
{
    print("Keyboard Pressed! \n");
    outb(0x20, 0x20);   //end of the interrupt
}

/**
 * @brief when interrupt is not set, this handler is invoked
*/
void no_interrupt_handler()
{
    outb(0x20, 0x20);   //end of the interrupt
}

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

    for (int i = 0; i < MAEROS_TOTAL_INTERRUPTS; i++)
    {
        idt_set(i, no_interrupt);
    }

    idt_set(0, idt_zero);
    idt_set(0x21, int21_handler);
    idt_set(0x80, isr80h_wrapper);
    
    /**
     * 0x20 Timer interrupt kaynağına denk geliyor.
     * Bu şekilde handler bağlarsak sürekli kesme üretir
     * kesme belirli aralıklarla geldigi icin
    */
    //idt_set(0x20, int21_handler);
    


    // for (int i = 0; i < 0x20; i++)
    // {
    //     idt_register_interrupt_callback(i, idt_handle_exception);
    // }
    

    // idt_register_interrupt_callback(0x20, idt_clock);

    // Load the interrupt descriptor table
    idt_load(&idtr_descriptor);
}

// int idt_register_interrupt_callback(int interrupt, INTERRUPT_CALLBACK_FUNCTION interrupt_callback)
// {
//     if (interrupt < 0 || interrupt >= MAEROS_TOTAL_INTERRUPTS)
//     {
//         return -EINVARG;
//     }

//     interrupt_callbacks[interrupt] = interrupt_callback;
//     return 0;
// }

/** @brief add syscall to commands table 
 * @note The kernel commands should be reserved only for a
 *  very strict kernel operations for things 
*/
void isr80h_register_command(int command_id, ISR80H_COMMAND command)
{
    if (command_id < 0 || command_id >= MAEROS_MAX_ISR80H_COMMANDS)
    {
        panic("The command is out of bounds\n");
    }

    if (isr80h_commands[command_id])
    {
        panic("Your attempting to overwrite an existing command\n");
    }

    isr80h_commands[command_id] = command;
}

/** @brief call required command operation */
void* isr80h_handle_command(int command, struct interrupt_frame* frame)
{
    void* result = 0;

    if(command < 0 || command >= MAEROS_MAX_ISR80H_COMMANDS)
    {
        // Invalid command
        return 0;
    }

    ISR80H_COMMAND command_func = isr80h_commands[command];
    if (!command_func)
    {
        /* If we are not able to respond user command, which means we do not support for not
        return zero, but not error */
        return 0;
    }

    result = command_func(frame);
    return result;
}

/** @brief In Intel architecture, interrupt 0x80 is used for making system calls 
 * in Linux. When a software running in user mode wants to request a service 
 * from the kernel (which runs in privileged mode), 
 * it can do so by triggering interrupt 0x80.*/
void* isr80h_handler(int command, struct interrupt_frame* frame)
{
    void* res = 0;

    kernel_page();
    
    /* save registers */
    task_current_save_state(frame);

    res = isr80h_handle_command(command, frame);
    
    task_page();
    
    return res;
}