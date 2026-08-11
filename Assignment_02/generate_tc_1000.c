#include <stdio.h>

int main(void)
{
    FILE *fp;
    int i;

    fp = fopen("Testcases/tc_1000.txt", "w");

    if (fp == NULL)
    {
        printf("Cannot create file.\n");
        return 1;
    }

    /*
     * 1000 vertices.
     *
     * Every group of 3 vertices forms a triangle:
     * (0,1,2), (3,4,5), ...
     *
     * The triangle groups are connected in a chain.
     */

    fprintf(fp, "1000 1332\n");

    for (i = 0; i < 1000; i++)
    {
        if (i % 3 == 0)
        {
            if (i + 2 < 1000)
                fprintf(fp, "%d 2 %d %d\n", i, i + 1, i + 2);
            else
                fprintf(fp, "%d 1 %d\n", i, i - 1);
        }
        else if (i % 3 == 1)
        {
            fprintf(fp, "%d 2 %d %d\n", i, i - 1, i + 1);
        }
        else
        {
            if (i + 1 < 1000)
                fprintf(fp, "%d 3 %d %d %d\n",
                        i, i - 2, i - 1, i + 1);
            else
                fprintf(fp, "%d 2 %d %d\n",
                        i, i - 2, i - 1);
        }
    }

    fclose(fp);

    printf("Created Testcases/tc_1000.txt\n");
    printf("Vertices: 1000\n");
    printf("Edges: 1332\n");

    return 0;
}