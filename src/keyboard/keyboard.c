#include "keyboard.h"
#include "status.h"
#include "kernel.h"
#include "task/process.h"
#include "task/task.h"
#include "classic.h"

/** @brief Keyboard list head, first keyboard in system */
static struct keyboard* keyboard_list_head = 0;
/** @brief Keyboard list last, last keyboard in system */
static struct keyboard* keyboard_list_last = 0;

/** @brief Initialize keyboard with classic keyboard */
void keyboard_init()
{
    keyboard_insert(classic_init());
}


int keyboard_insert(struct keyboard* keyboard)
{
    int res = 0;
    if (keyboard->init == 0)
    {
        res = -EINVARG;
        goto out;
    }

    // if there is already a keyboard added
    if (keyboard_list_last)
    {
        keyboard_list_last->next = keyboard;
        keyboard_list_last = keyboard;
    
    }
    else // it is the fist time keyboard inserted
    {
        keyboard_list_head = keyboard;
        keyboard_list_last = keyboard;
    }
    
    res = keyboard->init();
out:
    return res;
}

/** @brief return tail index in keyboard buffer */
static int keyboard_get_tail_index(struct process* process)
{
    return process->keyboard.tail % sizeof(process->keyboard.buffer);
}

/** @brief buffer behaviour when backspace is pressed (remove last written char from the buffer)*/
void keyboard_backspace(struct process* process)
{
    // take tail pointer back for one character because Tail is equal to
    // last written character index + 1
    process->keyboard.tail -=1 ;

    int real_index = keyboard_get_tail_index(process);
    process->keyboard.buffer[real_index] = 0x00;
}

void keyboard_set_capslock(struct keyboard* keyboard, KEYBOARD_CAPS_LOCK_STATE state)
{
    keyboard->capslock_state = state;
}

KEYBOARD_CAPS_LOCK_STATE keyboard_get_capslock(struct keyboard* keyboard)
{
    return keyboard->capslock_state;
}

void keyboard_push(char c)
{
    struct process* process = process_current();
    if (!process)
    {
        return;
    }

    if(c == 0)
    {
        return;
    }

    int real_index = keyboard_get_tail_index(process);
    process->keyboard.buffer[real_index] = c;
    process->keyboard.tail++;
}

char keyboard_pop()
{
    if (!task_current())
    {
        return 0;
    }

    struct process* process = task_current()->process;
    int real_index = process->keyboard.head % sizeof(process->keyboard.buffer);
    // get character from keyboard buffer
    char c = process->keyboard.buffer[real_index];
    if (c == 0x00)
    {
        // Nothing to pop return zero.
        return 0;
    }

    process->keyboard.buffer[real_index] = 0;
    process->keyboard.head++;
    return c;
}