#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <algorithm>
#include <climits>

int n, m, num_l; // n: number of stations, m: number of connections, num_l: number of lines.
std::vector<std::vector<std::pair<int, int>>> graph;  // x, y, l: Station x and Station y are connected by line l.
std::vector<std::vector<int>> station_to_lines;  // Stations and the corresponding lines
std::vector<std::vector<std::pair<int, int>>> line_graph;  // Connetions of all the lines

void build_line_graph() {
  station_to_lines.assign(n, std::vector<int>());
  line_graph.assign(num_l, std::vector<std::pair<int, int>>());
  
  // Track stations per line
  std::vector<std::set<int>> line_stations(num_l);
  
  for (int station = 0; station < n; ++station) {
    std::set<int> station_lines;
    
    // Collect lines and update line_stations
    for (const auto& edge : graph[station]) {
      station_lines.insert(edge.second);
      line_stations[edge.second].insert(station);
    }
    
    station_to_lines[station].assign(station_lines.begin(), station_lines.end());
    
    // Build line connections
    for (auto it1 = station_lines.begin(); it1 != station_lines.end(); ++it1) {
      auto it2 = it1;
      for (++it2; it2 != station_lines.end(); ++it2) {
        line_graph[*it1].emplace_back(*it2, station);
        line_graph[*it2].emplace_back(*it1, station);
      }
    }
  }
}

int bfs(int source_line) {
  std::queue<std::pair<int, int>> queue;  // (line_id, changes)
  std::vector<bool> visited(num_l, false);
  int max_changes = 0;
  
  queue.emplace(source_line, 0);
  visited[source_line] = true;
  
  while (!queue.empty()) {
    int current_line = queue.front().first;
    int changes = queue.front().second;
    queue.pop();
    
    max_changes = std::max(max_changes, changes);
    
    for (const auto& node : line_graph[current_line]) {
      int next_line = node.first;
      if (!visited[next_line]) {
        visited[next_line] = true;
        queue.emplace(next_line, changes + 1);
      }
    }
  }
  
  // Check if all lines were reached
  for (int i = 0; i < num_l; ++i) {
    if (!visited[i]) return -1;
  }
  
  return max_changes;
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

  build_line_graph();

  int connectivity_index = 0;
  for (int line = 0; line < num_l; ++line) {
    int max_changes = bfs(line);
    if (max_changes == -1) {
      std::cout << -1 << std::endl;
      return 0;
    }
    connectivity_index = std::max(connectivity_index, max_changes);
  }
  
  std::cout << connectivity_index << std::endl;
  return 0;
}
