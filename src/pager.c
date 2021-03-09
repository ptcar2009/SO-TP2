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
pager_p get_pager(unsigned board_size, unsigned total_size, pagination_func_t func, unsigned verbosity)
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

    unsigned temp = __p->n_boards;
    __p->index_mask = 0x80000000;

    __p->boards = malloc(sizeof(board_p) * __p->n_boards);
    for (size_t i = 0; i < __p->n_boards; i++)
    {
        __p->boards[i] = malloc(sizeof(board_t));
        __p->boards[i]->last_used = 0;
        __p->boards[i]->current_page = 0;
        __p->boards[i]->flags = 0;
    }

    __p->faults = __p->reads = __p->writes = __p->count_dirty = 0;
    __p->verbose = verbosity;

    return __p;
}

void read_page(pager_p p, unsigned address)
{
    unsigned page = address >> p->page_shift;
    unsigned current_index = hashmap_p_get(p->map, page);
    board_p current;

    if (current_index == 0)
    {
        p->faulted = page;
        unsigned index = paginate(p);
        current = p->boards[index];
        hashmap_p_remove(p->map, current->current_page);
        current->current_page = page;
        p->faults++;
        p->count_dirty += p->boards[index]->flags & 1;
        p->boards[index]->flags &= ~((int)1);
        hashmap_p_add(p->map, page, index + 1);
    }
    else
    {
        current = p->boards[current_index -1];
    }

    p->reads++;
    current->flags |= SECOND_CHANCE_FLAG;
    // current->last_op = "R";
    current->last_used = clock();
    if (p->verbose > 1){
        print_table(p);
    }
}

void write_page(pager_p p, unsigned address)
{
    unsigned page = address >> p->page_shift;
    unsigned current_index = hashmap_p_get(p->map, page);
    board_p current;

    if (current_index == 0)
    {
        p->faulted = page;
        unsigned index = paginate(p);
        current = p->boards[index];
        hashmap_p_remove(p->map, current->current_page);
        current->current_page = page;
        p->faults++;
        p->count_dirty += p->boards[index]->flags & DIRTY_FLAG;
        hashmap_p_add(p->map, page, index + 1);
    }
    else
    {
        current = p->boards[current_index -1];
    }

    p->writes++;
    current->last_used = clock();
    current->flags |= SECOND_CHANCE_FLAG; //sets second chance.
    current->flags |= DIRTY_FLAG; //sets dity beats.
    // current->last_op = "W";
    if (p->verbose > 1){
        print_table(p);
    }
}

void print_table(pager_p p)
{
    printf("key\t|    lastop\t|    dirtybit\n");
    for (size_t i = 0; i < p->map->_arr_len; i++)
    {
        hashmap_item_p cur = p->map->items[i];
        while (cur && cur->key)
        {
            printf("%x\t|\t%x\t|\t%d\n", cur->key, cur->content, p->boards[(int)cur->content-1]->flags & 1);
            cur = cur->next;
        }

    }
}
/**
 * @brief Applies the on-demand pagination function for the paginator
 * 
 * @param p paginator object that contains the function
 * @param page page that is trying to be accessed
 * @return unsigned the address of the board that will be accessed
 */
unsigned paginate(pager_p p)
{
    return p->func(p);
}

void delete_pager(pager_p p)
{
    hashmap_p_delete(p->map);
    for (size_t i = 0; i < p->n_boards; i++)
    {
        free(p->boards[i]);
    }
    free(p->boards);
    free(p);
}
