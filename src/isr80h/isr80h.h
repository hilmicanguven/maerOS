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
    SYSTEM_COMMAND2_GETKEY,
    SYSTEM_COMMAND3_PUTCHAR,
    SYSTEM_COMMAND4_MALLOC,
    SYSTEM_COMMAND5_FREE,
    SYSTEM_COMMAND6_PROCESS_LOAD_START,
    SYSTEM_COMMAND7_INVOKE_SYSTEM_COMMAND,
    SYSTEM_COMMAND8_GET_PROGRAM_ARGUMENTS,
    SYSTEM_COMMAND9_EXIT
};

void isr80h_register_commands();

#endif