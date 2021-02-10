#include "pager.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * @brief Singleton for a pager object
 * 
 */
pager_p __p = 0;

unsigned hash_func(hashmap_p h, void *key)
{
    return (unsigned)key;
}
unsigned comp_func(void *k1, void *k2)
{
    return (unsigned)k1 == (unsigned)k2;
}

/**
 * @brief Get the pager object  
 * 
 * @return pager_p 
 */
pager_p get_pager(int board_size, int total_size, pagination_func_t func)
{
    if (__p)
        return __p;
    __p = malloc(sizeof(pager_t));
    __p->n_boards = total_size / board_size;
    __p->map = new_hashmap_p(100, hash_func, comp_func);
    __p->page_shift = 10;
    __p->func = func;

    while (board_size > 1)
    {
        __p->page_shift += 1;
        board_size >>= 1;
    }

    __p->boards = malloc(sizeof(board_p) * __p->n_boards);
    for (size_t i = 0; i < __p->n_boards; i++)
    {
        __p->boards[i] = malloc(sizeof(board_t));
        __p->boards[i]->last_used = 0;
        __p->boards[i]->current_page = 0;
        __p->boards[i]->flags = 0;
    }

    __p->faults = __p->reads = __p->writes = 0;

    return __p;
}

void read_page(pager_p p, int address)
{
    int page = address >> p->page_shift;
    board_p current = hashmap_p_get(p->map, page);
    if (!current || current->current_page != page)
    {
        int index = paginate(p);
        current = p->boards[index];
        current->current_page = page;
        p->faults++;
        hashmap_p_add(p->map, page, current);
    }

    p->reads++;
    current->flags |= 2;
    current->last_used = clock();
}

void write_page(pager_p p, int address)
{
    int page = address >> p->page_shift;
    board_p current = hashmap_p_get(p->map, page);
    if (!current || current->current_page != page)
    {
        int index = paginate(p);
        current = p->boards[index];
        current->current_page = page;
        hashmap_p_add(p->map, (void *)page, current);
    }

    p->writes++;
    current->flags |= 3;
    current->last_used = clock();
}

/**
 * @brief Applies the on-demand pagination function for the paginator
 * 
 * @param p paginator object that contains the function
 * @param page page that is trying to be accessed
 * @return int the address of the board that will be accessed
 */
int paginate(pager_p p)
{
    return p->func(p);
}
