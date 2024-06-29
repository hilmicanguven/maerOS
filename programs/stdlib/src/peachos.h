#ifndef PEACHOS_H
#define PEACHOS_H
#include <stddef.h>
#include <stdbool.h>


struct command_argument
{
    char argument[512];
    struct command_argument* next;
};

struct process_arguments
{
    int argc;
    char** argv;
};

/** @brief print function implemented in stdlib 
 * notice that this also call syscal 1 to print to screen 
*/
void print(const char* filename);

/** @brief get pressed key by user implemented in standad library */
int peachos_getkey();

/** @brief the function calls malloc syscall function */
void* maeros_malloc(size_t size);
void maeros_free(void* ptr);
/** @brief stdlib put char to terminal */
void maeros_putchar(char c);
int peachos_getkeyblock();
void peachos_terminal_readline(char* out, int max, bool output_while_typing);

/** @brief the routine call syscall to load user process */
void maeros_process_load_start(const char* filename);
struct command_argument* peachos_parse_command(const char* command, int max);
void peachos_process_get_arguments(struct process_arguments* arguments);
int peachos_system(struct command_argument* arguments);
int peachos_system_run(const char* command);
void peachos_exit();
#endif