#include "paginations.h"

int fifo_pagination(pager_p p)
{
    static int board = 0;
    return board = (board + 1) % p->n_boards;
}

int lru_pagination(pager_p p)
{
    int least_recent = 0;
    clock_t least_time = p->boards[0]->last_used;
    for (size_t i = 0; i < p->n_boards; i++)
    {
        if (p->boards[i]->last_used < least_time)
        {
            least_recent = i;
            least_time = p->boards[i]->last_used;
        }
    }
    return least_recent;
}

int second_chance_pagination(pager_p p)
{
    static int board = 0;
    while (p->boards[board]->flags & 2)
    {
        p->boards[board]->flags &= 0b111111101;
        board++;
        board = board % p->n_boards;
    }
    return board;
}
