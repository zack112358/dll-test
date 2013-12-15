/**
 * @file dll-test.c
 * @brief Test program for dll library
**/

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "dll/dll.h"

#define N_LISTS 24

#define LINE_LEN 200

typedef struct {
    int value;
    dll_link_t links[N_LISTS];
} node_t;

int main(int argc, char **argv) {
    FILE *testfile = fopen("dll-test.in", "r");
    assert(testfile);

    int n_nodes;
    assert(fscanf(testfile, "n_nodes:%d\n", &n_nodes) == 1);

    node_t *nodes_a = malloc(sizeof(node_t) * n_nodes);
    assert(nodes_a);

    for (int i = 0; i < n_nodes; ++i) {
        nodes_a[i].value = i;
        for (int j = 0; j < N_LISTS; ++j) {
            dll_init_link(&nodes_a[i].links[j]);
        }
    }

    dll_root_t *lists_a = malloc(sizeof(dll_root_t) * N_LISTS);
    assert (lists_a);

    for (int i = 0; i < N_LISTS; ++i) {
        dll_init_root(&lists_a[i]);
    }

    char line[LINE_LEN];

    int n_ops = 0;
    while (fgets(line, LINE_LEN, testfile)) {
        int list_idx = 0;
        int node_idx = 0;
        int node_b_idx = 0;
        int result_idx = 0;
        ptrdiff_t offset = 0;
        dll_root_t *list_p;
        node_t *node_p;
        node_t *node_b_p;
        node_t *result_p;
#define CONV() do { \
            list_p = lists_a + list_idx; \
            node_p = nodes_a + node_idx; \
            node_b_p = nodes_a + node_b_idx; \
            result_p = nodes_a + result_idx; \
            offset = ((char*) &((node_t*) NULL)->links[list_idx]) - \
                     ((char*) NULL); \
        } while (0) 
        if (sscanf(line, "push_head %d %d", &list_idx, &node_idx) == 2) {
            CONV();
            dll_push_head(offset, list_p, node_p);
        } else if (sscanf(line, "push_tail %d %d", &list_idx, &node_idx) == 2) {
            CONV();
            dll_push_tail(offset, list_p, node_p);
        } else if (sscanf(line, "pop_head %d -> %d", &list_idx, &result_idx) == 2) {
            CONV();
            assert(dll_pop_head(offset, list_p) == result_p);
        } else if (sscanf(line, "pop_tail %d -> %d", &list_idx, &result_idx) == 2) {
            CONV();
            assert(dll_pop_tail(offset, list_p) == result_p);
        } else if (sscanf(line, "head %d -> %d", &list_idx, &result_idx) == 2) {
            CONV();
            assert(dll_head(offset, list_p) == result_p);
        } else if (sscanf(line, "tail %d -> %d", &list_idx, &result_idx) == 2) {
            CONV();
            assert(dll_tail(offset, list_p) == result_p);
        } else if (sscanf(line, "next %d %d -> %d", &list_idx, &node_idx, &result_idx) == 3) {
            CONV();
            assert(dll_next(offset, node_p) == result_p);
        } else if (sscanf(line, "prev %d %d -> %d", &list_idx, &node_idx, &result_idx) == 3) {
            CONV();
            assert(dll_prev(offset, node_p) == result_p);
        } else if (sscanf(line, "ins_after %d %d %d",
                   &list_idx, &node_idx, &node_b_idx) == 3) {
            CONV();
            dll_ins_after(offset, node_p, node_b_p);
        } else if (sscanf(line, "ins_before %d %d %d",
                   &list_idx, &node_idx, &node_b_idx) == 3) {
            CONV();
            dll_ins_before(offset, list_p, node_p, node_b_p);
        } else if (sscanf(line, "remove %d %d", &list_idx, &node_idx) == 2) {
            CONV();
            dll_remove(offset, list_p, node_p);
        } else {
            assert(!"Couldn't parse line");
        }

        ++n_ops;
        fprintf(stderr, "\rLine %d     ", n_ops + 2);
    }

    printf("Success: %d ops performed\n", n_ops);
}
