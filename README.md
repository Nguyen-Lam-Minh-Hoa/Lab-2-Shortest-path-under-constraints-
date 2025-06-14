
# Shortest Path under Traffic Light Constraints 🚦

This project implements **Dijkstra's algorithm** with traffic light constraints to find the shortest path from vertex **A** to **G** in one or multiple undirected graphs.

## 🔧 Features

- Parse multiple undirected graphs from input file.
- Read vertex traffic light cycle times and weighted edges.
- Implement Dijkstra’s algorithm with additional waiting time at each vertex based on traffic light cycles.
- Output shortest path and total travel time to both terminal and output file.

---

## 📁 File Structure

- `10422030.cpp` — Main source file containing full implementation.
- `tests.txt` — Input file including sample graphs (see below).
- `output_10422030.txt` — Output file containing shortest paths and cost.
- `how_to_run.txt` — Instructions for compiling and running on Windows/Linux.
- `README.md` — This file.

---

## 📥 Input Format

Input file starts with the number of graphs.  
Each graph includes:

1. A line with the number of vertices `n`.
2. `n` lines of format: `VertexName, CycleTime`.
3. Edges in format: `Vertex1, Vertex2, TravelTime`.

### Example:

```
2
7
A, 2
B, 3
C, 5
D, 4
E, 6
F, 2
G, 1
A, B, 4
A, C, 2
...
```

---

## 🚀 How to Run

See full instructions in [how_to_run.txt](how_to_run.txt), summarized here:

### On Linux/macOS:

```bash
g++ 10422030.cpp -o 10422030
./10422030 tests.txt output_10422030.txt
```

### On Windows (Using MinGW):

```cmd
g++ 10422030.cpp -o 10422030.exe
10422030.exe tests.txt output_10422030.txt
```

---

## 🧪 Example Output

```
19
A B D F G
13
A B D E G
```

---

## 📚 References

- Dijkstra’s Algorithm – [GeeksforGeeks](https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-using-priority_queue-stl/)
- Traffic Light Scheduling – [ResearchGate](https://www.researchgate.net/publication/228860354_Shortest_Path_Problems_with_Traffic_Lights)
- C++ File I/O – [cplusplus.com](https://cplusplus.com/doc/tutorial/files/)

---

## 🧠 Author

- Student ID: 10422030  
- Course: Algorithms and Data Structures  
- Lab 2: Shortest Path with Constraints  
- Name: Nguyen Lam Minh Hoa

---

## 📝 License

This project is for educational purposes. All rights reserved © 2025.
