#include <stdlib.h>
#include "hashmap.h"

int hash_map_get_index(hashmap_p h, const void *key)
{
    return h->hash_func(key) % h->_arr_len;
}

char hashmap_p_add(hashmap_p h, const void *key, const void *value)
{
    int index = hash_map_get_index(h, key);
    hashmap_item_p_add(h, h->items[index], key, value);
}

hashmap_item_p new_hashmap_item_p()
{
    hashmap_item_p item = malloc(sizeof(hashmap_item_t));
    item->next = item->key = item->content = 0;
    return item;
}

char hashmap_item_p_add(hashmap_p h, hashmap_item_p item, const void *key, const void *value)
{

    if (!item->key)
    {
        item->content = value;
        item->key = key;
        return 1;
    }
    if (h->comp_func(key, item->key))
    {
        item->content = value;
        return;
    }
    if (!item->next)
        item->next = new_hashmap_item_p();

    return hashmap_item_p_add(h, item->next, key, value);
}

void *hashmap_item_p_get(hashmap_p h, hashmap_item_p item, const void *key)
{

    if (!item->content)
    {
        return 0;
    }
    if (h->comp_func(key, item->key))
        return item->content;
    if (!item->next)
        return 0;

    return hashmap_item_p_get(h, item->next, key);
}

hashmap_p new_hashmap_p(int arr_size, hash_func_t hash, compare_func_t comp)
{
    hashmap_p h = malloc(sizeof(hashmap_t));
    h->comp_func = comp;
    h->hash_func = hash;
    h->items = malloc(arr_size * sizeof(hashmap_item_p));
    h->_arr_len = arr_size;
    for (size_t i = 0; i < arr_size; i++)
    {
        h->items[i] = new_hashmap_item_p();
    }
    return h;
}

void *hashmap_p_get(hashmap_p h, const void *key)
{
    unsigned index = hash_map_get_index(h, key);
    return hashmap_item_p_get(h, h->items[index], key);
}
