#ifndef STRING_H
#define STRING_H

#include <stdio.h>
#include <stdbool.h>

typedef struct
{
    char *arr;
    size_t size;
    size_t capacity;
} string;

/**
** \brief                           Initializes a string
** \return                          String initialized
*/
string *string_init (void);

/**
** \brief                           Frees the memory inside a string
** \param s                         The string to free
** \return                          void
*/
void string_free (string *s);

// TODO Comment
void string_flush (string *s);

/**
** \brief                           Resizes a string's arr argument
** \param s                         The string to alter
** \return                          The modified string
*/
void string_resize (string *s);

/**
** \brief                           Adds a char to a string
** \param s                         The string to alter
** \param c                         The character to add to the string
** \return                          The modified string
*/
void string_add_char(string *s, char c);

/**
** \brief                           Adds another string to a string object
** \param s                         The string to alter
** \param s2                        The string to add to the main string
** \return                          The modified string
*/
void string_add_str (string *s, char *s2);

// TODO comment
bool string_is_empty(string *s);

// TODO Comment
void string_copy(string **dst, string *src);

// TODO Comment
bool string_equals(string *s1, string *s2);

// TODO Comment
void string_pad_zeroes(string **s, int n);

#endif
