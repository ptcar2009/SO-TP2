#if !defined(PAGER_H)
#define PAGER_H
#include "time.h"
#include "hashmap.h"
#include <stdio.h>

typedef struct board board_t;
typedef board_t *board_p;
struct board
{
    int current_page;
    clock_t last_used;
    char flags;
};

typedef struct pager pager_t;
typedef pager_t *pager_p;

typedef int (*pagination_func_t)(pager_p);

struct pager
{
    pagination_func_t func;
    hashmap_p map;
    board_p *boards;
    int page_shift;
    int n_boards;
    int faults;
    int reads;
    int writes;
};

pager_p get_pager(int board_size, int total_size, pagination_func_t func);
int paginate(pager_p);
void write_page(pager_p p, int address);
void read_page(pager_p p, int address);

#endif // PAGER_H
