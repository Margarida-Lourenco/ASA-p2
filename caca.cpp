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
 * First DFS - finds the vertices ordered by decreasing end time
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
 * Second DFS - finds the SCCs and calculates the maximum distance
 * @param graph - graph to be explored
 * @param transposedGraph - transposed graph to be explored
 */
void secondDFS(vector<vertex> &graph, vector<vertex> &transposedGraph) {
    stack<int> endTimeStack;

    // Perform the first DFS to get vertices ordered by decreasing end time
    for (int i = 1; i < (int)graph.size(); i++) {
        firstDFS(graph, i, endTimeStack);
    }

    // Reset visited flags
    for (int i = 1; i < (int)graph.size(); i++) {
        graph[i].visited = false;
    }

    vector<int> distances(graph.size(), 0);

    // Perform the second DFS to find SCCs and calculate distances
    while (!endTimeStack.empty()) {
        int vertex = endTimeStack.top();
        endTimeStack.pop();

        stack<int> stack;
        stack.push(vertex);
        graph[vertex].visited = true;

        while (!stack.empty()) {
            int current = stack.top();
            stack.pop();

            for (int neighbor : transposedGraph[current].edges) {
                if (!graph[neighbor].visited) {
                    graph[neighbor].visited = true;
                    distances[neighbor] = max(distances[neighbor], distances[current] + 1);
                    stack.push(neighbor);
                } 
            }
        }
        // Update maxJumps during the second DFS itself
        for (int i = 1; i < (int)distances.size(); i++) {
            maxJumps = max(maxJumps, distances[i]);
        }
    }
}

int main() {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2)
        return 1;

    if (n < 2 || m < 0)
        return 1;

    vector<vertex> graph(n + 1, {false, {}});
    vector<vertex> transposedGraph(n + 1, {false, {}});

    for (int i = 0; i < m; i++) {
        int x, y;
        if (scanf("%d %d", &x, &y) != 2)
            return 1;

        if (x >= 1 && x <= n && y >= 1 && y <= n) {
            graph[x].edges.push_back(y);
            transposedGraph[y].edges.push_back(x);
        } else {
            return 1; // values out of range
        }
    }

    secondDFS(graph, transposedGraph);
    printf("%d\n", maxJumps);

    return 0;
}