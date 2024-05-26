#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KEYBOARD_CAPS_LOCK_ON 1
#define KEYBOARD_CAPS_LOCK_OFF 0

typedef int KEYBOARD_CAPS_LOCK_STATE;

struct process;

typedef int (*KEYBOARD_INIT_FUNCTION)();

/** @brief Represents the keyboard structure */
struct keyboard
{
    /** @brief keyboard specific initialization function */
    KEYBOARD_INIT_FUNCTION init;
    /** @brief name of the function, apple keyboard, logitech keyboard etc*/
    char name[20];    
    KEYBOARD_CAPS_LOCK_STATE capslock_state;
    /** @brief next keyboard */
    struct keyboard* next;
};

/** @brief */
void keyboard_init();

/** @brief */
void keyboard_backspace(struct process* process);

/** @brief add a character to keyboard buffer */
void keyboard_push(char c);

/** @brief remove a character to keyboard buffer */
char keyboard_pop();

/** @brief add a keyboard to keyboard linked list. Note that
 * keyboard must have initialization function
 */
int keyboard_insert(struct keyboard* keyboard);

void keyboard_set_capslock(struct keyboard* keyboard, KEYBOARD_CAPS_LOCK_STATE state);
KEYBOARD_CAPS_LOCK_STATE keyboard_get_capslock(struct keyboard* keyboard);

#endif