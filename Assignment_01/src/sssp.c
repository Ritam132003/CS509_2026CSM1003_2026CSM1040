#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "sssp.h"

void SSSP(CSRGraph *graph, int source, FILE *out)
{
    int V = graph->V;

    int *distance = (int *)malloc(V * sizeof(int));
    int *visited = (int *)calloc(V, sizeof(int));

    if (distance == NULL || visited == NULL)
    {
        printf("Memory allocation failed.\n");
        fprintf(out, "Memory allocation failed.\n");
        return;
    }

    for (int i = 0; i < V; i++)
        distance[i] = INT_MAX;

    distance[source] = 0;

    printf("\nAlgorithm : SSSP \n");
    printf("Source : %d\n\n", source);

    fprintf(out, "Algorithm : SSSP \n");
    fprintf(out, "Source : %d\n\n", source);

    for (int count = 0; count < V; count++)
    {
        int u = -1;
        int minDistance = INT_MAX;

        /* Find the unvisited vertex with minimum distance */
        for (int i = 0; i < V; i++)
        {
            if (!visited[i] && distance[i] < minDistance)
            {
                minDistance = distance[i];
                u = i;
            }
        }

        if (u == -1)
            break;

        visited[u] = 1;

        /* Relax all neighbours */
        for (int i = graph->row_ptr[u];
             i < graph->row_ptr[u + 1];
             i++)
        {
            int v = graph->col_idx[i];
            int weight = graph->values[i];

            if (!visited[v] &&
                distance[u] != INT_MAX &&
                distance[u] + weight < distance[v])
            {
                distance[v] = distance[u] + weight;
            }
        }
    }

    printf("Vertex\tDistance\n");
    fprintf(out, "Vertex\tDistance\n");

    for (int i = 0; i < V; i++)
    {
        if (distance[i] == INT_MAX)
        {
            printf("%d\tINF\n", i);
            fprintf(out, "%d\tINF\n", i);
        }
        else
        {
            printf("%d\t%d\n", i, distance[i]);
            fprintf(out, "%d\t%d\n", i, distance[i]);
        }
    }

    free(distance);
    free(visited);
}