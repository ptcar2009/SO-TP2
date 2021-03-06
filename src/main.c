#include "pager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "pagination_map.h"
#include "paginations.h"

int main(int argc, char const *argv[])
{
    clock_t t;
    t = clock();
    char *mem_sizes, *board_sizes, *algorithms, *files;
    pagination_func_map_p m = pagination_func_map_p_new();

    pagination_func_map_p_add(m, "lru\0", lru_pagination);
    pagination_func_map_p_add(m, "2a\0", second_chance_pagination);
    pagination_func_map_p_add(m, "fifo\0", fifo_pagination);
    pagination_func_map_p_add(m, "ram\0", ram_pagination);
    pagination_func_map_p_add(m, "mod\0", ram_pagination);
    int verbosity = 0;
#ifndef DEBUG
    mem_sizes = argv[4];
    board_sizes = argv[3];
    algorithms = argv[1];
    files = argv[2];
    if ( argc > 5) {
        verbosity = atoi(argv[5]);
    }
#else
    mem_sizes = "128\0";
    algorithms = "lru\0";
    board_sizes = "2\0";
    files = "test/compilador.log\0";
#endif

    int board_size = atoi(board_sizes), mem_size = atoi(mem_sizes);
    pager_p p = get_pager(board_size, mem_size, pagination_func_map_p_get(m, algorithms), verbosity);
    if (verbosity == -1){
        printf("%s,", files);
        printf("%s,", algorithms);
        printf("%d,", board_size);
        printf("%d,", mem_size);
    }else{
        printf("arquivo: %s\n", files);
        printf("memoria total: %d\n", mem_size);
        printf("tamanho da página: %d\n", board_size);
        printf("algoritmo: %s\n", algorithms);
    }

    unsigned addr;
    char rw;
    FILE *file = fopen(files, "r");

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
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 
    if(verbosity == -1){
        printf("%d,", p->reads);
        printf("%d,", p->writes);
        printf("%d,", p->faults);
        printf("%d,", p->count_dirty);
        printf("%f\n", time_taken);
    }else{
        printf("lidas: %d\n", p->reads);
        printf("escritas: %d\n", p->writes);
        printf("faults: %d\n", p->faults);
        printf("dirty: %d\n", p->count_dirty);
        print_table(p);
    }
    delete_pager(p);
    pagination_func_map_p_delete(m);
    fclose(file);
    return 0;
}
