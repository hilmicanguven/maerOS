#ifndef STATUS_H
#define STATUS_H

#define MAEROS_ALL_OK 0
#define EIO 1

/**
 * @brief Error invalid argument
*/
#define EINVARG 2

/**
 * @brief Error no memory
*/
#define ENOMEM 3
#define EBADPATH 4
#define EFSNOTUS 5
/** @brief Error read-only occurs when read-only file is accessed */
#define ERDONLY 6
#define EUNIMP 7

/** @brief Error process slot is already taken and can not be loaded again */
#define EISTKN 8
/** @brief Error invalid format, i.e. we expect .ELF file but it is not .ELF, it is .bin*/
#define EINFORMAT 9


#endif