#ifndef GRAPH_H
#define GRAPH_H

typedef struct
{
    int V;        
    int E;       

    int *row_ptr;   // CSR row pointer
    int *col_idx;   // CSR column indices
    int *values;    // Edge weights (used by SSSP)
} CSRGraph;

CSRGraph* loadGraphCSR(const char *filename, int weighted);

// Print CSR arrays (for debugging)
void printCSR(CSRGraph *graph);

// Free allocated memory
void freeGraph(CSRGraph *graph);

#endif