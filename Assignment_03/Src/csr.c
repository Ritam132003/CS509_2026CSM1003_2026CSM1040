#include <stdlib.h>
#include "../Include/csr.h"

CSRGraph *create_csr(int vertices, int edges, int **adjacency, int *degree)
{
    CSRGraph *graph = malloc(sizeof(CSRGraph));
    if (graph == NULL) return NULL;

    graph->vertices = vertices;
    graph->edges = edges;
    graph->row_ptr = malloc((size_t)(vertices + 1) * sizeof(int));

    if (graph->row_ptr == NULL) {
        free(graph);
        return NULL;
    }

    graph->row_ptr[0] = 0;
    for (int i = 0; i < vertices; ++i)
        graph->row_ptr[i + 1] = graph->row_ptr[i] + degree[i];

    int total = graph->row_ptr[vertices];

    graph->col_idx = total > 0 ? malloc((size_t)total * sizeof(int)) : NULL;
    graph->values  = total > 0 ? malloc((size_t)total * sizeof(int)) : NULL;

    if (total > 0 && (graph->col_idx == NULL || graph->values == NULL)) {
        free(graph->row_ptr);
        free(graph->col_idx);
        free(graph->values);
        free(graph);
        return NULL;
    }

    int k = 0;
    for (int u = 0; u < vertices; ++u) {
        for (int j = 0; j < degree[u]; ++j) {
            graph->col_idx[k] = adjacency[u][2 * j];
            graph->values[k] = adjacency[u][2 * j + 1];
            ++k;
        }
    }

    return graph;
}

void free_csr(CSRGraph *graph)
{
    if (graph == NULL) return;
    free(graph->row_ptr);
    free(graph->col_idx);
    free(graph->values);
    free(graph);
}
