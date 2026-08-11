#ifndef CSR_H
#define CSR_H

typedef struct
{
    int vertices;
    int edges;

    int *row_ptr;
    int *col_idx;
    int *values;

} CSRGraph;

CSRGraph *createCSR(int vertices, int edges);

void freeCSR(CSRGraph *graph);

CSRGraph *readCSR(const char *filename);

void printCSR(const CSRGraph *graph);

#endif