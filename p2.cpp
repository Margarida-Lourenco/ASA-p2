#include <iostream>
#include <vector>
#include <stack>
#include <set>

using namespace std;
#define max(a, b) (a > b ? a : b)

typedef struct {
    bool visited;
    vector<int> edges;
} vertex;

int maxJumps = 0;

/**
 * 1st DFS - finds the vertices ordered by decreasing end time
 * @param graph - graph to be explored
 * @param currentVertex - current vertex
 * @param endTimeStack - Empty stack to be filled with the vertices ordered by decreasing end time
*/
void firstDFS(vector<vertex> &graph, int currentVertex, stack<int> &endTimeStack) {
    if (graph[currentVertex].visited)
        return;
    stack<int> stack;
    stack.push(currentVertex);

    while (!stack.empty()) {
        currentVertex = stack.top();
        if (graph[currentVertex].visited) {
            stack.pop();
            continue;
        }
        graph[currentVertex].visited = true;
        for (int i : graph[currentVertex].edges) {
            if (!graph[i].visited) {
                stack.push(i);
            }
        }
        endTimeStack.push(currentVertex);
    }
}

/**
 * 2nd DFS - finds the SCCs
 * @param graph - graph to be explored
 * @param currentVertex - current vertex
 * @param scc - Empty set to be filled with the vertices of the SCC
 * @return 1 if the SCC was found, 0 otherwise
*/
int secondDFS(vector<vertex> &graph, int currentVertex, set<int> &scc) {
    stack<int> stack;
    stack.push(currentVertex);

    while (!stack.empty()) {
        currentVertex = stack.top();

        if (graph[currentVertex].visited || graph[currentVertex].edges.empty()) {
            stack.pop();
            continue;
        }

        graph[currentVertex].visited = true;
        scc.insert(currentVertex);   // add vertex to the SCC

        for (int i : graph[currentVertex].edges) {
            if (scc.count(i)) {
                return 1; // Stop it found a strongly connected component
            }
            if (!graph[i].visited) {
                stack.push(i);
            }
        }
    }
    return 0;
}
/**
 * 3rd DFS - finds the maximum number of jumps
 * @param graph - graph to be explored
 * @param sccs - vector of SCCs
*/

void thirdDFS(vector<vertex> &graph, vector<set<int>> sccs) {
    vector<int> distances(graph.size(), 0);

    for (int i = 1; i < (int) graph.size(); i++) {
        vector<bool> visited(graph.size(), false);
        stack<int> stack;

        stack.push(i);
        visited[i] = true;

        while (!stack.empty()) {
            int current = stack.top();
            stack.pop();

            for (int neighbor : graph[current].edges) {
                bool isSCC = false;
                
                // Check if the current vertex and the neighbor are in the same SCC
                for (set<int> scc : sccs) {
                    if (scc.count(current) && scc.count(neighbor)) {
                        isSCC = true;
                        break;
                    }
                }

                // If they are in the same SCC, the distance is the same
                if (isSCC && !visited[neighbor]) {
                    visited[neighbor] = true;
                    distances[neighbor] = max(distances[neighbor], distances[current]);
                    stack.push(neighbor);
                }

                // If they are not in the same SCC, the distance is plus one
                else if (!isSCC && !visited[neighbor]) {
                    visited[neighbor] = true;
                    distances[neighbor] = max(distances[neighbor], distances[current] + 1);
                    stack.push(neighbor);
                }
            }
        }
    }

    // Find the maximum distance
    for (int i = 1; i < (int) distances.size(); i++) {
        maxJumps = max(maxJumps, distances[i]);
    }
}

/**
 * Calculates the maximum number of jumps
 * @param graph - graph to be explored
 * @param transposedGraph - transposed graph to be explored
*/
void Calculator(vector<vertex> &graph) {
    stack<int> endTimeStack;
    vector<set<int>> sccs;

    for (int i = 1; i < (int) graph.size(); i++) {
        firstDFS(graph, i, endTimeStack);
    }

    vector<vertex> transposedGraph(graph.size(), {false, {}});
    for (int i = 1; i < (int) graph.size(); i++) {
        for (int j : graph[i].edges) {
            transposedGraph[j].edges.push_back(i);
        }
    }

    while (!endTimeStack.empty()) {
        int vertex = endTimeStack.top(); // get the vertex with the highest end time
        endTimeStack.pop(); // remove it from the stack

        set<int> scc;
        if (secondDFS(transposedGraph, vertex, scc) == 1) { // if it found a SCC
            sccs.push_back(scc);
        }
    }

    thirdDFS(graph, sccs);
}

int main() {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2)
        return 1;

    if (n < 2 || m < 0)
        return 1;

    vector<vertex> graph(n + 1, {false, {}});

    for (int i = 0; i < m; i++) {
        int x, y;
        if (scanf("%d %d", &x, &y) != 2)
            return 1;

        if (x >= 1 && x <= n && y >= 1 && y <= n) {
            graph[x].edges.push_back(y);
        } else {
            return 1; // values out of range
        }
    }

    Calculator(graph);
    printf("%d\n", maxJumps);

    return 0;
}