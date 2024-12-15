#include <iostream>
#include <vector>
#include <tuple>
#include <queue>
#include <algorithm>

int n, m, num_l; // n: number of stations, m: number of connections, num_l: number of lines.
std::vector<std::vector<std::pair<int, int>>> graph; // x, y, l: Station x and Station y are connected by line l.

int bfs(int x, int y) {
  std::queue<std::tuple<int, int>> queue; // current station, number of changes
  std::vector<bool> visited(n, false);
  std::vector<bool> lines_visited(num_l - 1, false);

  queue.emplace(x, 0); // Start 0 line changes
  visited[x] = true;

  while (!queue.empty()) {
    int current_station, num_changes;
    std::tie(current_station, num_changes) = queue.front();
    queue.pop();

    if (current_station == y) {
      return num_changes;
    }

    for (const auto& next : graph[current_station]) {
      int next_station = next.first;
      int connection_line = next.second;
      int next_changes = num_changes + (connection_line != -1 ? 1 : 0);

      if (!visited[next_station] || !lines_visited[connection_line]) {
        std::cout << "Queueing station " << next_station + 1 << " on line " << connection_line << " with " << next_changes << " changes" << std::endl;
        queue.emplace(next_station, next_changes);
        visited[next_station] = true;
        lines_visited[connection_line] = true;
      }
    }
  }

  int num_changes = -1;
  for (int i = 0; i < num_l - 1; ++i) {
    if (visited[i]) ++num_changes; 
  }
  return num_changes;
}

int main() {
  std::cin >> n >> m >> num_l; // n: number of stations, m: number of connections, num_l: number of lines.

  graph.resize(n);
  for (int i = 0; i < m; ++i) {
    int x, y, l;
    std::cin >> x >> y >> l, --x, --y;
    graph[x].emplace_back(y, l);
    graph[y].emplace_back(x, l);
  }

  int connectivity_index = 0;
  for (int x = 0; x < n; ++x) { // From each station to each station
    for (int y = x + 1; y < n; ++y) { // To avoid double searching since the graph is undirected.
      // BFS to find the minimum number of changes between two stations.
      int min_changes = bfs(x, y);
      std::cout << "From Station " << x+1 << " to station " << y+1 << " min_changes " << min_changes << std::endl;
      if (min_changes == -1) {
        std::cout << min_changes << std::endl;
        return 0;
      }
      // Find the maximum connectivity index.
      connectivity_index = std::max(connectivity_index, min_changes);
    }
  }

  std::cout << connectivity_index << std::endl;
}

