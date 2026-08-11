#ifndef BTWNES_CENTRALITY_H
#define BTWNES_CENTRALITY_H

#include "csr.h"

void betweennessCentrality(const CSRGraph *graph, double *centrality);

#endif