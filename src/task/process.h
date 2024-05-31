#ifndef PROCESS_H
#define PROCESS_H
#include <stdint.h>
#include <stdbool.h>

#include "task.h"
#include "config.h"

/** @brief Process is accepted as elf file format */
#define PROCESS_FILETYPE_ELF 0
/** @brief Process is accepted as binary file */
#define PROCESS_FILETYPE_BINARY 1

typedef unsigned char PROCESS_FILETYPE;

struct process_allocation
{
    void* ptr;
    size_t size;
};

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

/** @brief The structure to define a what process is (can it be called PCB Process Control Block?)*/
struct process
{
    /** @brief The process id */
    uint16_t id;

    /** @brief file name? */
    char filename[MAEROS_MAX_PATH];

    // The main process task
    struct task* task;

    /** @brief The memory (malloc) allocations that have been made of the process 
    When process is killed, free the memory previously allocated */
    struct process_allocation allocations[MAEROS_MAX_PROGRAM_ALLOCATIONS];

    /** @brief Process file type can be .elf or .bin */
    PROCESS_FILETYPE filetype;

    /** @brief process type can be changed -> .txt, .elf, ..*/
    union
    {
        /** @brief The physical pointer to the process loaded in memory. 
         * It contains code and data, assuming binary process (no section or other headers)
        */
        void* ptr;
        /** @brief The physical pointer to elf file */
        struct elf_file* elf_file;
    };
    

    /** @brief The physical pointer to the stack memory */
    void* stack;

    /** @brief The size of the data pointed to by "ptr" */
    uint32_t size;
    /** @brief keyboard buffer structure*/
    struct keyboard_buffer
    {
        char buffer[MAEROS_KEYBOARD_BUFFER_SIZE];
        int tail;
        int head;
    } keyboard;

    // The arguments of the process.
    struct process_arguments arguments;
};

int process_switch(struct process* process);
int process_load_switch(const char* filename, struct process** process);
int process_load(const char* filename, struct process** process);
int process_load_for_slot(const char* filename, struct process** process, int process_slot);
struct process* process_current();
struct process* process_get(int process_id);
void* process_malloc(struct process* process, size_t size);
void process_free(struct process* process, void* ptr);

void process_get_arguments(struct process* process, int* argc, char*** argv);
int process_inject_arguments(struct process* process, struct command_argument* root_argument);
int process_terminate(struct process* process);

#endif