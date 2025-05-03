// Design and implement Parallel DEPTH First Search based on existing algorithms using
// OpenMP. Use a Tree or an undirected graph for BFS.



#include <iostream>
#include <vector>
#include <stack>
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

    void dfs(int start) {
        stack<int> s;
        s.push(start);
        visited[start] = true;

        while (!s.empty()) {
            int curr = s.top(); s.pop();
            cout << curr << " ";

            for (int neigh : adj[curr]) {
                if (!visited[neigh]) {
                    visited[neigh] = true;
                    s.push(neigh);
                }
            }
        }
    }

    void parallel_dfs(int start) {
        stack<int> s;
        s.push(start);
        visited[start] = true;

        while (!s.empty()) {
            int curr = s.top(); s.pop();
            cout << curr << " ";

            #pragma omp parallel for
            for (int i = 0; i < adj[curr].size(); i++) {
                int neigh = adj[curr][i];
                if (!visited[neigh]) {
                    #pragma omp critical
                    {
                        if (!visited[neigh]) {
                            visited[neigh] = true;
                            s.push(neigh);
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
    cout << "Serial DFS: ";
    auto start = chrono::high_resolution_clock::now();
    g.dfs(0);
    auto end = chrono::high_resolution_clock::now();
    cout << "\nTime: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " microseconds\n";

    g.resetVisited();
    cout << "Parallel DFS: ";
    start = chrono::high_resolution_clock::now();
    g.parallel_dfs(0);
    end = chrono::high_resolution_clock::now();
    cout << "\nTime: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " microseconds\n";

    return 0;
}
