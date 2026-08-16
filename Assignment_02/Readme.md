# CS509 Assignment 02 - Buddy Task

## Graph Analytics

This assignment implements graph analytics algorithms using a **Compressed Sparse Row (CSR)** representation for unweighted, undirected graphs.

The following algorithms are implemented:

* Triangle Counting
* Connected Components
* Betweenness Centrality

---

## 1. Student Details

**Name:** Ritam Chowdhury (2026CSM1003)
          Yash Saklecha (2026CSM1040)
**Course:** CS509
**Assignment:** Assignment 02 - Buddy Task

---

## 2. Programming Environment

* **Programming Language:** C
* **Compiler:** GCC / MinGW
* **IDE:** Visual Studio Code
* **Operating System:** Windows

---

## 3. Assignment Objective

The objective of this assignment is to implement graph analytics algorithms using a CSR graph representation.

The implemented algorithms are:

1. **Triangle Counting**
2. **Connected Components**
3. **Betweenness Centrality**

The input graphs are represented using unweighted, undirected adjacency lists. These adjacency lists are converted into CSR format before running the algorithms.

---

## 4. Directory Structure

```text
Assignment_02/
│
├── Include/
│   ├── btwness_centrality.h
│   ├── connected_comp.h
│   ├── csr.h
│   └── triangle_count.h
│
├── Output/
│
├── Src/
│   ├── btwness_centrality.c
│   ├── connected_comp.c
│   ├── csr.c
│   └── triangle_count.c
│
├── Testcases/
│   ├── bc_5.txt
│   ├── bc_100.txt
│   ├── bc_1000.txt
│   ├── bc_5000.txt
│   ├── cc_8.txt
│   ├── cc_100.txt
│   ├── cc_1000.txt
│   ├── tc_6.txt
│   ├── tc_10.txt
│   ├── tc_100.txt
│   ├── tc_1000.txt
│   └── test_small.txt
│
├── driver.c
├── buddy.exe
│
├── generate_tc_100.c
├── generate_tc_100.exe
├── generate_tc_1000.c
├── generate_tc_1000.exe
│
└── generate_bc_1000.c
```

---

# 5. CSR Graph Representation

The input graph is first read as an adjacency list and then converted into **Compressed Sparse Row (CSR)** format.

CSR uses three arrays:

### `row_ptr`

Stores the starting position of the neighbors of every vertex in `col_idx`.

### `col_idx`

Stores the neighboring vertex IDs.

### `values`

Stores the value associated with every edge. Since the graph is unweighted, every value is `1`.

For example:

```text
row_ptr: 0 2 5 8 12 14 16
col_idx: 1 2 0 2 3 0 1 3 1 2 4 5 3 5 3 4
values:  1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1
```

CSR allows the algorithms to access the neighbors of each vertex efficiently.

---

# 6. Input File Format

The graph input follows the unweighted undirected adjacency-list format.

The first line contains:

```text
V E
```

where:

* `V` = number of vertices
* `E` = number of undirected edges

Each following line contains:

```text
vertex degree neighbor1 neighbor2 ...
```

For example:

```text
5 4
0 1 1
1 2 0 2
2 2 1 3
3 2 2 4
4 1 3
```

Every undirected edge appears in the adjacency list of both endpoints.

---

# 7. Triangle Counting

## Description

Triangle Counting determines the total number of triangles present in the graph.

A triangle exists when three vertices are mutually connected.

For example:

```text
0 -- 1
 \  /
   2
```

represents the triangle:

```text
(0, 1, 2)
```

The implementation uses the CSR representation to efficiently check whether neighboring vertices are connected.

For small graphs, the program also prints the triangles found.

## Example

For `tc_6.txt`:

```text
Vertices : 6
Edges    : 8
```

The program produced:

```text
Algorithm: Triangle Counting

Total triangles: 3

Triangles found:
(0, 1, 2)
(1, 2, 3)
(3, 4, 5)
```

Therefore:

```text
Total triangles = 3
```

---

# 8. Connected Components

## Description

Connected Components determines the number of separate connected components in the graph.

A component is a set of vertices where every vertex is reachable from another vertex in the same component.

The implementation uses graph traversal over the CSR representation.

Component IDs are assigned in the order in which the components are first discovered, starting from `0`.

## Example

For `cc_8.txt`:

```text
Vertices : 8
Edges    : 4
```

The output was:

```text
Number of components: 4

Vertex Component
0      0
1      0
2      0
3      0
4      1
5      1
6      2
7      3
```

Therefore:

```text
Number of components = 4
```

---

# 9. Betweenness Centrality

## Description

Betweenness Centrality measures how frequently a vertex occurs on shortest paths between other pairs of vertices.

The implementation uses a shortest-path based approach for every source vertex.

The algorithm calculates the raw, unnormalized betweenness centrality value for every vertex.

For an undirected graph, the values are divided by `2` because each shortest path is considered from both directions.

## Example

For `bc_5.txt`:

```text
Vertices : 5
Edges    : 4
```

The output was:

```text
Algorithm: Betweenness Centrality

Vertex Centrality
0 0.00
1 3.00
2 4.00
3 3.00
4 0.00
```

Therefore, the centrality values are:

| Vertex | Centrality |
| -----: | ---------: |
|      0 |       0.00 |
|      1 |       3.00 |
|      2 |       4.00 |
|      3 |       3.00 |
|      4 |       0.00 |

---

# 10. Test Cases

The following test cases were created and tested.

## Triangle Counting

| Test File     | Vertices |               Edges | Result       |
| ------------- | -------: | ------------------: | ------------ |
| `tc_6.txt`    |        6 |                   8 | 3 triangles  |
| `tc_10.txt`   |       10 |                  12 | 3 triangles  |
| `tc_100.txt`  |      100 |                 132 | 33 triangles |
| `tc_1000.txt` |     1000 | Generated test case | Tested       |

## Connected Components

| Test File     | Vertices |               Edges | Components |
| ------------- | -------: | ------------------: | ---------: |
| `cc_8.txt`    |        8 |                   4 |          4 |
| `cc_100.txt`  |      100 |                 132 |          1 |
| `cc_1000.txt` |     1000 | Generated test case |     Tested |

## Betweenness Centrality

| Test File     | Vertices |               Edges | Status |
| ------------- | -------: | ------------------: | ------ |
| `bc_5.txt`    |        5 |                   4 | Tested |
| `bc_100.txt`  |      100 |                 132 | Tested |
| `bc_1000.txt` |     1000 | Generated test case | Tested |
| `bc_5000.txt` |     5000 | Generated test case | Tested |

---

# 11. Important Test Results

### Triangle Counting - 6 Vertices

```text
Input File : Testcases\tc_6.txt
Vertices   : 6
Edges      : 8

Total triangles: 3

Triangles found:
(0, 1, 2)
(1, 2, 3)
(3, 4, 5)
```

### Triangle Counting - 100 Vertices

```text
Input File : Testcases\tc_100.txt
Vertices   : 100
Edges      : 132

Total triangles: 33
```

### Connected Components - 8 Vertices

```text
Input File : Testcases\cc_8.txt
Vertices   : 8
Edges      : 4

Number of components: 4
```

### Betweenness Centrality - 5 Vertices

```text
Input File : Testcases\bc_5.txt
Vertices   : 5
Edges      : 4

Vertex Centrality
0 0.00
1 3.00
2 4.00
3 3.00
4 0.00
```

---

# 12. Compilation

Open the terminal inside the `Assignment_02` directory.

Compile all source files using:

```cmd
gcc Src\csr.c Src\triangle_count.c Src\connected_comp.c Src\btwness_centrality.c driver.c -IInclude -o buddy.exe
```

If compilation is successful, `buddy.exe` will be created.

---

# 13. Running the Program

The executable requires the input file path as a command-line argument.

Example:

```cmd
.\buddy.exe Testcases\tc_6.txt
```

Another example:

```cmd
.\buddy.exe Testcases\cc_8.txt
```

For Betweenness Centrality:

```cmd
.\buddy.exe Testcases\bc_5.txt
```

The driver runs the implemented graph analytics algorithms and displays their results and execution time.

---

# 14. Test Case Generation

Additional test cases were generated using C programs.

### Triangle Counting Generator

```text
generate_tc_100.c
generate_tc_1000.c
```

They were compiled using:

```cmd
gcc generate_tc_100.c -o generate_tc_100.exe
```

and:

```cmd
gcc generate_tc_1000.c -o generate_tc_1000.exe
```

The generated test cases were then placed inside the `Testcases` directory.

### Betweenness Centrality Generator

```text
generate_bc_1000.c
```

was used to generate a larger test case for Betweenness Centrality.

---

# 15. Execution Time

Execution time is measured around the algorithm call using the C `clock()` function.

The measured section excludes:

* Input file reading
* Input parsing
* CSR construction
* Memory allocation used for setup
* Result printing

The reported execution time is displayed in milliseconds.

Example:

```text
Execution time: 5.000000 ms
```

For larger graphs, Betweenness Centrality requires significantly more computation time because shortest-path calculations are performed from every source vertex.

---

# 16. Algorithms Implemented

| Algorithm              | Source File            | Header File            |
| ---------------------- | ---------------------- | ---------------------- |
| CSR Representation     | `csr.c`                | `csr.h`                |
| Triangle Counting      | `triangle_count.c`     | `triangle_count.h`     |
| Connected Components   | `connected_comp.c`     | `connected_comp.h`     |
| Betweenness Centrality | `btwness_centrality.c` | `btwness_centrality.h` |

---

# 17. Conclusion

Assignment 02 implements three graph analytics algorithms using a CSR graph representation:

* Triangle Counting
* Connected Components
* Betweenness Centrality

The algorithms were implemented in C and tested using small and larger graph test cases.

The test results demonstrate that the implementation can correctly process different graph sizes and produce the expected triangle counts, connected-component assignments, and betweenness-centrality values.

The project also includes generated test cases for larger graphs to evaluate algorithm execution time and scalability.
