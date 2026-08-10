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

/* Create CSR graph */
CSRGraph *createCSR(int vertices, int edges);

/* Free CSR graph */
void freeCSR(CSRGraph *graph);

/* Read graph from input file */
CSRGraph *readCSR(const char *filename);

/* Print CSR representation */
void printCSR(const CSRGraph *graph);

#endif