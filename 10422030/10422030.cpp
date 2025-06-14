#include <iostream>         // For standard I/O operations (cin, cout)
#include <fstream>          // For file input and output streams
#include <sstream>          // For string stream manipulation
#include <vector>           // For using the vector container
#include <map>              // For using the map container
#include <string>           // For using the string class
#include <algorithm>        // For utility functions like remove, reverse
#include <cctype>           // For character handling functions like isdigit

// final code complete in 

using namespace std;        

//Define a structure for an edge in the graph
struct Edge {
    int to;         //the destination vertex index
    int weight;     //The weight (travel time) of the edge
};

// Define a structure for representing a graph
struct Graph {
    int num_vertices;                              //Number of vertices in the graph
    vector<string> index_to_vertex;                // Mapping from vertex index to vertex name
    map<string, int> vertex_to_index;              // Mapping from vertex name to vertex index
    vector<int> cycle_times;                       //traffic light cycle time for each vertex
    vector<vector<Edge>> adjacency_list;           // Adjacency list representation of graph
};

// Function declarations
vector<Graph> read_graphs_from_file(const string& filename);
void compute_shortest_path_with_constraints(const Graph& g, ofstream& outFile);
int find_min_distance_vertex(const vector<int>& distances, const vector<bool>& visited);

//main function
int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: ./10422030 tests.txt output_10422030.txt\n";  // Error if incorrect args
        return 1;
    }

    string input_filename = argv[1];         // Input filename from command line
    string output_filename = argv[2];        // Output filename from command line

    vector<Graph> graphs = read_graphs_from_file(input_filename);  //read all graphs from input

    ofstream outFile(output_filename);       //open output file stream
    if (!outFile.is_open()) {
        cerr << "Cannot open output file!\n"; //error if file can't be opened
        return 1;
    }

    // For each graph, compute the constrained shortest path and write output
    for (const auto& g : graphs) {
        compute_shortest_path_with_constraints(g, outFile);
    }

    outFile.close();     // Close output file stream
    return 0;
}

//stage 1 8/6/2025
// Function to read multiple graphs from a file
vector<Graph> read_graphs_from_file(const string& filename) {
    ifstream file(filename);           //Open file for reading
    if (!file.is_open()) {
        cerr << "Cannot open input file!\n";
        exit(1);                        // Exit if file can't be opened
    }

    vector<Graph> graphs;              // Vector to store multiple graphs
    string line;                       // for reading lines from file

    int num_graphs = 0;
    while (getline(file, line)) {
        if (!line.empty()) {
            num_graphs = stoi(line);   //First non-empty line is number of graphs
            break;
        }
    }

    for (int g = 0; g < num_graphs; ++g) {
        Graph graph;

        //Read number of vertices for this graph
        while (getline(file, line) && line.empty());
        if (line.empty()) break;
        graph.num_vertices = stoi(line);

        //Read vertex names and their cycle times
        for (int i = 0; i < graph.num_vertices; ++i) {
            getline(file, line);
            stringstream ss(line);
            string vertex;
            int cycle;
            getline(ss, vertex, ',');      //Vertex name before comma
            ss >> cycle;                   // cycle time after comma
            vertex.erase(remove(vertex.begin(), vertex.end(), ' '), vertex.end());

            //Store mappings and cycle time
            graph.vertex_to_index[vertex] = i;
            graph.index_to_vertex.push_back(vertex);
            graph.cycle_times.push_back(cycle);
        }

        graph.adjacency_list.resize(graph.num_vertices);  //initialize adjacency list

        streampos last_pos = file.tellg();  //remember last read position
        while (getline(file, line)) {
            if (line.empty()) continue;

            stringstream ss(line);
            string token;
            ss >> token;

            //check if line is start of next graph (a number)
            if (isdigit(token[0])) {
                file.seekg(last_pos);      // rewind to previous position
                break;                     // exit edge reading loop
            }

            string u_str, v_str;
            int weight;

            // Parse edge line: u, v, weight
            ss.clear();
            ss.str(line);
            getline(ss, u_str, ',');
            getline(ss, v_str, ',');
            ss >> weight;

            // Remove whitespace
            u_str.erase(remove(u_str.begin(), u_str.end(), ' '), u_str.end());
            v_str.erase(remove(v_str.begin(), v_str.end(), ' '), v_str.end());

            // Convert vertex names to indices
            int u = graph.vertex_to_index[u_str];
            int v = graph.vertex_to_index[v_str];

            // Add bidirectional edges
            graph.adjacency_list[u].push_back({v, weight});
            graph.adjacency_list[v].push_back({u, weight});

            last_pos = file.tellg();  // Update last position
        }

        graphs.push_back(graph);  // Add graph to list
    }

    file.close();  // Close input file
    return graphs;
}

// stage 2: apply Dijsktra's algorithm. (10/6/2025)
// Implements Dijkstra’s algorithm with traffic light constraints
void compute_shortest_path_with_constraints(const Graph& g, ofstream& outFile) {
    const int INF = 1e9;  // A large value representing infinity
    vector<int> dist(g.num_vertices, INF);    // Distance from start to each vertex
    vector<int> parent(g.num_vertices, -1);   // To reconstruct path
    vector<bool> visited(g.num_vertices, false);  // Visited flag

    int start = g.vertex_to_index.at("A");    //Start vertex index ("A")
    int goal = g.vertex_to_index.at("G");     // Goal vertex index ("G")
    dist[start] = 0;                           // distance to start is 0

    for (int i = 0; i < g.num_vertices; ++i) {
        int u = find_min_distance_vertex(dist, visited);  // get next vertex with min dist
        if (u == -1) break;
        visited[u] = true;

        for (const Edge& e : g.adjacency_list[u]) {
            int v = e.to;
            int travel_time = e.weight;
            int wait_time = 0;

            int current_time = dist[u];  // Current arrival time at u

            // Skip traffic light if u is the starting vertex
            if (u != start) {
                int cycle_u = g.cycle_times[u];
                if (current_time % cycle_u != 0) {
                    wait_time = cycle_u - (current_time % cycle_u);  // Wait until next green
                }
            }

            int arrival_time = current_time + wait_time + travel_time;

            // Relaxation step: if shorter path is found, update
            if (arrival_time < dist[v]) {
                dist[v] = arrival_time;
                parent[v] = u;
            }
        }
    }

    //stage 3 check 
    // If no path found, output message
    if (dist[goal] == INF) {
        cout << "No path from A to G\n";
        outFile << "No path from A to G\n";
        return;
    }

    // Reconstruct shortest path using parent array
    vector<string> path;
    for (int v = goal; v != -1; v = parent[v]) {
        path.push_back(g.index_to_vertex[v]);
    }
    reverse(path.begin(), path.end());

    //stage 3: print output (11/6/2025)
    // Output result to terminal
    cout << dist[goal] << "\n";
    for (int i = 0; i < path.size(); ++i) {
        cout << path[i];
        if (i < path.size() - 1) cout << " ";
    }
    cout << "\n";

    // Output result to file
    outFile << dist[goal] << "\n";
    for (int i = 0; i < path.size(); ++i) {
        outFile << path[i];
        if (i < path.size() - 1) outFile << " ";
    }
    outFile << "\n";
}

//stage 2: (10/6/2025)
// Find the vertex with the minimum distance value that has not been visited
int find_min_distance_vertex(const vector<int>& distances, const vector<bool>& visited) {
    int min_dist = 1e9;  // Start with large value
    int min_index = -1;
    for (int i = 0; i < distances.size(); ++i) {
        if (!visited[i] && distances[i] < min_dist) {
            min_dist = distances[i];
            min_index = i;
        }
    }
    return min_index;  // Return index of closest unvisited vertex
}

