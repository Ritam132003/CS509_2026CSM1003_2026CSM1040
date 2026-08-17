#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#endif

typedef struct {
#ifdef _WIN32
    LARGE_INTEGER value;
#else
    struct timespec value;
#endif
} HighResTimer;

static void timer_start(HighResTimer *timer)
{
#ifdef _WIN32
    QueryPerformanceCounter(&timer->value);
#else
    clock_gettime(CLOCK_MONOTONIC, &timer->value);
#endif
}

static double timer_elapsed_ms(const HighResTimer *start,
                               const HighResTimer *end)
{
#ifdef _WIN32
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    return ((double)(end->value.QuadPart - start->value.QuadPart)
            / (double)frequency.QuadPart) * 1000.0;
#else
    time_t sec = end->value.tv_sec - start->value.tv_sec;
    long nsec = end->value.tv_nsec - start->value.tv_nsec;
    return (double)sec * 1000.0 + (double)nsec / 1000000.0;
#endif
}

#include "../Include/csr.h"
#include "../Include/gradient_descent.h"
#include "../Include/maxflow.h"

static int ensure_output_directory(void)
{
#ifdef _WIN32
    return system("if not exist Output mkdir Output") == 0;
#else
    return system("mkdir -p Output") == 0;
#endif
}

static int read_graph_file(const char *filename, int *V, int *E,
                           int ***adjacency, int **degree,
                           int *source, int *sink, int read_flow_terminals)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) return 0;

    if (fscanf(file, "%d %d", V, E) != 2 || *V <= 0 || *E < 0) {
        fclose(file);
        return 0;
    }

    *adjacency = calloc((size_t)*V, sizeof(int *));
    *degree = calloc((size_t)*V, sizeof(int));

    if (*adjacency == NULL || *degree == NULL) {
        free(*adjacency);
        free(*degree);
        fclose(file);
        return 0;
    }

    for (int i = 0; i < *V; ++i) {
        int u, d;

        if (fscanf(file, "%d %d", &u, &d) != 2 ||
            u < 0 || u >= *V || d < 0) {
            fclose(file);
            return 0;
        }

        (*degree)[u] = d;

        if (d > 0) {
            (*adjacency)[u] = malloc((size_t)(2 * d) * sizeof(int));
            if ((*adjacency)[u] == NULL) {
                fclose(file);
                return 0;
            }

            for (int j = 0; j < d; ++j) {
                int v, w;

                if (fscanf(file, "%d %d", &v, &w) != 2 ||
                    v < 0 || v >= *V || w < 0) {
                    fclose(file);
                    return 0;
                }

                (*adjacency)[u][2 * j] = v;
                (*adjacency)[u][2 * j + 1] = w;
            }
        }
    }

    if (read_flow_terminals) {
        char word[32];

        if (fscanf(file, "%31s %d", word, source) != 2 ||
            strcmp(word, "SOURCE") != 0) {
            fclose(file);
            return 0;
        }

        if (fscanf(file, "%31s %d", word, sink) != 2 ||
            strcmp(word, "SINK") != 0 ||
            *source < 0 || *source >= *V ||
            *sink < 0 || *sink >= *V ||
            *source == *sink) {
            fclose(file);
            return 0;
        }
    }

    fclose(file);
    return 1;
}

static void free_adjacency(int V, int **adjacency, int *degree)
{
    if (adjacency != NULL) {
        for (int i = 0; i < V; ++i)
            free(adjacency[i]);
    }
    free(adjacency);
    free(degree);
}

static int read_gd_file(const char *filename, int *degree, double **coeff,
                        double *initial_x, double *rate,
                        double *tolerance, int *max_iterations)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) return 0;

    char key[64];

    if (fscanf(file, "%63s %d", key, degree) != 2 ||
        strcmp(key, "DEGREE") != 0 ||
        *degree < 1 || *degree > 100) {
        fclose(file);
        return 0;
    }

    *coeff = malloc((size_t)(*degree + 1) * sizeof(double));
    if (*coeff == NULL) {
        fclose(file);
        return 0;
    }

    if (fscanf(file, "%63s", key) != 1 ||
        strcmp(key, "COEFFICIENTS") != 0) {
        free(*coeff);
        fclose(file);
        return 0;
    }

    for (int i = 0; i <= *degree; ++i) {
        if (fscanf(file, "%lf", &(*coeff)[i]) != 1) {
            free(*coeff);
            fclose(file);
            return 0;
        }
    }

    if (fscanf(file, "%63s %lf", key, initial_x) != 2 ||
        strcmp(key, "INITIAL_X") != 0) goto fail;

    if (fscanf(file, "%63s %lf", key, rate) != 2 ||
        strcmp(key, "LEARNING_RATE") != 0 || *rate <= 0.0) goto fail;

    if (fscanf(file, "%63s %lf", key, tolerance) != 2 ||
        strcmp(key, "TOLERANCE") != 0 || *tolerance <= 0.0) goto fail;

    if (fscanf(file, "%63s %d", key, max_iterations) != 2 ||
        strcmp(key, "MAX_ITERATIONS") != 0 || *max_iterations <= 0) goto fail;

    fclose(file);
    return 1;

fail:
    free(*coeff);
    fclose(file);
    return 0;
}

static void make_output_name(const char *algorithm, const char *input,
                             char *out, size_t size)
{
    const char *base = strrchr(input, '/');
#ifdef _WIN32
    const char *b2 = strrchr(input, '\\');
    if (b2 && (!base || b2 > base)) base = b2;
#endif
    base = base ? base + 1 : input;

    char name[256];
    snprintf(name, sizeof(name), "%s", base);

    char *dot = strrchr(name, '.');
    if (dot) *dot = '\0';

    snprintf(out, size, "Output/%s_%s.txt", algorithm, name);
}

static int run_gd(const char *filename)
{
    int degree, max_iterations;
    double *coeff = NULL;
    double initial_x, rate, tolerance;

    if (!read_gd_file(filename, &degree, &coeff, &initial_x,
                      &rate, &tolerance, &max_iterations))
        return 1;

    clock_t start = clock();
    GDResult result = gradient_descent(
        degree, coeff, initial_x, rate, tolerance, max_iterations
    );
    clock_t end = clock();

    double time_ms = ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0;

    ensure_output_directory();

    char output_file[512];
    make_output_name("gradient_descent", filename,
                     output_file, sizeof(output_file));

    FILE *out = fopen(output_file, "w");
    if (out == NULL) {
        free(coeff);
        return 1;
    }

    fprintf(out, "Degree: %d\n", result.degree);
    fprintf(out, "Final x: %.10f\n", result.final_x);
    fprintf(out, "Final f(x): %.10f\n", result.final_fx);
    fprintf(out, "Iterations: %d\n", result.iterations);
    fprintf(out, "Converged: %s\n", result.converged ? "true" : "false");
    fprintf(out, "Execution time: %.6f ms\n", time_ms);
    fclose(out);

    FILE *in = fopen(output_file, "r");
    if (in != NULL) {
        char line[512];
        while (fgets(line, sizeof(line), in))
            fputs(line, stdout);
        fclose(in);
    }

    free(coeff);
    return 0;
}

static int run_maxflow(const char *filename)
{
    int V, E, source, sink;
    int **adjacency = NULL;
    int *degree = NULL;

    if (!read_graph_file(filename, &V, &E, &adjacency, &degree,
                         &source, &sink, 1))
        return 1;

    CSRGraph *graph = create_csr(V, E, adjacency, degree);
    free_adjacency(V, adjacency, degree);

    if (graph == NULL)
        return 1;

    clock_t start = clock();
    MaxflowResult result = maxflow_mincut(graph, source, sink);
    clock_t end = clock();

    double time_ms = ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0;

    ensure_output_directory();

    char output_file[512];
    make_output_name("maxflow", filename,
                     output_file, sizeof(output_file));

    FILE *out = fopen(output_file, "w");
    if (out == NULL) {
        free_maxflow_result(&result);
        free_csr(graph);
        return 1;
    }

    fprintf(out, "Source: %d\n", source);
    fprintf(out, "Sink: %d\n", sink);
    fprintf(out, "Maximum flow: %lld\n", result.maximum_flow);
    fprintf(out, "Minimum cut capacity: %lld\n",
            result.minimum_cut_capacity);

    fprintf(out, "Source side:");
    for (int i = 0; i < V; ++i)
        if (result.source_side[i])
            fprintf(out, " %d", i);
    fprintf(out, "\n");

    fprintf(out, "Sink side:");
    for (int i = 0; i < V; ++i)
        if (result.sink_side[i])
            fprintf(out, " %d", i);
    fprintf(out, "\n");

    fprintf(out, "Cut edges:\n");
    for (int u = 0; u < V; ++u) {
        if (!result.source_side[u]) continue;

        for (int i = graph->row_ptr[u]; i < graph->row_ptr[u + 1]; ++i) {
            int v = graph->col_idx[i];
            int capacity = graph->values[i];

            if (!result.source_side[v])
                fprintf(out, "%d %d %d\n", u, v, capacity);
        }
    }

    fprintf(out, "Execution time: %.6f ms\n", time_ms);
    fclose(out);

    FILE *in = fopen(output_file, "r");
    if (in != NULL) {
        char line[512];
        while (fgets(line, sizeof(line), in))
            fputs(line, stdout);
        fclose(in);
    }

    free_maxflow_result(&result);
    free_csr(graph);
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
        return 1;

    if (strcmp(argv[1], "gd") == 0)
        return run_gd(argv[2]);

    if (strcmp(argv[1], "maxflow") == 0)
        return run_maxflow(argv[2]);

    return 1;
}
