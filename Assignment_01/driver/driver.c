#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "graph.h"
#include "bfs.h"
#include "dfs.h"
#include "sssp.h"

int main()
{
    int algorithm;
    int sizeChoice;
    int weighted = 0;

    char filename[100];
    char outputFile[100];

    printf("=====================================\n");
    printf("       GRAPH ALGORITHMS MENU\n");
    printf("=====================================\n");
    printf("1. BFS\n");
    printf("2. DFS\n");
    printf("3. SSSP\n");
    printf("Enter your choice: ");
    scanf("%d", &algorithm);

    switch (algorithm)
    {
        case 1:

            printf("\n===== BFS INPUT SIZE =====\n");
            printf("1. 10 Vertices\n");
            printf("2. 100 Vertices\n");
            printf("3. 5000 Vertices\n");
            printf("Enter your choice: ");
            scanf("%d", &sizeChoice);

            switch (sizeChoice)
            {
                case 1:
                    strcpy(filename, "Testcases/bfs_10.txt");
                    strcpy(outputFile, "Output/bfs_10_output.txt");
                    break;

                case 2:
                    strcpy(filename, "Testcases/bfs_100.txt");
                    strcpy(outputFile, "Output/bfs_100_output.txt");
                    break;

                case 3:
                    strcpy(filename, "Testcases/bfs_5000.txt");
                    strcpy(outputFile, "Output/bfs_5000_output.txt");
                    break;

                default:
                    printf("Invalid Input Size!\n");
                    return 1;
            }

            weighted = 0;
            break;

        case 2:

            printf("\n===== DFS INPUT SIZE =====\n");
            printf("1. 10 Vertices\n");
            printf("2. 100 Vertices\n");
            printf("3. 5000 Vertices\n");
            printf("Enter your choice: ");
            scanf("%d", &sizeChoice);

            switch (sizeChoice)
            {
                case 1:
                    strcpy(filename, "Testcases/dfs_10.txt");
                    strcpy(outputFile, "Output/dfs_10_output.txt");
                    break;

                case 2:
                    strcpy(filename, "Testcases/dfs_100.txt");
                    strcpy(outputFile, "Output/dfs_100_output.txt");
                    break;

                case 3:
                    strcpy(filename, "Testcases/dfs_5000.txt");
                    strcpy(outputFile, "Output/dfs_5000_output.txt");
                    break;

                default:
                    printf("Invalid Input Size!\n");
                    return 1;
            }

            weighted = 0;
            break;

        case 3:

            printf("\n===== SSSP INPUT SIZE =====\n");
            printf("1. 10 Vertices\n");
            printf("2. 100 Vertices\n");
            printf("3. 5000 Vertices\n");
            printf("Enter your choice: ");
            scanf("%d", &sizeChoice);

            switch (sizeChoice)
            {
                case 1:
                    strcpy(filename, "Testcases/sssp_10.txt");
                    strcpy(outputFile, "Output/sssp_10_output.txt");
                    break;

                case 2:
                    strcpy(filename, "Testcases/sssp_100.txt");
                    strcpy(outputFile, "Output/sssp_100_output.txt");
                    break;

                case 3:
                    strcpy(filename, "Testcases/sssp_5000.txt");
                    strcpy(outputFile, "Output/sssp_5000_output.txt");
                    break;

                default:
                    printf("Invalid Input Size!\n");
                    return 1;
            }

            weighted = 1;
            break;

        default:
            printf("Invalid Algorithm!\n");
            return 1;
    }

    printf("\nLoading graph from %s\n", filename);

    CSRGraph *graph = loadGraphCSR(filename, weighted);

    if (graph == NULL)
    {
        return 1;
    }

    printf("Graph loaded successfully.\n");

    FILE *out = fopen(outputFile, "w");

    if (out == NULL)
    {
        printf("Cannot create output file.\n");
        freeGraph(graph);
        return 1;
    }

    fprintf(out, "Algorithm Output\n");
    fprintf(out, "=============================\n");
    fprintf(out, "Vertices : %d\n", graph->V);
    fprintf(out, "Edges    : %d\n", graph->E);
    fprintf(out, "Source   : %d\n\n", graph->source);

    clock_t start, end;
    double executionTime;

    switch (algorithm)
    {
        case 1:

            start = clock();

            BFS(graph, graph->source, out);

            end = clock();

            break;

        case 2:

            start = clock();

            DFS(graph, graph->source, out);

            end = clock();

            break;

        case 3:

            start = clock();
            SSSP(graph, graph->source, out);
            end = clock();
            break;
    }

    executionTime = ((double)(end - start) * 1000.0) / CLOCKS_PER_SEC;

    printf("\nExecution Time : %.3f ms\n", executionTime);

    fprintf(out, "\nExecution Time : %.3f ms\n", executionTime);

    fclose(out);

    freeGraph(graph);

    printf("\nOutput saved to : %s\n", outputFile);

    return 0;
}