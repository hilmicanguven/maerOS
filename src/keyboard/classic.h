#ifndef CLASSIC_KEYBOARD_H
#define CLASSIC_KEYBOARD_H


/** @file The PS/2 Keyboard is a device that talks to a PS/2 controller using 
 * serial communication. Ideally, each different type of PS/2 controller driver 
 * should provide some sort of standard/simple "send byte/receive byte" interface,
 *  and the PS/2 Keyboard driver would use this interface without caring about 
 * lower level details (like what type of PS/2 controller the device is plugged 
 * into).
 * 
 * The PS/2 Keyboard accepts commands and sends responses to those commands, and 
 * also sends scan codes indicating when a key was pressed or released. Keep in mind 
 * that these scan codes (from the tables below) may be altered (i.e. translated) by 
 * the PS/2 controller, depending on the controller's settings. 
 */

// See https://wiki.osdev.org/%228042%22_PS/2_Controller for below numbers

/** @brief It is PS2 Port (command register) to access(writing) for the first port */
#define PS2_PORT 0x64

/** @brief Command 0xAE is enable command  for the first port */
#define PS2_COMMAND_ENABLE_FIRST_PORT 0xAE

/** @brief scan code for key is released*/
#define CLASSIC_KEYBOARD_KEY_RELEASED 0x80

/** @brief keyboard interrupt number when any key is pressed */
#define ISR_KEYBOARD_INTERRUPT 0x21

/** @brief it is the data port of PS2 keyword */
#define KEYBOARD_INPUT_PORT 0x60

struct keyboard* classic_init();

#endif