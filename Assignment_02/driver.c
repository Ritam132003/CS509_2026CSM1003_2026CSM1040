#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Include/csr.h"
#include "Include/triangle_count.h"
#include "Include/connected_comp.h"
#include "Include/btwness_centrality.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];

    /* =====================================
       READ GRAPH
       ===================================== */

    CSRGraph *graph = readCSR(filename);

    if (graph == NULL)
    {
        printf("Error: Could not read graph.\n");
        return 1;
    }

    printf("\nInput File : %s\n", filename);
    printf("Vertices   : %d\n", graph->vertices);
    printf("Edges      : %d\n", graph->edges);

    printf("\n## CSR Representation\n\n");
    printCSR(graph);

    /* =====================================
       TRIANGLE COUNTING
       ===================================== */

    clock_t triangleStart = clock();

    long long triangles = countTriangles(graph);

    clock_t triangleEnd = clock();

    double triangleTime =
        ((double)(triangleEnd - triangleStart) * 1000.0)
        / CLOCKS_PER_SEC;

    printf("\nAlgorithm: Triangle Counting\n\n");

    printf("Total triangles: %lld\n", triangles);

    if (graph->vertices <= 10)
    {
        printf("Triangles found:\n");
        printTriangles(graph);
    }

    printf("Execution time: %.6f ms\n", triangleTime);

    /* =====================================
       CONNECTED COMPONENTS
       ===================================== */

    int *component =
        (int *)malloc(graph->vertices * sizeof(int));

    if (component == NULL)
    {
        printf("Error: Memory allocation failed.\n");
        freeCSR(graph);
        return 1;
    }

    clock_t ccStart = clock();

    int componentCount =
        connectedComponents(graph, component);

    clock_t ccEnd = clock();

    double ccTime =
        ((double)(ccEnd - ccStart) * 1000.0)
        / CLOCKS_PER_SEC;

    printf("\nAlgorithm: Connected Components\n\n");

    printf("Number of components: %d\n\n",
           componentCount);

    printf("Vertex Component\n");

    for (int i = 0; i < graph->vertices; i++)
    {
        printf("%d %d\n", i, component[i]);
    }

    printf("Execution time: %.6f ms\n", ccTime);

    free(component);

    /* =====================================
       BETWEENNESS CENTRALITY
       ===================================== */

    double *centrality =
        (double *)malloc(graph->vertices * sizeof(double));

    if (centrality == NULL)
    {
        printf("Error: Memory allocation failed.\n");
        freeCSR(graph);
        return 1;
    }

    clock_t bcStart = clock();

    betweennessCentrality(graph, centrality);

    clock_t bcEnd = clock();

    double bcTime =
        ((double)(bcEnd - bcStart) * 1000.0)
        / CLOCKS_PER_SEC;

    printf("\nAlgorithm: Betweenness Centrality\n\n");

    printf("Vertex Centrality\n");

    for (int i = 0; i < graph->vertices; i++)
    {
        printf("%d %.2f\n", i, centrality[i]);
    }

    printf("Execution time: %.6f ms\n", bcTime);

    free(centrality);

    /* =====================================
       FREE MEMORY
       ===================================== */

    freeCSR(graph);

    return 0;
}