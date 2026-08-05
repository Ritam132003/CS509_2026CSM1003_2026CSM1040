#include <stdio.h>
#include <stdlib.h>
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
    graph->col_idx = (int *)malloc(graph->E * sizeof(int));
    graph->values  = (int *)malloc(graph->E * sizeof(int));

    int edgeIndex = 0;

    for(int i = 0; i < graph->V; i++)
    {
        int vertex;
        int degree;

        fscanf(fp,"%d %d",&vertex,&degree);

        graph->row_ptr[i] = edgeIndex;

        for(int j = 0; j < degree; j++)
        {
            int neighbour;
            int weight = 1;

            fscanf(fp,"%d",&neighbour);

            graph->col_idx[edgeIndex] = neighbour;

            if(weighted)
            {
                fscanf(fp,"%d",&weight);
            }

            graph->values[edgeIndex] = weight;

            edgeIndex++;
        }
    }

    graph->row_ptr[graph->V] = edgeIndex;

    fclose(fp);

    return graph;
}

void printCSR(CSRGraph *graph)
{
    int i;

    printf("\nCSR Representation\n");

    printf("\nrow_ptr:\n");

    for(i=0;i<=graph->V;i++)
        printf("%d ",graph->row_ptr[i]);

    printf("\n\ncol_idx:\n");

    for(i=0;i<graph->row_ptr[graph->V];i++)
        printf("%d ",graph->col_idx[i]);

    printf("\n\nvalues:\n");

    for(i=0;i<graph->row_ptr[graph->V];i++)
        printf("%d ",graph->values[i]);

    printf("\n");
}

void freeGraph(CSRGraph *graph)
{
    free(graph->row_ptr);
    free(graph->col_idx);
    free(graph->values);
    free(graph);
}