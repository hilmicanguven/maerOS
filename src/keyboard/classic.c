#include "classic.h"
#include "keyboard.h"
#include "io/io.h"
#include "kernel.h"
#include "idt/idt.h"
#include "task/task.h"

#include <stdint.h>
#include <stddef.h>

/** @brief the scancode for caps lock key in keyboard */
#define CLASSIC_KEYBOARD_CAPSLOCK 0x3A

/** @brief "Classic" keyboard initialization function */
int classic_keyboard_init();

/** @brief Scan Code Set 1: The following table shows which scan codes correspond to 
 * which keys when using scan code set 1 (for a "US QWERTY" keyboard only):
 * 
 * each scan code (index of this array) corresponds to ascii code(item in that index), 
 * we can fill these by looking osdev wiki ps2 page. */
static uint8_t keyboard_scan_set_one[] = {
    0x00, 0x1B, '1', '2', '3', '4', '5',
    '6', '7', '8', '9', '0', '-', '=',
    0x08, '\t', 'Q', 'W', 'E', 'R', 'T',
    'Y', 'U', 'I', 'O', 'P', '[', ']',
    0x0d, 0x00, 'A', 'S', 'D', 'F', 'G',
    'H', 'J', 'K', 'L', ';', '\'', '`', 
    0x00, '\\', 'Z', 'X', 'C', 'V', 'B',
    'N', 'M', ',', '.', '/', 0x00, '*',
    0x00, 0x20, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, '7', '8', '9', '-', '4', '5',
    '6', '+', '1', '2', '3', '0', '.'
};

/** @brief "Classic PS2" keyboard structure */
struct keyboard classic_keyboard = {
    .name = {"Classic"},
    .init = classic_keyboard_init
};

/** @brief interrupt handler triggered when keyboard is pressed*/
void classic_keyboard_handle_interrupt();

int classic_keyboard_init()
{
    // set interrupt handler for keyboard interrupt number
    idt_register_interrupt_callback(ISR_KEYBOARD_INTERRUPT, classic_keyboard_handle_interrupt);

    keyboard_set_capslock(&classic_keyboard, KEYBOARD_CAPS_LOCK_OFF);

    outb(PS2_PORT, PS2_COMMAND_ENABLE_FIRST_PORT);

    return 0;
}

/** @brief The function finds ascii corresponds to scancode by looking at array 'keyboard_scan_set_one' */
uint8_t classic_keyboard_scancode_to_char(uint8_t scancode)
{
    size_t size_of_keyboard_set_one = sizeof(keyboard_scan_set_one) / sizeof(uint8_t);
    if (scancode > size_of_keyboard_set_one)
    {
        return 0;
    }

    char c = keyboard_scan_set_one[scancode];
    if (keyboard_get_capslock(&classic_keyboard) == KEYBOARD_CAPS_LOCK_OFF)
    {
        if (c >= 'A' && c <= 'Z')
        {
            c += 32; /* make lower case to upper case according to ascii table */
        }
    }
    return c;
}

void classic_keyboard_handle_interrupt()
{
    kernel_page();
    uint8_t scancode = 0;
    scancode = insb(KEYBOARD_INPUT_PORT);
    insb(KEYBOARD_INPUT_PORT); //this reading is just for ignore 1 byte (this byte is other information that we do not care about)

    if(scancode & CLASSIC_KEYBOARD_KEY_RELEASED)
    {
        // key is released, no other key pressing is coming 
        return;
    }

    if (scancode == CLASSIC_KEYBOARD_CAPSLOCK)
    {
        KEYBOARD_CAPS_LOCK_STATE old_state = keyboard_get_capslock(&classic_keyboard);
        keyboard_set_capslock(&classic_keyboard, old_state == KEYBOARD_CAPS_LOCK_ON ? KEYBOARD_CAPS_LOCK_OFF : KEYBOARD_CAPS_LOCK_ON);
    }

    uint8_t c = classic_keyboard_scancode_to_char(scancode);
    if (c != 0)
    {
        keyboard_push(c); // push keyword to buffer
    }

    task_page();

}

/** @brief the function returns classic keyboard structure */
struct keyboard* classic_init()
{
    return &classic_keyboard;
}