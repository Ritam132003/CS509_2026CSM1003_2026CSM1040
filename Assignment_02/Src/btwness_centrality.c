#include <stdio.h>
#include <stdlib.h>
#include "../Include/btwness_centrality.h"

void betweennessCentrality(const CSRGraph *graph, double *centrality)
{
    int V = graph->vertices;

    /* Initialize centrality */
    for (int i = 0; i < V; i++)
    {
        centrality[i] = 0.0;
    }

    int *queue = malloc(V * sizeof(int));
    int *stack = malloc(V * sizeof(int));
    int *distance = malloc(V * sizeof(int));
    int *sigma = malloc(V * sizeof(int));
    double *delta = malloc(V * sizeof(double));
    int **predecessors = malloc(V * sizeof(int *));

    if (queue == NULL || stack == NULL ||
        distance == NULL || sigma == NULL ||
        delta == NULL || predecessors == NULL)
    {
        free(queue);
        free(stack);
        free(distance);
        free(sigma);
        free(delta);
        free(predecessors);
        return;
    }

    for (int s = 0; s < V; s++)
    {
        for (int i = 0; i < V; i++)
        {
            distance[i] = -1;
            sigma[i] = 0;
            delta[i] = 0.0;

            predecessors[i] =
                malloc(V * sizeof(int));

            if (predecessors[i] == NULL)
            {
                for (int j = 0; j < i; j++)
                {
                    free(predecessors[j]);
                }

                free(queue);
                free(stack);
                free(distance);
                free(sigma);
                free(delta);
                free(predecessors);

                return;
            }

          
            for (int j = 0; j < V; j++)
            {
                predecessors[i][j] = -1;
            }
        }

        int stackSize = 0;
        int front = 0;
        int rear = 0;

        queue[rear++] = s;

        distance[s] = 0;
        sigma[s] = 1;

        /*
         * BFS
         */
        while (front < rear)
        {
            int v = queue[front++];

            stack[stackSize++] = v;

            for (int i = graph->row_ptr[v];
                 i < graph->row_ptr[v + 1];
                 i++)
            {
                int w = graph->col_idx[i];

                /*
                 * First time discovering w.
                 */
                if (distance[w] < 0)
                {
                    distance[w] = distance[v] + 1;
                    queue[rear++] = w;
                }

                if (distance[w] == distance[v] + 1)
                {
                    sigma[w] += sigma[v];

                    int count = 0;

                    while (count < V &&
                           predecessors[w][count] != -1)
                    {
                        count++;
                    }

                    if (count < V)
                    {
                        predecessors[w][count] = v;
                    }
                }
            }
        }

        while (stackSize > 0)
        {
            int w = stack[--stackSize];

            for (int p = 0; p < V; p++)
            {
                if (predecessors[w][p] == -1)
                {
                    break;
                }

                int v = predecessors[w][p];

                if (sigma[w] != 0)
                {
                    delta[v] +=
                        ((double)sigma[v] /
                         (double)sigma[w]) *
                        (1.0 + delta[w]);
                }
            }

            if (w != s)
            {
                centrality[w] += delta[w];
            }
        }

        for (int i = 0; i < V; i++)
        {
            free(predecessors[i]);
        }
    }

    for (int i = 0; i < V; i++)
    {
        centrality[i] /= 2.0;
    }

    free(queue);
    free(stack);
    free(distance);
    free(sigma);
    free(delta);
    free(predecessors);
}