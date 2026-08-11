#include <stdio.h>

int main(void)
{
    FILE *fp = fopen("Testcases/bc_1000.txt", "w");

    if (fp == NULL)
    {
        printf("Error creating file.\n");
        return 1;
    }

    int V = 1000;

    /*
     * Create a simple connected chain:
     * 0--1--2--3--...--999
     *
     * Number of undirected edges = 999
     */

    fprintf(fp, "%d %d\n", V, V - 1);

    for (int i = 0; i < V; i++)
    {
        if (i == 0)
        {
            fprintf(fp, "0 1 1\n");
        }
        else if (i == V - 1)
        {
            fprintf(fp, "%d 1 %d\n", i, i - 1);
        }
        else
        {
            fprintf(fp, "%d 2 %d %d\n",
                    i, i - 1, i + 1);
        }
    }

    fclose(fp);

    printf("Created Testcases/bc_1000.txt\n");
    printf("Vertices: %d\n", V);
    printf("Edges: %d\n", V - 1);

    return 0;
}