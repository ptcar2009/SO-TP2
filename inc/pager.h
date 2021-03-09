#if !defined(PAGER_H)
#define PAGER_H
#include "time.h"
#include "hashmap.h"
#include <stdio.h>

#define SECOND_CHANCE_FLAG (char)2
#define DIRTY_FLAG (char)1

typedef struct board board_t;
typedef board_t *board_p;
struct board
{
    unsigned current_page;
    clock_t last_used;
    char flags;
    char last_op;
};

typedef struct pager pager_t;
typedef pager_t *pager_p;

typedef unsigned (*pagination_func_t)(pager_p);

struct pager
{
    pagination_func_t func;
    hashmap_p map;
    board_p *boards;
    unsigned page_shift;
    unsigned n_boards;
    unsigned faults;
    unsigned reads;
    unsigned writes;
    unsigned index_mask;
    unsigned count_dirty;
    int verbose;
    unsigned faulted;
};

pager_p get_pager(unsigned board_size, unsigned total_size, pagination_func_t func, unsigned verbosity);
unsigned paginate(pager_p);
void write_page(pager_p p, unsigned address);
void read_page(pager_p p, unsigned address);
void print_table(pager_p p);
void delete_pager(pager_p p);

#endif // PAGER_H
