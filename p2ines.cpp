#include <iostream>
#include <vector>
#include <stack>
#include <set>

using namespace std;
#define max(a, b) (a > b ? a : b)

typedef struct {
    bool visited;
    bool searchtime;
    set<int> addedEdges;
    vector<int> edges;
} vertex;

/**
 * 1st DFS - finds the end time of each vertex
 * @param graph - graph to be explored
 * @param endTimeStack - stack to be filled with the vertices in decreasing order of end time
*/
void firstDFS(vector<vertex> &graph, stack<int> &endTimeStack) {
    for (int start = 1; start < (int) graph.size(); start++) {
        if (graph[start].visited)
            continue;
        
        stack<int> s;
        s.push(start);

        while (!s.empty()) {
            int current = s.top();

            if (!graph[current].visited) {
                graph[current].visited = true;

                for (int neighbor : graph[current].edges) {
                    if (!graph[neighbor].visited) {
                        s.push(neighbor);
                    }
                }
            } else {
                // If the vertex has already been visited, it means that all its neighbors have been visited
                s.pop();

                if (graph[current].searchtime == false) {
                    graph[current].searchtime = true;
                    endTimeStack.push(current);
                }
            }
        }
    }
}

/**
 * 2nd DFS - finds the SCCs
 * @param graph - graph to be explored
 * @param sccs - vector of SCCs
 * @param endTimeStack - stack with the vertices in decreasing order of end time
*/
void secondDFS(vector<vertex> &graph, vector<int> &sccs, stack<int> &endTimeStack) {
    int scc = 1;    
    while (!endTimeStack.empty()) {
        int start = endTimeStack.top(); // get the vertex with the highest end time
        endTimeStack.pop(); // remove it from the stack

        if (graph[start].edges.empty()) { // if the vertex has no edges, it is a SCC
            graph[start].visited = true;
            sccs[start] = scc;
            scc++;
            continue;
        }

        stack<int> stack;
        stack.push(start);
        
        while (!stack.empty()) {
            start = stack.top();

            if (graph[start].visited) {
                stack.pop();
                continue;
            }

            graph[start].visited = true;
            sccs[start] = scc;

            for (int neighbor : graph[start].edges) {
                if (!graph[neighbor].visited) {
                    stack.push(neighbor);
                }
            }
        }
        scc++;
    }
}

/**
 * 3rd DFS - finds the maximum distance from each vertex to a vertex with no outgoing edges
 * @param graph - graph to be explored
*/
int thirdDFS(vector<vertex> &graph) {
    vector<int> distance(graph.size(), 0);
    vector<bool> visited(graph.size(), false);

    for (int i = 1; i < (int) graph.size(); i++) {
        if (graph[i].edges.empty()) 
            continue;
        stack<int> stack;

        stack.push(i);
        visited[i] = true;

        while (!stack.empty()) {
            int current = stack.top();
            stack.pop();

            for (int neighbor : graph[current].edges) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    stack.push(neighbor);
                    distance[neighbor] = max (distance[neighbor], distance[current] + 1);
                }
            }
        }
        fill(visited.begin(), visited.end(), false);
    }

    int maxJumps = 0;
    // Find the maximum distance
    for (int i = 1; i < (int) distance.size(); i++) {
        maxJumps = max(maxJumps, distance[i]);
    }
    return maxJumps;
}

/**
 * Creates a graph with the SCCs as vertices
 * @param sccs - vector of SCCs
 * @param graph - graph to be explored
 * @return a graph with the SCCs as vertices
*/
vector<vertex> createSCCGraph(const vector<int> &sccs, const vector<vertex> &graph) {
    vector<vertex> sccGraph(graph.size(), {false, 0, {}});

    for (int i = 1; i < (int) sccs.size(); i++) {
        if (graph[i].edges.empty())
            continue;
        stack<int> stack;
        stack.push(i);

        while (!stack.empty()) {
            int current = stack.top();
            stack.pop();

            for (int neighbor : graph[current].edges) {

                // If the neighbor is not in the same SCC and the edge has not been added yet
                if (sccs[current] != sccs[neighbor] && sccGraph[sccs[current]].addedEdges.count(sccs[neighbor]) == 0) {
                    sccGraph[sccs[current]].edges.emplace_back(sccs[neighbor]);
                    sccGraph[sccs[current]].addedEdges.insert(sccs[neighbor]);
                }
            }
        }
    }

    return sccGraph;
}

int main() {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2)
        return 1;

    vector<vertex> graph(n + 1, {false, false, {}});

    for (int i = 0; i < m; i++) {
        int x, y;
        if (scanf("%d %d", &x, &y) != 2)
            return 1;

        if (x >= 1 && x <= n && y >= 1 && y <= n) {
            graph[x].edges.emplace_back(y);
        } else {
            return 1; // values out of range
        }
    }

    stack<int> endTimeStack;
    vector<int> sccs (graph.size(), 0);

    firstDFS(graph, endTimeStack);

    vector<vertex> transposedGraph(graph.size(), {false, {}});
    for (int i = 1; i < (int) graph.size(); i++) {
        for (int j : graph[i].edges) {
            transposedGraph[j].edges.push_back(i);
        }
    }

    secondDFS(transposedGraph, sccs, endTimeStack);

    for (auto &v : transposedGraph)
    v.edges.clear();

    transposedGraph.clear();

    vector<vertex> sccGraph = createSCCGraph(sccs, graph);

    for (auto &v : graph)
        v.edges.clear();

    graph.clear();
    
    int maxJumps = thirdDFS(sccGraph);
    printf("%d\n", maxJumps);

    return 0;
}

