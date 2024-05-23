#ifndef TASK_H
#define TASK_H

#include "config.h"
#include "memory/paging/paging.h"

struct interrupt_frame;

/** @brief CPU registers 
 * 
 * @note an interrupt comes, store these registers
*/
struct registers
{
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;

    /** @brief Program Counter (PC) */
    uint32_t ip;
    uint32_t cs;
    uint32_t flags;
    uint32_t esp;
    uint32_t ss;
};


struct process;

/** @brief Task structure */
struct task
{
    /** @brief The page directory of the task */
    struct paging_4gb_chunk* page_directory;

    /** @brief The registers of the task when the task is not running */
    struct registers registers;

    /** @brief The process of the task */
    struct process* process;

    /** @brief The next task in the linked list */
    struct task* next;

    /** @brief Previous task in the linked list */
    struct task* prev;
};

/** @brief create a new task */
struct task* task_new(struct process* process);

struct task* task_current();
struct task* task_get_next();
int task_free(struct task* task);

int task_switch(struct task* task);

/** @brief Task page takes us out of the kernel page directory and 
 * loads us into the task page directory.*/
int task_page();
int task_page_task(struct task* task);

void task_run_first_ever_task();

/** @brief the function will drop us into userland */
void task_return(struct registers* regs);

/** @brief restore general purpose registers in assembly */
void restore_general_purpose_registers(struct registers* regs);
void user_registers();

/** @brief Save the current task state (registers) */
void task_current_save_state(struct interrupt_frame *frame);

int copy_string_from_task(struct task* task, void* virtual, void* phys, int max);

/** @brief Read an 'index'th item (arguments) from the stack */
void* task_get_stack_item(struct task* task, int index);
void* task_virtual_address_to_physical(struct task* task, void* virtual_address);
void task_next();

#endif