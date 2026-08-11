#include <stdlib.h>

#include "../Include/connected_comp.h"

int connectedComponents(const CSRGraph *graph, int *component)
{
    int V = graph->vertices;

    for (int i = 0; i < V; i++)
        component[i] = -1;

    int componentCount = 0;

    int *queue = malloc(V * sizeof(int));

    if (queue == NULL)
        return -1;

    for (int start = 0; start < V; start++)
    {
        if (component[start] != -1)
            continue;

        int front = 0;
        int rear = 0;

        queue[rear++] = start;
        component[start] = componentCount;

        while (front < rear)
        {
            int u = queue[front++];

            for (int i = graph->row_ptr[u];
                 i < graph->row_ptr[u + 1];
                 i++)
            {
                int v = graph->col_idx[i];

                if (component[v] == -1)
                {
                    component[v] = componentCount;
                    queue[rear++] = v;
                }
            }
        }

        componentCount++;
    }

    free(queue);

    return componentCount;
}