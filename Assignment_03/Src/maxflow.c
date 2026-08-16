#include <stdlib.h>
#include <limits.h>
#include "../Include/maxflow.h"

typedef struct {
    int to;
    int rev;
    long long cap;
    long long original_cap;
    int original_u;
    int original_v;
} Edge;

typedef struct {
    Edge *edges;
    int size;
    int capacity;
} EdgeList;

static int add_edge(EdgeList *g, int u, int v, long long cap)
{
    if (g[u].size + 1 > g[u].capacity) {
        int new_cap = g[u].capacity == 0 ? 4 : g[u].capacity * 2;
        Edge *tmp = realloc(g[u].edges, (size_t)new_cap * sizeof(Edge));
        if (tmp == NULL) return 0;
        g[u].edges = tmp;
        g[u].capacity = new_cap;
    }

    if (g[v].size + 1 > g[v].capacity) {
        int new_cap = g[v].capacity == 0 ? 4 : g[v].capacity * 2;
        Edge *tmp = realloc(g[v].edges, (size_t)new_cap * sizeof(Edge));
        if (tmp == NULL) return 0;
        g[v].edges = tmp;
        g[v].capacity = new_cap;
    }

    int ui = g[u].size;
    int vi = g[v].size;

    g[u].edges[ui] = (Edge){v, vi, cap, cap, u, v};
    g[v].edges[vi] = (Edge){u, ui, 0, 0, -1, -1};

    g[u].size++;
    g[v].size++;

    return 1;
}

static int bfs(EdgeList *g, int n, int s, int t, int *level, int *queue)
{
    for (int i = 0; i < n; ++i)
        level[i] = -1;

    int head = 0, tail = 0;
    level[s] = 0;
    queue[tail++] = s;

    while (head < tail) {
        int u = queue[head++];

        for (int i = 0; i < g[u].size; ++i) {
            Edge *e = &g[u].edges[i];

            if (e->cap > 0 && level[e->to] < 0) {
                level[e->to] = level[u] + 1;
                queue[tail++] = e->to;
            }
        }
    }

    return level[t] >= 0;
}

static long long dfs(EdgeList *g, int u, int t, long long pushed,
                     int *level, int *it)
{
    if (u == t)
        return pushed;

    while (it[u] < g[u].size) {
        Edge *e = &g[u].edges[it[u]];

        if (e->cap > 0 && level[e->to] == level[u] + 1) {
            long long amount = dfs(
                g, e->to, t,
                pushed < e->cap ? pushed : e->cap,
                level, it
            );

            if (amount > 0) {
                e->cap -= amount;
                g[e->to].edges[e->rev].cap += amount;
                return amount;
            }
        }

        it[u]++;
    }

    return 0;
}

static void mark_source_side(EdgeList *g, int n, int source, int *side)
{
    int *queue = malloc((size_t)n * sizeof(int));
    if (queue == NULL) return;

    for (int i = 0; i < n; ++i)
        side[i] = 0;

    int head = 0, tail = 0;
    side[source] = 1;
    queue[tail++] = source;

    while (head < tail) {
        int u = queue[head++];

        for (int i = 0; i < g[u].size; ++i) {
            Edge *e = &g[u].edges[i];

            if (e->cap > 0 && !side[e->to]) {
                side[e->to] = 1;
                queue[tail++] = e->to;
            }
        }
    }

    free(queue);
}

MaxflowResult maxflow_mincut(const CSRGraph *graph, int source, int sink)
{
    MaxflowResult result = {0};

    int n = graph->vertices;
    EdgeList *g = calloc((size_t)n, sizeof(EdgeList));

    result.source_side = calloc((size_t)n, sizeof(int));
    result.sink_side = calloc((size_t)n, sizeof(int));

    if (g == NULL || result.source_side == NULL || result.sink_side == NULL)
        goto cleanup;

    /* Residual network construction is part of the timed algorithm call. */
    for (int u = 0; u < n; ++u) {
        for (int i = graph->row_ptr[u]; i < graph->row_ptr[u + 1]; ++i) {
            if (!add_edge(g, u, graph->col_idx[i], graph->values[i]))
                goto cleanup;
        }
    }

    int *level = malloc((size_t)n * sizeof(int));
    int *it = malloc((size_t)n * sizeof(int));
    int *queue = malloc((size_t)n * sizeof(int));

    if (level == NULL || it == NULL || queue == NULL) {
        free(level);
        free(it);
        free(queue);
        goto cleanup;
    }

    while (bfs(g, n, source, sink, level, queue)) {
        for (int i = 0; i < n; ++i)
            it[i] = 0;

        while (1) {
            long long pushed = dfs(g, source, sink, LLONG_MAX,
                                   level, it);
            if (pushed == 0)
                break;
            result.maximum_flow += pushed;
        }
    }

    mark_source_side(g, n, source, result.source_side);

    for (int u = 0; u < n; ++u) {
        if (result.source_side[u])
            result.source_count++;
        else
            result.sink_count++;
    }

    /* Minimum cut is the capacity of original edges crossing the cut. */
    for (int u = 0; u < n; ++u) {
        if (!result.source_side[u])
            continue;

        for (int i = graph->row_ptr[u]; i < graph->row_ptr[u + 1]; ++i) {
            int v = graph->col_idx[i];
            if (!result.source_side[v])
                result.minimum_cut_capacity += graph->values[i];
        }
    }

    for (int i = 0; i < n; ++i)
        result.sink_side[i] = !result.source_side[i];

    free(level);
    free(it);
    free(queue);

cleanup:
    if (g != NULL) {
        for (int i = 0; i < n; ++i)
            free(g[i].edges);
        free(g);
    }

    return result;
}

void free_maxflow_result(MaxflowResult *result)
{
    if (result == NULL) return;
    free(result->source_side);
    free(result->sink_side);
    result->source_side = NULL;
    result->sink_side = NULL;
}
