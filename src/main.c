#include "pager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pagination_map.h"
#include "paginations.h"

int main(int argc, char const *argv[])
{
    pagination_func_map_p m = pagination_func_map_p_new();

    pagination_func_map_p_add(m, "lru\0", lru_pagination);
    pagination_func_map_p_add(m, "2a\0", second_chance_pagination);
    pagination_func_map_p_add(m, "fifo\0", fifo_pagination);

    int board_size = atoi(argv[3]), mem_size = atoi(argv[4]);
    pager_p p = get_pager(board_size, mem_size, pagination_func_map_p_get(m, argv[1]));

    printf("arquivo: %s\n", argv[2]);
    printf("memoria total: %d\n", mem_size);
    printf("tamanho da página: %d\n", board_size);
    printf("algoritmo: %s\n", argv[1]);

    unsigned addr;
    char rw;
    FILE *file = fopen(argv[2], "r");

    while (fscanf(file, "%x %c", &addr, &rw) != EOF)
    {
        switch (rw)
        {
        case 'W':
            write_page(p, addr);
            break;

        case 'R':
            read_page(p, addr);
            break;

        default:
            break;
        }
    }
    printf("lidas: %d\n", p->reads);
    printf("escritas: %d\n", p->writes);
    printf("faults: %d\n", p->faults);

    return 0;
}
