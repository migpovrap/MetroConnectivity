#include <iostream>
#include <vector>
#include <tuple>
#include <queue>
#include <algorithm>
#include <climits>

int n, m, num_l; // n: number of stations, m: number of connections, num_l: number of lines.
std::vector<std::vector<std::pair<int, int>>> graph;  // x, y, l: Station x and Station y are connected by line l.
std::vector<std::vector<int>> station_to_lines;  // Stations and the corresponding lines
std::vector<std::vector<std::pair<int, int>>> line_graph;  // Connetions of all the lines

// Function to build a line graph representing transitions between lines and stations
void build_line_graph() {
  station_to_lines.assign(n, std::vector<int>());
  line_graph.assign(num_l, std::vector<std::pair<int, int>>());
  
  // Map each station to the lines that pass through it
  for (int i = 0; i < n; ++i) {
    for (const auto& edge : graph[i]) {
      int line = edge.second;
      station_to_lines[i].push_back(line); // Add the line to the station's line list
    }
  }
  
  // Build the line graph by connecting lines that share stations
  for (int station = 0; station < n; ++station) {
    auto& lines = station_to_lines[station];
    sort(lines.begin(), lines.end()); // Sort the lines to prepare for removing duplicates
    lines.erase(unique(lines.begin(), lines.end()), lines.end()); // Remove duplicate lines
    
    // Add all pairs of lines that share this station
    for (size_t i = 0; i < lines.size(); ++i) {
      for (size_t j = i + 1; j < lines.size(); ++j) {
        line_graph[lines[i]].emplace_back(lines[j], station);
        line_graph[lines[j]].emplace_back(lines[i], station);
      }
    }
  }
}

int bfs(int start, int end) {
  std::queue<std::pair<int, int>> queue;  // (line, changes)
  std::vector<bool> visited(num_l, false);
  
  // Use existing station_to_lines mapping
  for (int start_line : station_to_lines[start]) {
    queue.emplace(start_line, 0);
    visited[start_line] = true;
  }
  
  while (!queue.empty()) {
    int current_line = queue.front().first;
    int changes = queue.front().second;
    queue.pop();
    
    // Use existing station_to_lines mapping
    if (std::find(station_to_lines[end].begin(), station_to_lines[end].end(), current_line) != station_to_lines[end].end()) {
      return changes;
    }
    
    // Use existing line_graph
    for (const auto& node : line_graph[current_line]) {
      int next_line = node.first;
      int station = node.second;
      if (!visited[next_line]) {
        visited[next_line] = true;
        queue.emplace(next_line, changes + 1);
      }
    }
  }
  return -1;
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
    graph[x].emplace_back(y, l); // Add connection from station x to station y
    graph[y].emplace_back(x, l); // Add reverse connection from station y to station x
  }

  // Check if any station is disconnected
  for (int i = 0; i < n; ++i) {
    if (graph[i].empty()) {
      std::cout << -1 << std::endl;
      return 0;
    }
  }

  build_line_graph(); // Construct the line graph based on shared stations

  /*
  for (int i = 0; i < num_l; ++i) {
    std::cout << "Line " << i + 1 << " connections: ";
    for (const auto& connection : line_graph[i]) {
      std::cout << "(Line " << connection.first + 1 << ", Station " << connection.second + 1 << ") ";
    }
    std::cout << std::endl;
  }
  */

  int connectivity_index = 0;
  for (int x = 0; x < n; ++x) { // From each station to each station
    for (int y = x + 1; y < n; ++y) { // To avoid double searching since the graph is undirected.
      // Dijkstra to find the minimum number of changes between two stations.
      int min_changes = bfs(x, y);
      if (min_changes == -1) { // If no path is found
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
