#include "pagination_map.h"
#include <stdlib.h>

int pagination_func_map_p_add(pagination_func_map_p m, char *key, pagination_func_t func)
{
    hashmap_p_add(m->map, key, func);
}

pagination_func_t pagination_func_map_p_get(pagination_func_map_p m, const char *key)
{
    return hashmap_p_get(m->map, key);
}

int hash_str(void *key)
{
    int tot = 0;
    char *s = (char *)key;
    for (size_t i = 0; s[i] != '\0'; i++)
    {
        tot += s[i];
    }
    return tot;
}

char compare_str(void *i1, void *i2)
{
    return !strcmp((char *)i1, (char *)i2);
}

pagination_func_map_p pagination_func_map_p_new()
{
    pagination_func_map_p m = malloc(sizeof(pagination_func_map_t));
    m->map = new_hashmap_p(10, hash_str, compare_str);
    return m;
}
