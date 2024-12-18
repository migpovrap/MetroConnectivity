#include <algorithm>
#include <climits>
#include <iostream>
#include <queue>
#include <set>
#include <vector>

int num_stations, num_connections, num_lines;
// Adjacency list representing the graph.
std::vector<std::vector<int>> graph; // index: line, value: station.
std::vector<std::vector<int>> line_graph; // index: line, values: connected lines.
int connectivity_index = -1;

void build_line_graph() {
  line_graph.assign(num_lines, std::vector<int>());

  // Create a map to store which lines pass through each station.
  std::vector<std::set<int>> station_to_lines(num_stations);

  for (int line = 0; line < num_lines; ++line) {
    for (int station : graph[line]) {
      station_to_lines[station].insert(line);
    }
  }

  // Build the line-to-line graph.
  for (int station = 0; station < num_stations; ++station) {
    const std::set<int>& lines = station_to_lines[station];

    // If all stations are connected via the same line the number of changes is 0.
    if (lines.size() == (size_t) num_lines) {
      connectivity_index = 0;
      return;
    }

    for (std::set<int>::iterator line1 = lines.begin(); line1 != lines.end(); ++line1) {
      for (std::set<int>::iterator line2 = std::next(line1); line2 != lines.end(); ++line2) {
        line_graph[*line1].push_back(*line2);
        line_graph[*line2].push_back(*line1);
      }
    }
  }
}

int bfs(int source_line) {
  std::queue<std::pair<int, int>> queue; // pair<line_id, changes>
  std::vector<bool> visited(num_lines, false); // Tracks visited lines.

  queue.push(std::make_pair(source_line, 0));
  visited[source_line] = true;

  int changes_furthest_line = 0;

  while (!queue.empty()) {
    std::pair<int, int> front = queue.front();
    int current_line = front.first;
    int changes = front.second;
    queue.pop();

    for (const int& next_line : line_graph[current_line]) {
      if (!visited[next_line]) {
        visited[next_line] = true;
        queue.emplace(next_line, changes + 1);
        changes_furthest_line = std::max(changes_furthest_line, changes + 1);
      }
    }
  }
  return changes_furthest_line;
}

int main() {
  std::ios::sync_with_stdio(0); // Disable sync with printf/scanf
  std::cin.tie(0); // Discard cin buffer after each line of input

  std::cin >> num_stations >> num_connections >> num_lines;
  graph.resize(num_lines);
  std::vector<bool> station_has_connection(num_stations, false);

  for (int i = 0; i < num_connections; ++i) {
    int station1, station2, line;
    std::cin >> station1 >> station2 >> line;
    --station1; --station2; --line; // Convert to 0-based indexing
    if (!station_has_connection[station1]) {
      station_has_connection[station1] = true;
    }
    if (!station_has_connection[station2]) {
      station_has_connection[station2] = true;
    }
    graph[line].push_back(station1); // Add station1 to the line.
    graph[line].push_back(station2); // Add station2 to the line.
  }

  for (int station = 0; station < num_stations; ++station) {
    if (!station_has_connection[station]) {
      std::cout << -1 << std::endl;
      return 0;
    }
  }

  build_line_graph(); // Build the SCC graph.

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
