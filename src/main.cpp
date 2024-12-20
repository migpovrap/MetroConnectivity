#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

int main() {
  std::ios::sync_with_stdio(0); // Disable sync with printf/scanf
  std::cin.tie(0); // Discard cin buffer after each line of input

  int num_stations, num_connections, num_lines;
  std::cin >> num_stations >> num_connections >> num_lines;
  std::vector<std::vector<int>> lines_to_station(num_lines); // index: line, values: stations.
  std::vector<std::vector<int>> station_to_lines(num_stations); // index station, values: lines.
  std::vector<bool> station_has_connection(num_stations, false); // index station, values: true if station has connection.

  for (int i = 0; i < num_connections; ++i) {
    int station1, station2, line;
    std::cin >> station1 >> station2 >> line;
    --station1; --station2; --line; // Convert to 0-based indexing
    station_has_connection[station1] = true;
    station_has_connection[station2] = true;
    lines_to_station[line].push_back(station1);
    lines_to_station[line].push_back(station2);
    station_to_lines[station1].push_back(line);
    station_to_lines[station2].push_back(line);
  }

  for (int station = 0; station < num_stations; ++station) {
    // If a station is not connected to any other station, the graph is disconnected and therefore invalid.
    if (!station_has_connection[station]) {
      std::cout << -1 << std::endl;
      return 0;
    }
    // Remove duplicates from station_to_lines[station]
    std::sort(station_to_lines[station].begin(), station_to_lines[station].end());
    station_to_lines[station].erase(std::unique(station_to_lines[station].begin(),
    station_to_lines[station].end()), station_to_lines[station].end());
    // If all stations are connected via the same line the number of changes is 0.
    if (station_to_lines[station].size() == (size_t) num_lines) {
      std::cout << 0 << std::endl;
      return 0;
    }
  }

  // Build the line graph.
  std::vector<std::vector<int>> line_graph(num_lines);
  for (int station = 0; station < num_stations; ++station) {
    for (size_t i = 0; i < station_to_lines[station].size(); ++i) {
      for (size_t j = i + 1; j < station_to_lines[station].size(); ++j) {
        int line = station_to_lines[station][i];
        int other_line = station_to_lines[station][j];
        line_graph[line].push_back(other_line);
        line_graph[other_line].push_back(line);
      }
    }
  }

  int connectivity_index = 0;
  for (int line = 0; line < num_lines; ++line) {
    // Conduct a BFS for each line.
    std::queue<int> queue;
    std::vector<int> changes(num_lines, -1); // Tracks changes to reach each line

    queue.push(line);
    changes[line] = 0;
    int min_required_changes = 0;

    while (!queue.empty()) {
      int current_line = queue.front();
      queue.pop();
      for (const int& next_line : line_graph[current_line]) {
        if (changes[next_line] == -1) {
          changes[next_line] = changes[current_line] + 1;
          queue.push(next_line);
          if (changes[next_line] > min_required_changes) {
            min_required_changes = changes[next_line];
          }
        }
      }
    }
    if (min_required_changes == num_lines - 1) {
      std::cout << min_required_changes << std::endl;
      return 0;
    }
    if (min_required_changes > connectivity_index)
      connectivity_index = min_required_changes;
  }

  std::cout << connectivity_index << std::endl;
  return 0;
}
