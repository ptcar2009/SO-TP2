#if !defined(PAGINATION_FACTORY_H)
#define PAGINATION_FACTORY_H

#include "pager.h"
#include "hashmap.h"

typedef struct pagination_func_map pagination_func_map_t;
typedef pagination_func_map_t *pagination_func_map_p;
struct pagination_func_map
{
    hashmap_p map;
};

int pagination_func_map_p_add(pagination_func_map_p m, char *key, pagination_func_t func);
pagination_func_t pagination_func_map_p_get(pagination_func_map_p m, const char *key);
pagination_func_map_p pagination_func_map_p_new();

#endif // PAGINATION_FACTORY_H
