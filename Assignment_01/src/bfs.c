#include <stdio.h>
#include <stdlib.h>

#include "bfs.h"

void BFS(CSRGraph *graph, int source, FILE *out)
{
    int V = graph->V;

    int *visited = (int *)calloc(V, sizeof(int));
    int *distance = (int *)malloc(V * sizeof(int));
    int *queue = (int *)malloc(V * sizeof(int));

    int front = 0;
    int rear = 0;

    for (int i = 0; i < V; i++)
        distance[i] = -1;

    visited[source] = 1;
    distance[source] = 0;

    queue[rear++] = source;

    printf("\nAlgorithm : BFS\n");
    printf("Source : %d\n", source);
    printf("Traversal : ");

    fprintf(out, "Algorithm : BFS\n");
    fprintf(out, "Source : %d\n", source);
    fprintf(out, "Traversal : ");

    while (front < rear)
    {
        int u = queue[front++];

        printf("%d ", u);
        fprintf(out, "%d ", u);

        for (int i = graph->row_ptr[u];
             i < graph->row_ptr[u + 1];
             i++)
        {
            int v = graph->col_idx[i];

            if (!visited[v])
            {
                visited[v] = 1;
                distance[v] = distance[u] + 1;

                queue[rear++] = v;
            }
        }
    }

    printf("\n\nDistances\n");
    fprintf(out, "\n\nDistances\n");

    for (int i = 0; i < V; i++)
    {
        if (distance[i] == -1)
        {
            printf("%d : INF\n", i);
            fprintf(out, "%d : INF\n", i);
        }
        else
        {
            printf("%d : %d\n", i, distance[i]);
            fprintf(out, "%d : %d\n", i, distance[i]);
        }
    }

    free(queue);
    free(visited);
    free(distance);
}