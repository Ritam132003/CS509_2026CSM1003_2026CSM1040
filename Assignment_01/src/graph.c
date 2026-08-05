#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

CSRGraph* loadGraphCSR(const char *filename, int weighted)
{
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Cannot open input file.\n");
        return NULL;
    }

    CSRGraph *graph = (CSRGraph *)malloc(sizeof(CSRGraph));

    fscanf(fp, "%d %d", &graph->V, &graph->E);

    graph->row_ptr = (int *)malloc((graph->V + 1) * sizeof(int));

    /* ---------- First Pass : Read Degrees ---------- */

    int *degree = (int *)malloc(graph->V * sizeof(int));

    int totalEntries = 0;

    for (int i = 0; i < graph->V; i++)
    {
        int vertex;

        fscanf(fp, "%d %d", &vertex, &degree[i]);

        totalEntries += degree[i];

        if (weighted)
        {
            for (int j = 0; j < degree[i]; j++)
            {
                int neighbour, weight;
                fscanf(fp, "%d %d", &neighbour, &weight);
            }
        }
        else
        {
            for (int j = 0; j < degree[i]; j++)
            {
                int neighbour;
                fscanf(fp, "%d", &neighbour);
            }
        }
    }

    /* Read source */

    char word[20];
    fscanf(fp, "%s %d", word, &graph->source);

    fclose(fp);

    /* ---------- Allocate CSR Arrays ---------- */

    graph->col_idx = (int *)malloc(totalEntries * sizeof(int));
    graph->values  = (int *)malloc(totalEntries * sizeof(int));

    /* ---------- Second Pass ---------- */

    fp = fopen(filename, "r");

    fscanf(fp, "%d %d", &graph->V, &graph->E);

    int edgeIndex = 0;

    for (int i = 0; i < graph->V; i++)
    {
        int vertex, deg;

        fscanf(fp, "%d %d", &vertex, &deg);

        graph->row_ptr[i] = edgeIndex;

        for (int j = 0; j < deg; j++)
        {
            int neighbour;
            int weight = 1;

            fscanf(fp, "%d", &neighbour);

            if (weighted)
                fscanf(fp, "%d", &weight);

            graph->col_idx[edgeIndex] = neighbour;
            graph->values[edgeIndex] = weight;

            edgeIndex++;
        }
    }

    graph->row_ptr[graph->V] = edgeIndex;

    fscanf(fp, "%s %d", word, &graph->source);

    fclose(fp);

    free(degree);

    return graph;
}

void printCSR(CSRGraph *graph)
{
    printf("\nCSR Representation\n");

    printf("\nrow_ptr:\n");
    for (int i = 0; i <= graph->V; i++)
        printf("%d ", graph->row_ptr[i]);

    printf("\n\ncol_idx:\n");
    for (int i = 0; i < graph->row_ptr[graph->V]; i++)
        printf("%d ", graph->col_idx[i]);

    printf("\n\nvalues:\n");
    for (int i = 0; i < graph->row_ptr[graph->V]; i++)
        printf("%d ", graph->values[i]);

    printf("\n");
}

void freeGraph(CSRGraph *graph)
{
    free(graph->row_ptr);
    free(graph->col_idx);
    free(graph->values);
    free(graph);
}