# CS509 Assignment 3 — Buddy Project

## Algorithms

### Buddy Tasks

- Gradient Descent
- Maxflow-Mincut using Dinic's algorithm

The assignment specification requires Gradient Descent to use a generic
one-variable polynomial representation and Maxflow-Mincut to use a directed
capacity graph. Gradient Descent does not use CSR; Maxflow-Mincut does.

---

# 1. Gradient Descent

Gradient Descent minimizes:

```text
f(x) = c0 + c1*x + c2*x^2 + ... + cd*x^d
```

The derivative is evaluated from the coefficient array:

```text
f'(x) = c1 + 2*c2*x + ... + d*cd*x^(d-1)
```

The update is:

```text
x_new = x - learning_rate * f'(x)
```

The algorithm stops when:

```text
|f'(x)| <= tolerance
```

or when `MAX_ITERATIONS` is reached.

The same implementation supports all required polynomial degrees and does
not hard-code separate functions for individual test cases.

---

## Gradient Descent Input Format

```text
DEGREE d
COEFFICIENTS c0 c1 c2 ... cd
INITIAL_X x0
LEARNING_RATE alpha
TOLERANCE epsilon
MAX_ITERATIONS n
```

Example:

```text
DEGREE 6
COEFFICIENTS 0 0 1 0 0.5 0 0.1
INITIAL_X 2
LEARNING_RATE 0.02
TOLERANCE 0.000001
MAX_ITERATIONS 20000
```

---

## Required Gradient Descent Test Cases

| File | Degree | Initial x | Learning Rate | Tolerance | Max Iterations | Expected x* |
|---|---:|---:|---:|---:|---:|---:|
| `gd_01.txt` | 2 | 0 | 0.10 | 1e-6 | 5,000 | 3 |
| `gd_02.txt` | 4 | 2 | 0.02 | 1e-6 | 10,000 | 0 |
| `gd_03.txt` | 6 | 2 | 0.02 | 1e-6 | 20,000 | 0 |
| `gd_04.txt` | 8 | 2 | 0.01 | 1e-8 | 50,000 | 0 |
| `gd_05.txt` | 10 | 2 | 0.005 | 1e-10 | 100,000 | 0 |

---

# 2. Maxflow-Mincut

Maxflow-Mincut calculates the maximum amount of flow that can be sent
from a source vertex to a sink vertex without exceeding edge capacities.

The implementation uses **Dinic's algorithm**.

The corresponding minimum cut is extracted from the final residual graph.

The maximum-flow value must equal the minimum-cut capacity.

---

## Maxflow Input Format

```text
V E
u0 degree neighbor1 capacity1 neighbor2 capacity2 ...
u1 degree neighbor1 capacity1 neighbor2 capacity2 ...
...
u(V-1) degree ...
SOURCE s
SINK t
```

Only the original outgoing capacity edges are stored in the input.
Reverse residual edges are created internally.

Example:

```text
6 10
0 2 1 16 2 13
1 2 2 10 3 12
2 2 1 4 4 14
3 2 2 9 5 20
4 2 3 7 5 4
5 0
SOURCE 0
SINK 5
```

Expected maximum flow:

```text
23
```

---

## Required Maxflow Test Cases

| File | Vertices |
|---|---:|
| `maxflow_10.txt` | 10 |
| `maxflow_100.txt` | 100 |
| `maxflow_1000.txt` | 1,000 |
| `maxflow_10000.txt` | 10,000 |
| `maxflow_50000.txt` | 50,000 |

`maxflow_10_required.txt` is also included as a 10-vertex generated
test case. `maxflow_10.txt` contains the specification's example graph.

Large graphs are kept sparse.

---

# 3. CSR Representation

Maxflow-Mincut uses the existing CSR representation.

CSR contains:

- `row_ptr`
- `col_idx`
- `values`

For Maxflow-Mincut, `values` stores edge capacities.

CSR conversion is preprocessing and is not included in the measured
algorithm execution time.

Gradient Descent does not use CSR because it reads polynomial coefficients
and scalar parameters directly from its input file.

---

# 4. Timing

Only algorithm execution time is measured.

The following are excluded:

- File reading
- Input parsing
- Setup memory allocation
- CSR conversion
- Result printing
- Output file writing

For Maxflow-Mincut, residual-network initialization and minimum-cut extraction
are part of the required algorithm operation and remain inside the timed
section.

Timing follows:

```text
Read input
    ↓
Build data structure
    ↓
Convert to CSR if required
    ↓
START TIMER
    ↓
Run algorithm
    ↓
STOP TIMER
    ↓
Print result
    ↓
Write output file
```

---

# 5. Compilation

From the `Assignment_3_Buddy` directory:

```bash
gcc -Wall -Wextra -O2 -IInclude Driver/main.c Src/csr.c Src/gradient_descent.c Src/maxflow.c -lm -o assignment3_buddy
```

Or:

```bash
make
```

---

# 6. Execution

## Gradient Descent

Linux/macOS:

```bash
./assignment3_buddy gd Testcases/Gradient_Descent/gd_01.txt
```

Windows PowerShell:

```powershell
.\assignment3_buddy.exe gd .\Testcases\Gradient_Descent\gd_01.txt
```

## Maxflow-Mincut

Linux/macOS:

```bash
./assignment3_buddy maxflow Testcases/Maxflow/maxflow_10.txt
```

Windows PowerShell:

```powershell
.\assignment3_buddy.exe maxflow .\Testcases\Maxflow\maxflow_10.txt
```

---

# 7. Output

The driver prints only the algorithm result and execution time.

The same result is stored in the `Output` directory.

Example Gradient Descent output:

```text
Degree: 2
Final x: 3.0000000000
Final f(x): 0.0000000000
Iterations: <value>
Converged: true
Execution time: <value> ms
```

Example Maxflow-Mincut output:

```text
Source: 0
Sink: 5
Maximum flow: 23
Minimum cut capacity: 23
Source side: 0 1 2 4
Sink side: 3 5
Cut edges:
1 3 12
4 3 7
4 5 4
Execution time: <value> ms
```

Output files are automatically created as:

```text
Output/gradient_descent_gd_01.txt
Output/maxflow_maxflow_10.txt
```

---

# 8. Performance Notes

## Gradient Descent

The required polynomial degrees increase from 2 to 10.

Higher degree increases the arithmetic performed during every iteration.

The required maximum iterations also increase for the higher-degree test cases.

## Maxflow-Mincut

Dinic's algorithm is used because the required graph sizes are relatively
large.

Large networks are generated as sparse directed capacity graphs.

The required sizes are:

```text
10
100
1,000
10,000
50,000
```

---
# 8. Timing Results

Only the algorithm execution time is measured. File reading, input parsing,
setup, CSR conversion, result printing, and output file writing are excluded.

## Gradient Descent Timing Results

| Test Case | Degree | Iterations | Execution Time | Status |
|---|---:|---:|---:|:---:|
| `gd_01.txt` | 10|1000000 | 33.00 ms | Pass |
| `gd_02.txt` | 10|5000000 |  163.00 ms | Pass |
| `gd_03.txt` | 10|10000000 |  326.00 ms | Pass |
| `gd_04.txt` | 10| 20000000 |539.00 ms | Pass |
| `gd_05.txt` | 10| 30000000| 543.00 ms | Pass |

## Maxflow-Mincut Timing Results

| Test Case | Vertices | Edges | Maximum Flow | Min-Cut Capacity | Execution Time | Status |
|---|---:|---:|---:|---:|---:|:---:|
| `maxflow_10.txt` | 6 | 10 | 23 | 23 | 0 ms | Pass |
| `maxflow_100.txt` | 100 | 200 | 36 | 36 | 0  ms | Pass |
| `maxflow_1000.txt` | 1,000 | 2,000 | 10 | 10 | 0 ms | Pass |
| `maxflow_10000.txt` | 10,000 | 20,000 | 10 | 10 | 9 ms | Pass |
| `maxflow_50000.txt` | 50,000 | 100,000 | 14 | 14 | 79 ms | Pass |

### Timing Method

The timer starts immediately before the algorithm function is called and
stops immediately after the algorithm completes.

For Maxflow-Mincut, residual-network initialization and minimum-cut extraction
are included in the timed algorithm section.

The following operations are excluded from the measured time:

- File reading
- Input parsing
- Memory allocation during setup
- CSR conversion
- Result printing
- Output file writing

The execution time is reported in milliseconds.


## High-Resolution Timing

The driver uses a high-resolution monotonic timer instead of `clock()`.

- Windows: `QueryPerformanceCounter`
- Linux/macOS: `clock_gettime(CLOCK_MONOTONIC)`

Only the algorithm call is inside the timed region. File input, preprocessing,
CSR conversion, output generation, and file writing remain outside the timed region.
