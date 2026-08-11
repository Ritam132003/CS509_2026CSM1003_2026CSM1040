#include <stdio.h>

#define V 100

int main()
{
    FILE *fp = fopen("Testcases/tc_100.txt", "w");

    if (fp == NULL)
    {
        printf("Error: Could not create file.\n");
        return 1;
    }

    /*
     * Graph:
     * 0-1-2-3-...-99
     *
     * Additional edges create triangles:
     * (0,1,2), (3,4,5), (6,7,8), ...
     */

    int adj[V][V] = {0};
    int edges = 0;

    /* Create chain */
    for (int i = 0; i < V - 1; i++)
    {
        adj[i][i + 1] = 1;
        adj[i + 1][i] = 1;
        edges++;
    }

    /* Add triangle edges */
    for (int i = 0; i < V - 2; i += 3)
    {
        if (!adj[i][i + 2])
        {
            adj[i][i + 2] = 1;
            adj[i + 2][i] = 1;
            edges++;
        }
    }

    fprintf(fp, "%d %d\n", V, edges);

    /* Write adjacency-list format */
    for (int i = 0; i < V; i++)
    {
        int degree = 0;

        for (int j = 0; j < V; j++)
        {
            if (adj[i][j])
                degree++;
        }

        fprintf(fp, "%d %d", i, degree);

        for (int j = 0; j < V; j++)
        {
            if (adj[i][j])
                fprintf(fp, " %d", j);
        }

        fprintf(fp, "\n");
    }

    fclose(fp);

    printf("Created Testcases/tc_100.txt\n");
    printf("Vertices: %d\n", V);
    printf("Edges: %d\n", edges);

    return 0;
}