#include <stdio.h>

#include "../Include/triangle_count.h"

static int hasEdge(
    const CSRGraph *graph,
    int source,
    int destination)
{
    for (int i = graph->row_ptr[source];
         i < graph->row_ptr[source + 1];
         i++)
    {
        if (graph->col_idx[i] == destination)
            return 1;
    }

    return 0;
}

long long countTriangles(const CSRGraph *graph)
{
    long long triangles = 0;

    for (int u = 0; u < graph->vertices; u++)
    {
        for (int i = graph->row_ptr[u];
             i < graph->row_ptr[u + 1];
             i++)
        {
            int v = graph->col_idx[i];

            if (v <= u)
                continue;

            for (int j = graph->row_ptr[v];
                 j < graph->row_ptr[v + 1];
                 j++)
            {
                int w = graph->col_idx[j];

                if (w <= v)
                    continue;

                if (hasEdge(graph, u, w))
                    triangles++;
            }
        }
    }

    return triangles;
}

void printTriangles(const CSRGraph *graph)
{
    for (int u = 0; u < graph->vertices; u++)
    {
        for (int i = graph->row_ptr[u];
             i < graph->row_ptr[u + 1];
             i++)
        {
            int v = graph->col_idx[i];

            if (v <= u)
                continue;

            for (int j = graph->row_ptr[v];
                 j < graph->row_ptr[v + 1];
                 j++)
            {
                int w = graph->col_idx[j];

                if (w <= v)
                    continue;

                if (hasEdge(graph, u, w))
                {
                    printf("(%d, %d, %d)\n",
                           u, v, w);
                }
            }
        }
    }
}