#include <algorithm>
#include <climits>
#include <iostream>
#include <queue>
#include <set>
#include <vector>

int num_stations, num_connections, num_lines;
// Adjacency list representing the graph.
std::vector<std::vector<std::pair<int, int>>> graph; // index: station1, values: (station2, connecting line).
std::vector<std::vector<int>> station_to_lines; // index: stations, values: the various lines the station is on simultaneously.
std::vector<std::vector<std::pair<int, int>>> line_graph; // index: line, values: (line, station).
int connectivity_index = -1;

void build_scc_graph() {
  station_to_lines.assign(num_stations, std::vector<int>());
  line_graph.assign(num_lines, std::vector<std::pair<int, int>>());

  std::vector<std::set<int>> line_stations(num_lines); // index: line, values: stations on the line (set).

  for (int station = 0; station < num_stations; ++station) {
    std::set<int> station_lines;

    // Gather lines connected to the current station and update line_stations.
    for (std::vector<std::pair<int, int>>::iterator connection = graph[station].begin(); connection != graph[station].end(); ++connection) {
      station_lines.insert(connection->second);
      line_stations[connection->second].insert(station);
    }

    // If all stations are connected via the same line the number of changes is 0.
    if (station_lines.size() == (size_t) num_lines) {
      connectivity_index = 0;
      return;
    }

    station_to_lines[station].assign(station_lines.begin(), station_lines.end());

    // Build line connections.
    for (std::set<int>::iterator line1 = station_lines.begin(); line1 != station_lines.end(); ++line1) {
      std::set<int>::iterator line2 = line1;
      for (++line2; line2 != station_lines.end(); ++line2) {
        line_graph[*line1].emplace_back(*line2, station);
        line_graph[*line2].emplace_back(*line1, station);
      }
    }
  }
}

int bfs(int source_line) {
  std::queue<std::pair<int, int>> queue; // (line_id, changes)
  std::vector<int> num_line_changes(num_lines, INT_MAX); // Track minimum changes to reach each line. index: line_id, value: changes.

  // Initialize queue with 0 changes.
  queue.emplace(source_line, 0);
  num_line_changes[source_line] = 0; // To get to the starting line, no changes are needed.

  while (!queue.empty()) {
    int current_line = queue.front().first;
    int changes = queue.front().second;
    queue.pop();

    // Skip if a better path has been found.
    if (changes > num_line_changes[current_line]) continue;

    for (size_t i = 0; i < line_graph[current_line].size(); ++i) {
      int next_line = line_graph[current_line][i].first;
      // Increment the number of changes count when transitioning to a different line.
      int new_changes = (next_line != current_line) ? changes + 1 : changes;

      // If the new number of changes is less than the current number of changes,
      // update the number of changes and add the line to the queue.
      if (new_changes < num_line_changes[next_line]) {
        num_line_changes[next_line] = new_changes;
        queue.emplace(next_line, new_changes);
      }
    }
  }

  // Find the minumum number of line changes required to reach all other lines
  int min_required_changes = 0;
  for (int i = 0; i < num_lines; ++i) {
    if (num_line_changes[i] == INT_MAX) return -1;
    min_required_changes = std::max(min_required_changes, num_line_changes[i]);
  }

  return min_required_changes;
}

int main() {
  std::ios::sync_with_stdio(0); // Disable sync with printf/scanf
  std::cin.tie(0); // Discard cin buffer after each line of input

  std::cin >> num_stations >> num_connections >> num_lines;
  graph.resize(num_stations);
  for (int i = 0; i < num_connections; ++i) {
    int station1, station2, line;
    std::cin >> station1 >> station2 >> line;
    --station1; --station2; --line; // Convert to 0-based indexing
    graph[station1].emplace_back(station2, line); // Add connection from station 1 to station 2.
    graph[station2].emplace_back(station1, line); // Add reverse connection from station 2 to station 1.
  }

  // Check if any station is disconnected.
  for (int i = 0; i < num_stations; ++i) {
    if (graph[i].empty()) {
      std::cout << -1 << std::endl; // If so return -1 and exit early as the graph is invalid.
      return 0;
    }
  }

  build_scc_graph(); // Build the SCC graph.

  // Early return if station connects all lines.
  if (connectivity_index == 0) {
    std::cout << 0 << std::endl;
    return 0;
  }

  for (int line = 0; line < num_lines; ++line) {
    // Conducts a BFS for each line.
    int min_required_changes = bfs(line);
    if (min_required_changes == num_lines - 1) {
      std::cout << min_required_changes << std::endl;
      return 0;
    }
    connectivity_index = std::max(connectivity_index, min_required_changes);
  }

  std::cout << connectivity_index << std::endl;
  return 0;
}
