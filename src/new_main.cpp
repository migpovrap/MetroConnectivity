#include <iostream>
#include <vector>
#include <tuple>
#include <queue>
#include <algorithm>
#include <climits>
#include <stack>

int n, m, num_l; // n: number of stations, m: number of connections, num_l: number of lines.
std::vector<std::vector<std::pair<int, int>>> graph; // x, y, l: Station x and Station y are connected by line l.
std::vector<std::vector<int>> scc_graph; // Graph of SCCs
std::vector<int> scc_ids; // SCC id for each station
std::vector<bool> visited; // Visited stations for SCC discovery
std::stack<int> dfs_stack; // Stack used in Tarjan's Algorithm to track the current path.
int scc_counter = 0; // Counter for the number of SCCs identified.

// Data used for the Tarjan's algorithm
std::vector<int> low, ids; // `low` and `ids` arrays for Tarjan's Algorithm.
int id_counter = 0; // Counter for assigning unique ids during DFS traversal.

// Function implementing Tarjan's Algorithm for finding SCCs
void tarjanDFS(int at) {
  visited[at] = true; // Mark the current node as visited
  ids[at] = low[at] = id_counter++; // Assign a unique id and initialize its low-link value
  dfs_stack.push(at); // Push the current node onto the stack

  // Traverse all neighbors of the current station
  for (const auto& neighbor : graph[at]) {
    int to = neighbor.first;
    if (ids[to] == -1) { // If the neighbor hasn't been visited, perform DFS on it
      tarjanDFS(to);
      low[at] = std::min(low[at], low[to]); // Update the low-link value
    } else if (visited[to]) { // If the neighbor is on the stack, it's part of the current SCC
      low[at] = std::min(low[at], ids[to]);
    }
  }

  // If the current station is a root node of an SCC
  if (low[at] == ids[at]) {
    while (true) {
      int node = dfs_stack.top();
      dfs_stack.pop();
      visited[node] = false; // Mark the node as no longer in the stack
      scc_ids[node] = scc_counter; // Assign the SCC id to the node
      if (node == at) break; // Stop when the root node is reached
    }
    scc_counter++; // Increment the SCC counter
  }
}

// Function to find all SCCs in the graph using Tarjan's Algorithm
void findSCCs() {
  ids.assign(n, -1);  // Initialize ids array with -1
  low.assign(n, 0); // Initialize low-link values to 0
  visited.assign(n, false); // Mark all stations as unvisited
  scc_ids.assign(n, -1); // Initialize SCC ids to -1

  // Perform DFS for each station that hasn't been visited
  for (int i = 0; i < n; ++i) {
    if (ids[i] == -1) tarjanDFS(i);
  }
}

int dijkstra(int start, int end) {
  // (number of changes, current station, current line)
  std::priority_queue<std::tuple<int, int, int>, std::vector<std::tuple<int, int, int>>, std::greater<std::tuple<int, int, int>>> pq;
  std::vector<int> min_changes(n, INT_MAX);
  
  pq.emplace(0, start, -1); // Start with no line (-1) and 0 changes
  min_changes[start] = 0;

  while (!pq.empty()) {
    int changes = std::get<0>(pq.top());
    int current_station = std::get<1>(pq.top());
    int current_line = std::get<2>(pq.top());
    pq.pop();

    if (current_station == end) return changes; // If it as reached the desired station retunrs the required number of line changes
    if (changes > min_changes[current_station]) continue; // If we've already found a better path, skip this one

    // Traverse all neighbors of the current station
    for (const auto& station : graph[current_station]) {
      // Count a change only when switching to a different line
      int next_station = station.first;
      int next_line = station.second;
      int new_changes = (current_line != -1 && next_line != current_line) ? changes + 1 : changes; // If not the same line adds one line change, also checks if it is connected
      // If a better path is found, update and push to the queue
      if (new_changes < min_changes[next_station]) {
        min_changes[next_station] = new_changes;
        pq.emplace(new_changes, next_station, next_line);
      }
    }
  }
  return -1; // No path found
}

int main() {
  std::ios::sync_with_stdio(0); // disable sync with c libs (printf/scanf)
  std::cin.tie(0); // discard cin buffer after each line of input

  std::cin >> n >> m >> num_l; // n: number of stations, m: number of connections, num_l: number of lines.

  graph.resize(n);
  for (int i = 0; i < m; ++i) {
    int x, y, l;
    std::cin >> x >> y >> l;
    --x; --y; --l; // Convert to 0-based indexing
    graph[x].emplace_back(y, l);
    graph[y].emplace_back(x, l);
  }

  // Find all SCCs in the graph
  findSCCs();

  if (scc_counter > 1) { // If more than one scc is found, graph is not fully connected
    std::cout << -1 << std::endl;
    return 0;
  }

  int connectivity_index = 0;
  for (int x = 0; x < n; ++x) { // From each station to each station
    for (int y = x + 1; y < n; ++y) { // To avoid double searching since the graph is undirected.
      // Dijkstra to find the minimum number of changes between two stations.
      int min_changes = dijkstra(x, y);
      if (min_changes == -1) {
        std::cout << -1 << std::endl;
        return 0;
      }
       // Find the maximum connectivity index.s
      connectivity_index = std::max(connectivity_index, min_changes);
    }
  }

  std::cout << connectivity_index << std::endl;
  return 0;
}
