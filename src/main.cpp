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
        if (changes > min_changes[current_station]) continue;

        for (const auto& station : graph[current_station]) {
            // Count a change only when switching to a different line
            int next_station = station.first;
            int next_line = station.second;
            int new_changes = (current_line != -1 && next_line != current_line) ? changes + 1 : changes; // If not the same line adds one line change, also checks if it is connected
            
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
    --x; --y; --l; // Convert to 0-based index
    graph[x].emplace_back(y, l);
    graph[y].emplace_back(x, l);
  }
  
  for (int i = 0; i < n; ++i) {
    if (graph[i].empty()) {
      std::cout << -1 << std::endl;
      return 0;
    }
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
      if (connectivity_index == 0) {
        // If we've found a path with 0 changes and haven't found any disconnected stations,
        // the answer must be 0
        continue;
      }
    }
  }

  std::cout << connectivity_index << std::endl;
  return 0;
}