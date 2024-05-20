#ifndef ISR80H_MISC_H
#define ISR80H_MISC_H

/** @note misc, miscallaneous*/

struct interrupt_frame;

/** @brief The function sums two variable (yeah it is simplest example and helloWorld command)*/
void* isr80h_command0_sum(struct interrupt_frame* frame);

#endif