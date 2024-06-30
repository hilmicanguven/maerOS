#ifndef ISR80H_PROCESS_H
#define ISR80H_PROCESS_H

/** @file process.h
 * @brief create a kernel command that allows us to start processes.
*/

struct interrupt_frame;

/** @brief load new process(user process) and start its tasks*/
void* isr80h_command6_process_load_start(struct interrupt_frame* frame);

/** @brief parse command and pass to process..starting the process and injecting the arguments */
void* isr80h_command7_invoke_system_command(struct interrupt_frame* frame);

/** @brief get program/process arguments given via shell */
void* isr80h_command8_get_program_arguments(struct interrupt_frame* frame);

/** @brief exit/terminate the process/program and runs next task */
void* isr80h_command9_exit(struct interrupt_frame* frame);

#endif