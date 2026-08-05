#include <stdio.h>
#include <stdlib.h>

#include "dfs.h"

static void dfsVisit(CSRGraph *graph,
                     int vertex,
                     int *visited,
                     FILE *out)
{
    visited[vertex] = 1;

    printf("%d ", vertex);
    fprintf(out, "%d ", vertex);

    for (int i = graph->row_ptr[vertex];
         i < graph->row_ptr[vertex + 1];
         i++)
    {
        int next = graph->col_idx[i];

        if (!visited[next])
        {
            dfsVisit(graph,
                     next,
                     visited,
                     out);
        }
    }
}

void DFS(CSRGraph *graph, int source, FILE *out)
{
    int *visited = (int *)calloc(graph->V, sizeof(int));

    if (visited == NULL)
    {
        printf("Memory allocation failed.\n");
        fprintf(out, "Memory allocation failed.\n");
        return;
    }

    printf("\nAlgorithm : DFS\n");
    printf("Source : %d\n", source);
    printf("Traversal : ");

    fprintf(out, "Algorithm : DFS\n");
    fprintf(out, "Source : %d\n", source);
    fprintf(out, "Traversal : ");

    dfsVisit(graph,
             source,
             visited,
             out);

    printf("\n");
    fprintf(out, "\n");

    free(visited);
}