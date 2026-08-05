# CS509 Assignment 1 - Buddy Project

## Algorithms

- Breadth First Search (BFS)
- Depth First Search (DFS)
- Single Source Shortest Path (SSSP)
- CSR (Compressed Sparse Row) Graph Representation

---

## Graph Representation

All graph input files are stored as **Adjacency Lists**.

Before executing any graph algorithm, the adjacency list is converted into **Compressed Sparse Row (CSR)** representation.

CSR consists of three arrays:

- **row_ptr** – Stores the starting index of each vertex's adjacency list.
- **col_idx** – Stores adjacent vertices.
- **values** – Stores edge weights (used only for weighted graphs).

The CSR representation improves memory efficiency for sparse graphs and enables faster graph traversal.

---

## Algorithms

### Breadth First Search (BFS)

Breadth First Search explores graph vertices level-by-level beginning from the source vertex.

The implementation uses a queue and computes:

- BFS Traversal
- Minimum edge distance from the source vertex
---

### Depth First Search (DFS)

Depth First Search explores one branch completely before backtracking.

The implementation uses recursive DFS on the CSR graph.

The program outputs:

- DFS Traversal

---

### Single Source Shortest Path (SSSP)

The Single Source Shortest Path problem is solved using **Dijkstra's Algorithm** because all edge weights are positive.

The implementation computes the shortest distance from the source vertex to every other reachable vertex.

# Timing

Only the **algorithm execution time** is measured.

The following operations are **NOT** included in the reported execution time:

- File reading
- Input parsing
- Adjacency List to CSR conversion
- Output generation
- Output file writing

Timing starts immediately before calling the selected algorithm and stops immediately after the algorithm completes.

---

# Graph Test Cases
 
| Test File | Vertices | Graph Type |
|-----------|---------:|------------|
| bfs_10.txt    | 10   | Unweighted |
| bfs_100.txt   | 100  | Unweighted |
| bfs_5000.txt  | 5000 | Unweighted |
| dfs_10.txt    | 10   | Unweighted |
| dfs_100.txt   | 100  | Unweighted |
| dfs_5000.txt  | 5000 | Unweighted |
| sssp_10.txt   | 10   | Weighted   |
| sssp_100.txt  | 100  | Weighted   |
| sssp_5000.txt | 5000 | Weighted   |

---

# Performance Results

| Algorithm | Test File | Time | Status |
|-----------|-----------|-----:|:------:|
| BFS | bfs_10.txt     |  0.753ms| Pass |
| BFS | bfs_100.txt    | 35.000ms| Pass |
| BFS | bfs_5000.txt   |721.000ms| Pass |
| DFS | dfs_10.txt     | 0853 ms | Pass |
| DFS | dfs_100.txt    |22.000 ms| Pass |
| DFS | dfs_5000.txt   |450.000ms| Pass |
| SSSP | sssp_10.txt   |0.880 ms | Pass |
| SSSP | sssp_100.txt  |21.000 ms| Pass |
| SSSP | sssp_5000.txt |502.000ms| Pass |

---

# Output

The generated outputs are stored in the **Output/** directory.

Example:

```
Output/
├── bfs_10_output.txt
├── bfs_100_output.txt
├── bfs_5000_output.txt
├── dfs_10_output.txt
├── dfs_100_output.txt
├── dfs_5000_output.txt
├── sssp_10_output.txt
├── sssp_100_output.txt
└── sssp_5000_output.txt
```

---

# Observations

- CSR efficiently stores sparse graphs using compact arrays.
- CSR conversion is performed once before algorithm execution.
- BFS visits vertices level-by-level and computes minimum edge distances.
- DFS explores vertices recursively by traversing one path completely before backtracking.
- Dijkstra's algorithm correctly computes shortest paths for graphs with positive edge weights.
- Algorithm execution time increases with graph size.
- CSR representation reduces memory usage compared to storing large adjacency matrices.

---

# Conclusion

The project successfully implements BFS, DFS, and SSSP using the CSR graph representation. The driver program reads graph input files, converts them into CSR format, executes the selected algorithm, and reports only the algorithm execution time. Using CSR provides an efficient representation for sparse graphs while supporting scalable graph traversal and shortest path computation.