#ifndef TASKSWITCHSEGMENT_H
#define TASKSWITCHSEGMENT_H

#include <stdint.h>

/** @brief A Task State Segment (TSS) is a binary data structure 
 * specific to the IA-32 and x86-64 architectures. 
 * It holds information about a task.  */
struct tss
{
    uint32_t link;
    /** @brief Kernel stack pointer*/
    uint32_t esp0;
    /** @brief  Kernel stack segment */
    uint32_t ss0; 
    uint32_t esp1;
    uint32_t esp2;
    uint32_t ss2;
    uint32_t sr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint32_t es;
    uint32_t cs;
    uint32_t ss;
    uint32_t ds;
    uint32_t fs;
    uint32_t gs;
    uint32_t ldtr;
    uint32_t iopb;
} __attribute__((packed));

/** @brief load tss to CPU via assembly instructions */
void tss_load(int tss_segment);

#endif