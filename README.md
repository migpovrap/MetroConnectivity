# MetroConnectivity
### Problem Description
This program aims to evaluate the efficiency of a metro network by calculating the minimum number of changes required to travel between any two stations in the metro network.

This is achieved through a metro connectivity index (mc), which is defined as follows:
```
mc = max{ML(e1, e2) | e1, e2 ∈ Metro}
```
where ML(e1, e2) denotes the minimum number of line changes required to travel between metro stations e1 and e2.

### Input
The input file contains information about the entire metro network, which is defined as an undirected multi-graph, where each vertex represents a station in the metro network and each edge represents an indivisible section of a line. Thus, the input file is defined as follows:
- A line containing three integers: the number n of stations (n ≥ 2), the number m of connections (m ≥ 0), and the number l of lines (l ≥ 0) in the metro network;
- A list where each line i of the file contains three integers x, y, and l, representing that station x is directly connected to station y on metro line l.

### Output
The program should write to the output an integer s corresponding to the connectivity index of the given metro network, except in the following two cases:
- If no line changes are necessary, the expected output is 0.
- If there are at least two stations e1, e2 without a connection between them, the expected output is -1.

### Example
#### Example 1
**Input:**
```
7 8 3
3 2 1
2 7 1
7 5 1
2 6 2
6 4 2
4 1 2
4 1 3
1 5 3
```
**Output:**
`1`

#### Example 2
**Input:**
```
7 6 3
3 2 1
2 7 1
7 5 1
2 6 2
6 4 2
1 5 3
```
**Output:**
`2`

### Solution Description
Our solution involves creating a new graph using strongly connected components (SCC) and then performing a breadth-first search (BFS) for each metro line to determine the minimum number of line changes required.
