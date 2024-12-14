#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <tuple>

int main() {
  int n, m, l;
  std::cin >> n >> m >> l;

  std::vector<std::vector<std::pair<int, int>>> graph(n + 1);
  for (int i = 0; i < m; ++i) {
    int x, y, line;
    std::cin >> x >> y >> line;
    graph[x].emplace_back(y, line);
    graph[y].emplace_back(x, line);
  }

  int maxChanges = 0;
  for (int i = 1; i <= n; ++i) {
    for (int j = i + 1; j <= n; ++j) {
      std::queue<std::tuple<int, int, int>> q; // {current station, current line, changes}
      std::unordered_map<int, std::unordered_set<int>> visited; // station -> set of lines
      q.push(std::make_tuple(i, -1, 0));
      visited[i].insert(-1);
      bool found = false;
      int minChanges = -1;

      while (!q.empty() && !found) {
        int current, currentLine, changes;
        std::tie(current, currentLine, changes) = q.front();
        q.pop();
        if (current == j) {
          found = true;
          minChanges = changes;
          break;
        }

        for (const auto& edge : graph[current]) {
          if (visited[edge.first].count(edge.second) == 0) {
            visited[edge.first].insert(edge.second);
            q.push(std::make_tuple(edge.first, edge.second, changes + (currentLine != edge.second)));
          }
        }
      }

      if (!found) {
        std::cout << -1 << std::endl;
        return 0;
      }
      maxChanges = std::max(maxChanges, minChanges);
    }
  }

  std::cout << maxChanges << std::endl;
  return 0;
}
