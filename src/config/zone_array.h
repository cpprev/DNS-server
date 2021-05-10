#ifndef ZONE_ARRAY_H
#define ZONE_ARRAY_H

#include <stdio.h>

#include "config/zone.h"

typedef struct
{
    size_t size;
    size_t capacity;
    zone **zones;
} zone_array;

/**
** \brief                           Initializes a struct zone_array
** \param void
** \return                          The newly created zone_array
*/
zone_array *zone_array_init (void);

/**
** \brief                           Frees the memory inside a zone_array
** \param z_arr                     The zone_array to free
** \return                          void
*/
void zone_array_free (zone_array *z_arr);

/**
** \brief                           Resizes a zone_array's arr argument
** \param z_arr                     The zone_array to alter
** \return                          The modified zone_array
*/
void zone_array_resize (zone_array *z_arr);

/**
** \brief                           Adds a zone object to a zone_array
** \param z_arr                     The zone_array to alter
** \param z                         The string to add to the vector2
** \return                          The modified zone_array
*/
void zone_array_add_zone (zone_array *z_arr, zone *z);

#endif
