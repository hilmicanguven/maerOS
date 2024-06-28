#ifndef ISR80H_HEAP_H
#define ISR80H_HEAP_H

struct interrupt_frame;

/** @brief syscall function to allocate memory */
void* isr80h_command4_malloc(struct interrupt_frame* frame);

/** @brief syscall function to free memory */
void* isr80h_command5_free(struct interrupt_frame* frame);

#endif