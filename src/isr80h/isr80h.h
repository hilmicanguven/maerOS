#ifndef ISR80H_H
#define ISR80H_H


/** @brief Kernel system command list (each command corresponds to
 * specific request from kernel)*/
enum SystemCommands
{
    /** @brief syscall to sum two numbers */
    SYSTEM_COMMAND0_SUM,
    /** @brief syscall to print message to screen */
    SYSTEM_COMMAND1_PRINT,
    /** @brief syscall to get keyword which is pressed */
    SYSTEM_COMMAND2_GETKEY,
    /** @brief syscall to put characters to screen */
    SYSTEM_COMMAND3_PUTCHAR,
    /** @brief syscall to allocate memory */
    SYSTEM_COMMAND4_MALLOC,
    /** @brief syscall to free memory*/
    SYSTEM_COMMAND5_FREE,
    /** @brief syscall to load user process and start its task */
    SYSTEM_COMMAND6_PROCESS_LOAD_START,
    /** @brief syscall to handle system commands entered by user via shell*/
    SYSTEM_COMMAND7_INVOKE_SYSTEM_COMMAND,
    /** @brief syscall to get program argument into process*/
    SYSTEM_COMMAND8_GET_PROGRAM_ARGUMENTS,
    /** @brief ssycall to exit a program */
    SYSTEM_COMMAND9_EXIT
};

void isr80h_register_commands();

#endif