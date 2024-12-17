#include <algorithm>
#include <climits>
#include <iostream>
#include <queue>
#include <set>
#include <vector>

int n, m, num_l; // n: number of stations, m: number of connections, num_l: number of lines.
std::vector<std::vector<std::pair<int, int>>> graph;  // x, y, l: Station x and Station y are connected by line l.
std::vector<std::vector<int>> station_to_lines;  // Stations and the corresponding lines
std::vector<std::vector<std::pair<int, int>>> line_graph;  // Connetions of all the lines
int connectivity_index = -1;

void build_line_graph() {
  station_to_lines.assign(n, std::vector<int>());
  line_graph.assign(num_l, std::vector<std::pair<int, int>>());

  // Track stations per line
  std::vector<std::set<int>> line_stations(num_l);

  for (int station = 0; station < n; ++station) {
    std::set<int> station_lines;

    // Collect lines and update line_stations
    for (int i = 0; i < (int) graph[station].size(); ++i) {
      std::pair<int, int> edge = graph[station][i];
      station_lines.insert(edge.second);
      line_stations[edge.second].insert(station);
    }

    // If all stations are connect via the same line the number of changes is 0
    if (station_lines.size() == (size_t) num_l) {
      connectivity_index = 0;
      return;
    }

    station_to_lines[station].assign(station_lines.begin(), station_lines.end());

    // Build line connections
    std::vector<int> station_lines_vec(station_lines.begin(), station_lines.end());
    for (size_t i = 0; i < station_lines_vec.size(); ++i) {
      for (size_t j = i + 1; j < station_lines_vec.size(); ++j) {
        line_graph[station_lines_vec[i]].emplace_back(station_lines_vec[j], station);
        line_graph[station_lines_vec[j]].emplace_back(station_lines_vec[i], station);
      }
    }
  }
}

int bfs(int source_line) {
  std::queue<std::pair<int, int>> queue;  // (line_id, changes)
  std::vector<int> dist(num_l, INT_MAX);  // Track minimum changes to reach each line

  // Initialize queue with 0 changes
  queue.emplace(source_line, 0);
  dist[source_line] = 0;  // Source line is already visited

  while (!queue.empty()) {
    int current_line = queue.front().first;
    int changes = queue.front().second;
    queue.pop();

    // Skip if we found a better path
    if (changes > dist[current_line]) continue;

    for (int i = 0; i < (int) line_graph[current_line].size(); ++i) {
      std::pair<int, int> node = line_graph[current_line][i];
      int next_line = node.first;
      // Only count a change if moving to different line
      int new_changes = (next_line != current_line) ? changes + 1 : changes;

      if (new_changes < dist[next_line]) {
        dist[next_line] = new_changes;
        queue.push(std::make_pair(next_line, new_changes));
      }
    }
  }

  // Find the minumum number of line changes required to reach all other lines
  int min_required_changes = 0;
  for (int i = 0; i < num_l; ++i) {
    if (dist[i] == INT_MAX) return -1;
    min_required_changes = std::max(min_required_changes, dist[i]);
  }

  return min_required_changes;
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

  build_line_graph(); // Build the SCC graph

  // Early return if station connects all lines
  if (connectivity_index == 0) {
    std::cout << 0 << std::endl;
    return 0;
  }

  // Executes the bfs for each line
  for (int line = 0; line < num_l; ++line) {
    int min_required_changes = bfs(line);
    connectivity_index = std::max(connectivity_index, min_required_changes);
  }
  std::cout << connectivity_index << std::endl;
  return 0;
}
