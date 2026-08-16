#ifndef MAXFLOW_H
#define MAXFLOW_H

#include "csr.h"

typedef struct {
    long long maximum_flow;
    long long minimum_cut_capacity;
    int *source_side;
    int *sink_side;
    int source_count;
    int sink_count;
} MaxflowResult;

MaxflowResult maxflow_mincut(const CSRGraph *graph, int source, int sink);
void free_maxflow_result(MaxflowResult *result);

#endif
