#include <iostream>
#include <vector>
#include <tuple>
#include <queue>
#include <algorithm>
#include <limits>

int n, m, num_l; // n: number of stations, m: number of connections, num_l: number of lines.
std::vector<std::vector<std::pair<int, int>>> graph; // Graph is a list of adjacency lists: station -> (neighbor, line).

int bfs(int start, int end) {
    std::queue<std::tuple<int, int, int>> queue; // (current_station, current_line, transfers)
    std::vector<std::vector<int>> min_transfers(n, std::vector<int>(num_l + 1, std::numeric_limits<int>::max())); // min_transfers[station][line]

    // Initialize BFS with the starting station and no line (-1)
    for (int line = 1; line <= num_l; ++line) {
        queue.emplace(start, line, 0);
        min_transfers[start][line] = 0;
    }

    int min_changes = std::numeric_limits<int>::max(); // Track the minimum transfers to the target station. Currently initialized to the max value an int can take.

    while (!queue.empty()) {
        int current_station, current_line, transfers;
        std::tie(current_station, current_line, transfers) = queue.front();
        queue.pop();

        // If we reach the destination, update the minimum changes
        if (current_station == end) {
            if (transfers == 0) return 0;
            min_changes = std::min(min_changes, transfers);
            continue; // Continue exploring to find potentially better solutions
        }

        // Explore neighbors
        for (const auto& next : graph[current_station]) {
            int next_station = next.first;
            int next_line = next.second;

            // Calculate new transfer count
            int new_transfers = (current_line == next_line) ? transfers : transfers + 1;

            // Only process if this path is better
            if (new_transfers < min_transfers[next_station][next_line]) {
                min_transfers[next_station][next_line] = new_transfers;
                queue.emplace(next_station, next_line, new_transfers);
            }
        }
    }
    return (min_changes == std::numeric_limits<int>::max()) ? -1 : min_changes; // Return -1 if no path is found
}

int main() {
    std::cin >> n >> m >> num_l; // n: number of stations, m: number of connections, num_l: number of lines.

    graph.resize(n);
    for (int i = 0; i < m; ++i) {
        int x, y, l;
        std::cin >> x >> y >> l; // Station x and station y are connected by line l.
        --x, --y; // Convert to 0-based indexing
        graph[x].emplace_back(y, l);
        graph[y].emplace_back(x, l);
    }

    int connectivity_index = 0;
    for (int x = 0; x < n; ++x) { // From each station
        for (int y = x + 1; y < n; ++y) { // To every other station
            // Find the minimum number of changes between two stations
            int min_changes = bfs(x, y);

            if (min_changes == -1) {
                std::cout << min_changes << std::endl;
                return 0;
            }
            // Update the maximum connectivity index
            connectivity_index = std::max(connectivity_index, min_changes);
        }
    }

    std::cout << connectivity_index << std::endl;
    return 0;
}
