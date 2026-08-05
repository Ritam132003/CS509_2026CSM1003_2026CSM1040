#ifndef GRAPH_H
#define GRAPH_H

typedef struct
{
    int V;
    int E;

    int source;        // Source vertex from input file

    int *row_ptr;
    int *col_idx;
    int *values;

} CSRGraph;

CSRGraph* loadGraphCSR(const char *filename, int weighted);

void printCSR(CSRGraph *graph);

void freeGraph(CSRGraph *graph);

#endif