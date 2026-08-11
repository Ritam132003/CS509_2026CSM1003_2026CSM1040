#include <stdio.h>
#include <stdlib.h>

#include "../Include/csr.h"

CSRGraph *createCSR(int vertices, int edges)
{
    CSRGraph *graph = malloc(sizeof(CSRGraph));

    if (graph == NULL)
        return NULL;

    graph->vertices = vertices;
    graph->edges = edges;

  
    graph->row_ptr = malloc((vertices + 1) * sizeof(int));
    graph->col_idx = malloc((2 * edges) * sizeof(int));
    graph->values = malloc((2 * edges) * sizeof(int));

    if (graph->row_ptr == NULL ||
        graph->col_idx == NULL ||
        graph->values == NULL)
    {
        freeCSR(graph);
        return NULL;
    }

    return graph;
}

void freeCSR(CSRGraph *graph)
{
    if (graph == NULL)
        return;

    free(graph->row_ptr);
    free(graph->col_idx);
    free(graph->values);
    free(graph);
}

CSRGraph *readCSR(const char *filename)
{
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Error: Cannot open file %s\n", filename);
        return NULL;
    }

    int vertices, edges;

    if (fscanf(fp, "%d %d", &vertices, &edges) != 2)
    {
        fclose(fp);
        return NULL;
    }

    CSRGraph *graph = createCSR(vertices, edges);

    if (graph == NULL)
    {
        fclose(fp);
        return NULL;
    }

    int *degree = calloc(vertices, sizeof(int));

    if (degree == NULL)
    {
        freeCSR(graph);
        fclose(fp);
        return NULL;
    }

    for (int i = 0; i < vertices; i++)
    {
        int vertex;
        int count;

        if (fscanf(fp, "%d %d", &vertex, &count) != 2)
        {
            free(degree);
            freeCSR(graph);
            fclose(fp);
            return NULL;
        }

        degree[vertex] = count;

        for (int j = 0; j < count; j++)
        {
            int neighbor;

            if (fscanf(fp, "%d", &neighbor) != 1)
            {
                free(degree);
                freeCSR(graph);
                fclose(fp);
                return NULL;
            }
        }
    }

    graph->row_ptr[0] = 0;

    for (int i = 0; i < vertices; i++)
    {
        graph->row_ptr[i + 1] =
            graph->row_ptr[i] + degree[i];
    }

    rewind(fp);

    fscanf(fp, "%d %d", &vertices, &edges);

    for (int i = 0; i < vertices; i++)
    {
        int vertex;
        int count;

        fscanf(fp, "%d %d", &vertex, &count);

        int start = graph->row_ptr[vertex];

        for (int j = 0; j < count; j++)
        {
            int neighbor;

            fscanf(fp, "%d", &neighbor);

            graph->col_idx[start + j] = neighbor;

            /*
             * The graph is unweighted, so every value is 1.
             */
            graph->values[start + j] = 1;
        }
    }

    free(degree);
    fclose(fp);

    return graph;
}

void printCSR(const CSRGraph *graph)
{
    printf("\nCSR Representation\n");
    printf("------------------\n");

    printf("row_ptr: ");

    for (int i = 0; i <= graph->vertices; i++)
        printf("%d ", graph->row_ptr[i]);

    printf("\n");

    printf("col_idx: ");

    int totalEntries = graph->row_ptr[graph->vertices];

    for (int i = 0; i < totalEntries; i++)
        printf("%d ", graph->col_idx[i]);

    printf("\n");
}