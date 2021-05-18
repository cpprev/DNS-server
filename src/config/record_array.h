#ifndef RECORD_ARRAY_H
#define RECORD_ARRAY_H

#include "config/record_array.h"
#include "config/record.h"

typedef struct
{
    size_t size;
    size_t capacity;
    record **arr;
} record_array;

/**
** \brief                           Initializes a struct record_array
** \param void
** \return                          The newly created record_array
*/
record_array *record_array_init (void);

record_array *record_array_copy (record_array *r_arr);

/**
** \brief                           Frees the memory inside a record_array
** \param r_arr                     The record_array to free
** \return                          void
*/
void record_array_free (record_array *r_arr);

/**
** \brief                           Resizes a record_array's arr argument
** \param r_arr                     The record_array to alter
** \return                          The modified record_array
*/
void record_array_resize (record_array *r_arr);

/**
** \brief                           Adds a record object to a record_array
** \param r_arr                     The record_array to alter
** \param r                         The record to add to the record_array
** \return                          The modified record_array
*/
void record_array_add_record (record_array *r_arr, record *r);

void record_array_add_copied_record (record_array *r_arr, record *r);

#endif
