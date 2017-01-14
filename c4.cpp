#include <iostream>
#include <vector>
#include <cassert>
using namespace std;

typedef vector<vector<int>> graph;
typedef unsigned uint;

bool has_c4_slow(const graph& g) {
  uint n = g.size();
  for (uint i = 0; i < n; ++i) {
    for (uint j : g[i]) {
      for (uint k : g[j]) if (k != i) {
        for (uint l : g[k]) if (l != j && l != i) {
          for (uint m : g[l]) {
            if (m == i) return true;
          }
        }
      }
    }
  }
  return false;
}

bool has_c4_fast(const graph& g) {
  uint n = g.size();
  vector<vector<uint>> arr(n);
  for (uint i = 0; i < n; ++i) {
    arr[i].resize(n);
  }
  for (uint i = 0; i < n; ++i) {
    for (uint j : g[i]) {
      for (uint k : g[i]) if (j < k) {
        if (++arr[j][k] == 2) {
          return true;
        }
      }
    }
  }
  return false;
}

void forall_graphs(uint n, uint i, uint j, graph& g, void (*f)(const graph&)) {
  if (i == n) {
    f(g);
    return;
  }
  if (j == n) {
    forall_graphs(n, i+1, i+2, g, f);
  } else {
    forall_graphs(n, i, j+1, g, f);
    g[i].push_back(j);
    g[j].push_back(i);
    forall_graphs(n, i, j+1, g, f);
    g[i].pop_back();
    g[j].pop_back();
  }
}

ostream& operator <<(ostream& os, const graph& g) {
  uint n = g.size();
  for (uint i = 0; i < n; ++i) {
    os << i << ": ";
    for (uint j : g[i]) {
      os << j << " ";
    }
    os << "\n";
  }
  os << "\n";
  return os;
}

void doit(const graph& g) {
  bool c4_slow = has_c4_slow(g);
  bool c4_fast = has_c4_fast(g);
  assert(!(c4_slow ^ c4_fast));
  if (c4_slow) {
    cout << g;
  }
}

int main() {
  // Generates all graphs of 7 vertices and verifies that the slow and fast
  // algorithms for detecting C4 agree on them.
  // Outputs all graphs that contain C4.
  uint n = 7;
  graph g(n);
  forall_graphs(n, 0, 1, g, doit);
  return 0;
}
