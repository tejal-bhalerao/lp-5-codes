// Design and implement Parallel Breadth First Search based on existing algorithms using
// OpenMP. Use a Tree or an undirected graph for BFS.

#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
#include <chrono>
using namespace std;

class Graph {
public:
    int V;
    vector<vector<int>> adj;
    vector<bool> visited;

    Graph(int v) : V(v) {
        adj.resize(V);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void resetVisited() {
        visited.assign(V, false);
    }

    void bfs(int start) {
        queue<int> q;
        q.push(start);
        visited[start] = true;

        while (!q.empty()) {
            int curr = q.front(); q.pop();
            cout << curr << " ";

            for (int neigh : adj[curr]) {
                if (!visited[neigh]) {
                    visited[neigh] = true;
                    q.push(neigh);
                }
            }
        }
    }

    void parallel_bfs(int start) {
        queue<int> q;
        q.push(start);
        visited[start] = true;

        while (!q.empty()) {
            int curr = q.front(); q.pop();
            cout << curr << " ";

            #pragma omp parallel for
            for (int i = 0; i < adj[curr].size(); i++) {
                int neigh = adj[curr][i];
                if (!visited[neigh]) {
                    #pragma omp critical
                    {
                        if (!visited[neigh]) {
                            visited[neigh] = true;
                            q.push(neigh);
                        }
                    }
                }
            }
        }
    }
};

int main() {
    int v, e;
    cout << "Vertices and Edges: ";
    cin >> v >> e;

    Graph g(v);
    cout << "Enter edges (u v):\n";
    for (int i = 0; i < e; i++) {
        int u, v;
        cin >> u >> v;
        g.addEdge(u, v);
    }

    g.resetVisited();
    cout << "Serial BFS: ";
    auto start = chrono::high_resolution_clock::now();
    g.bfs(0);
    auto end = chrono::high_resolution_clock::now();
    cout << "\nTime: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " microseconds\n";

    g.resetVisited();
    cout << "Parallel BFS: ";
    start = chrono::high_resolution_clock::now();
    g.parallel_bfs(0);
    end = chrono::high_resolution_clock::now();
    cout << "\nTime: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " microseconds\n";

    return 0;
}
