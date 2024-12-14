#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <random>
#include <algorithm>
using namespace std;

// Returns a random value between [1, m]
#define randomValue(m) ((rand() % m) + 1)

//-----------------------------------------------------------------------------
void printUsage(char *progname) {
  cerr << "Usage: " << progname << " <V> <E> <L> <b> <seed>" << endl;
  cerr << "  V: number of stations (vertices)" << endl;
  cerr << "  E: number of connections (edges)" << endl;
  cerr << "  L: number of lines (colors)" << endl;
  cerr << "  b: all stations be connected? (optional, 0/1) E >= V" << endl;
  cerr << "  seed: random seed generator (opcional)" << endl;
  exit(1);
}

//-----------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    int V, E, L;
    bool bConnect = false;
    int seed = 0;

    if (argc < 4 || argc > 6) {
      cerr << "ERROR: Wrong number of arguments" << endl;
      printUsage(argv[0]);
    }

    sscanf(argv[1], "%d", &V);
    sscanf(argv[2], "%d", &E);
    sscanf(argv[3], "%d", &L);
    if (argc > 4) {
      int arg;
      sscanf(argv[4], "%d", &arg);
      bConnect = (arg!=0);
    }
    if (argc > 5) {
      sscanf(argv[5], "%d", &seed);
      srand(seed);
    } else { // pseudo-random seed
      srand((unsigned int)time(NULL));
    }

    if (bConnect && E < V) {
      cerr << "ERROR: E < V and bConnect" << endl;
      printUsage(argv[0]);
    }
    cout << V << " " << E << " " << L << endl;

    //-------------------------------------------------------------------------
    mt19937 rng(seed);
    map<int, set<int>> lineStations;
    vector<int> vAllVs = vector<int>(V);
    for (int i = 0; i < V; ++i) vAllVs[i] = i+1;

    // Each line is contiguous
    int lineEdges = V / L; // min # edges for a line
    for (int line = 1; line <= L; ++line) {
        shuffle(vAllVs.begin(), vAllVs.end(), rng);
        for (int i = 0; i < lineEdges; i++) {
          lineStations[line].insert(vAllVs[i]);
        }
    }

    // force that all nodes are connected
    if (bConnect) {
      set<int> totalStations(vAllVs.begin(), vAllVs.end());
      for (int l = 1; l <= L; ++l) {
        for (int e : lineStations[l]) totalStations.erase(e);
      }
      while (!totalStations.empty()) {
        int e = *totalStations.begin();
        int l = randomValue(L);
        if (lineStations[l].find(e) != lineStations[l].end()) continue;
        totalStations.erase(e);
        lineStations[l].insert(e);
      }
    }

    // Some additional edges (if needed)
    int currE = 0;
    for (int l = 1; l <= L; ++l) { // |E| = |V| - 1
      currE += (lineStations[l].size() - 1);
    }
    while (currE < E) {
      int l = randomValue(L);
      int e = randomValue(V);
      if (lineStations[l].find(e) != lineStations[l].end()) continue;
      lineStations[l].insert(e);
      currE++;
    }

    // Print the edges
    for (int l = 1; l <= L; ++l) {
      // linearly shuffle stations of line l
      vector<int> stationList(lineStations[l].begin(), lineStations[l].end());
      shuffle(stationList.begin(), stationList.end(), rng);
      // print edge (e1, e2) -> l
      for (size_t i = 1; i < stationList.size(); ++i) {
        int e1 = stationList[i - 1];
        int e2 = stationList[i];
        cout << e1 << " " << e2 << " " << l << std::endl;
      }
    }

    return 0;
}
