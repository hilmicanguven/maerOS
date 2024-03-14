#ifndef IO_H
#define IO_H

/**
 * @brief read one byte from the given port
*/
unsigned char insb(unsigned short port);

/**
 * @brief read one word from the given port
*/
unsigned short insw(unsigned short port);

/**
 * @brief write one byte to the given port
*/
void outb(unsigned short port, unsigned char val);

/**
 * @brief write one word to the given port
*/
void outw(unsigned short port, unsigned short val);

#endif