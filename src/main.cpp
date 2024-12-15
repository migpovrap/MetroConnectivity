#include <iostream>
#include <vector>
#include <tuple>
#include <queue>
#include <algorithm>
#include <climits>

int n, m, num_l; // n: number of stations, m: number of connections, num_l: number of lines.
std::vector<std::vector<std::pair<int, int>>> graph; // x, y, l: Station x and Station y are connected by line l.

int dijkstra(int start, int end) {
  // (number of changes, current station, current line)
  std::priority_queue<std::tuple<int, int, int>, std::vector<std::tuple<int, int, int>>, std::greater<std::tuple<int, int, int>>> priority_queue;
  std::vector<std::vector<int>> required_line_changes(n, std::vector<int>(num_l, INT_MAX));
  std::vector<bool> visited(n, false);

  for (int i = 0; i < num_l; ++i) {
    required_line_changes[start][i] = 0;
  }
  priority_queue.emplace(0, start, -1); // Start with no line (-1) and 0 changes

  while (!priority_queue.empty()) {
    int num_changes, current_station, current_line;
    std::tie(num_changes, current_station, current_line) = priority_queue.top();
    priority_queue.pop();

    if (current_station == end) { // If it as reached the desired station retunrs the required number of line changes
      return num_changes;
    }

    if (visited[current_station]) continue; // Advances to the next element in the queue if the stations was already visited
    visited[current_station] = true;

    for (const auto& next : graph[current_station]) {
      int next_station = next.first;
      int next_line = next.second;
      int next_changes = num_changes + (current_line != next_line && current_line != -1 ? 1 : 0); // If not the same line adds one line change, also checks if it is connected

      if (next_changes < required_line_changes[next_station][next_line]) {
        required_line_changes[next_station][next_line] = next_changes;
        priority_queue.emplace(next_changes, next_station, next_line);
      }
    }
  }

  return -1; // No path found
}

int main() {
  std::cin >> n >> m >> num_l; // n: number of stations, m: number of connections, num_l: number of lines.

  graph.resize(n);
  for (int i = 0; i < m; ++i) {
    int x, y, l;
    std::cin >> x >> y >> l;
    --x; --y; --l; // Convert to 0-based index
    graph[x].emplace_back(y, l);
    graph[y].emplace_back(x, l);
  }

  int connectivity_index = 0;
  for (int x = 0; x < n; ++x) { // From each station to each station
    for (int y = x + 1; y < n; ++y) { // To avoid double searching since the graph is undirected.
      // Dijkstra to find the minimum number of changes between two stations.
      int min_changes = dijkstra(x, y);
      //std::cout << "From Station " << x+1 << " to station " << y+1 << " min_changes " << min_changes << std::endl;
      if (min_changes == -1) {
        std::cout << -1 << std::endl;
        return 0;
      }
      // Find the maximum connectivity index.
      connectivity_index = std::max(connectivity_index, min_changes);
    }
  }

  std::cout << connectivity_index << std::endl;
  return 0;
}