#include <iostream>
#include <vector>
#include <tuple>
#include <queue>
#include <unordered_map>
#include <unordered_set>

std::vector<std::vector<std::pair<int, int>>> graph;

// Breadth-first search to find the minimum number of changes between two stations.
int bfs(int start, int end) {
  std::queue<std::tuple<int, int, int>> q; // current station, current line, number of changes)
  std::unordered_map<int, std::unordered_set<int>> visited; // station -> set of lines visited

  std::cout << "Start: " << start << ", End: " << end << std::endl;

  q.emplace(start, -1, 0); // Start with no line (-1) and 0 changes
  visited[start].insert(-1);

  while (!q.empty()) {
    int current_station, current_line, changes;
    std::tie(current_station, current_line, changes) = q.front();
    q.pop();

    if (current_station == end) {
      return changes;
    }

    for (const auto& next : graph[current_station]) {
      int next_station = next.first;
      int next_line = next.second;
      if (visited[next_station].find(next_line) == visited[next_station].end()) {
        visited[next_station].insert(next_line);
        q.emplace(next_station, next_line, changes + (current_line != next_line));
      }
    }
  }

  return -1;
}

int main() {
  int n, m, num_l; // n: number of stations, m: number of connections, num_l: number of lines.
  std::cin >> n >> m >> num_l;

  // x, y, l: Station x and Station y are connected by line l.
  graph.resize(n);
  for (int i = 0; i < m; ++i) {
    int x, y, l;
    std::cin >> x >> y >> l, --x, --y;
    graph[x].emplace_back(y, l);
    graph[y].emplace_back(x, l);
  }

  int connectivity_index = 0;
  for (int i = 0; i < n; ++i) { // From each station to each station
    for (int j = i + 1; j < n; ++j) { // To avoid double counting
      // Find the maximum connectivity index
      int min_changes = bfs(i, j);
      if (min_changes == -1) {
        std::cout << min_changes << std::endl;
        return 0;
      };
      connectivity_index = std::max(min_changes, connectivity_index);
    }
  }
}


/**
 * Debug print
  for (int i = 1; i <= n; ++i) {
    std::cout << "Station " << i << ": ";
    for (const auto& edge : graph[i]) {
      std::cout << "(to: " << edge.first << ", line: " << edge.second << ") ";
    }
    std::cout << std::endl;
  }
 */