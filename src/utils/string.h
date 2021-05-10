#ifndef STRING_H
#define STRING_H

#include <stdio.h>

typedef struct
{
    char *arr;
    size_t size;
    size_t capacity;
} string;

/**
** \brief                           Initializes a vector
** \return                          vector initialized
*/
string string_init (void);

/**
** \brief                           Resizes a struct vect's arr argument
** \param s                         The vect to alter
** \return                          The modified string
*/
string string_resize (string s);

/**
** \brief                           Adds a char to a vector
** \param s                         The vector to alter
** \param c                         The character to add to the vector
** \return                          The modified string
*/
string string_add_char(string s, char c);

/**
** \brief                           Adds a string to a vector
** \param s                         The vector to alter
** \param s2                        The string to add to the vector
** \return                          The modified string
*/
string string_add_str (string s, char *s2);

#endif
