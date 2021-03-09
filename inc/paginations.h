#if !defined(PAGINATIONS_H)
#define PAGINATIONS_H
#include "pager.h"

int fifo_pagination(pager_p p);
int second_chance_pagination(pager_p p);
int lru_pagination(pager_p p);
int ram_pagination(pager_p p);
int mod_pagination(pager_p p);

#endif // PAGINATIONS_H
