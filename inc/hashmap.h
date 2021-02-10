#if !defined(HASHMAP_H)
#define HASHMAP_H

typedef struct hashmap_item_t hashmap_item_t;
typedef hashmap_item_t *hashmap_item_p;
struct hashmap_item_t
{
    void *key;
    void *content;
    hashmap_item_p next;
};

typedef unsigned (*hash_func_t)(const void *);
typedef char (*compare_func_t)(const void *, const void *);

typedef struct hashmap_t hashmap_t;
typedef hashmap_t *hashmap_p;
struct hashmap_t
{
    hashmap_item_p *items;
    hash_func_t hash_func;
    compare_func_t comp_func;
    int _arr_len;
};

char hashmap_item_p_add(hashmap_p h, hashmap_item_p item, const void *key, const void *value);
char hashmap_p_add(hashmap_p, const void *, const void *);
char hashmap_p_remove(hashmap_p, const void *);
void hashmap_p_delete(hashmap_p);
void *hashmap_p_get(hashmap_p h, const void *key);
hashmap_p new_hashmap_p(int arr_size, hash_func_t hash, compare_func_t comp);

#endif // HASHMAP_H
