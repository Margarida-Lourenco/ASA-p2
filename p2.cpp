#include <iostream>
#include <vector>
#include <stack>

using namespace std;
#define max(a, b) (a > b ? a : b)

typedef struct {
    bool visited;
    bool searchtime;
    vector<int> edges;
} vertex;

int maxJumps = 0;

/**
 * 1st DFS - finds the vertices ordered by decreasing end time
 * @param graph - graph to be explored
 * @param currentVertex - current vertex
 * @param endTimeStack - Empty stack to be filled with the vertices ordered by decreasing end time
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
                // If the vertex has already been added to the endTimeStack, pop it without pushing again
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
 * @param currentVertex - current vertex
 * @param scc - Empty set to be filled with the vertices of the SCC
 * @return 1 if the SCC was found, 0 otherwise
*/
void secondDFS(vector<vertex> &graph, vector<int> &sccs, stack<int> &endTimeStack) {
    int scc = 1; 
    vector<int> distance(graph.size(), 0);   
    while (!endTimeStack.empty()) {
        int vertex = endTimeStack.top(); // get the vertex with the highest end time
        endTimeStack.pop(); // remove it from the stack

        if(graph[vertex].visited)
            continue;

        stack<int> stack;
        stack.push(vertex);

        if (graph[vertex].edges.empty()) {
            graph[vertex].visited = true;
            sccs[vertex] = scc;
            scc++;
            continue;
        }
        
        while (!stack.empty()) {
            vertex = stack.top();

            if (graph[vertex].visited) {
                stack.pop();
                continue;
            }

            graph[vertex].visited = true;
            sccs[vertex] = scc;

            for (int neighbor : graph[vertex].edges) {
                int v = sccs[neighbor], u = sccs[vertex];
                if (graph[neighbor].visited && v != u)
                    distance[u] = max(distance[u], distance[v] + 1);

                else if(graph[neighbor].visited && v == u)
                    distance[v] = max(distance[v], distance[u]);
                    
                else if (!graph[neighbor].visited) {
                    distance[v] = max(distance[v], distance[u]);
                    stack.push(neighbor);
                }
            }
        }
        scc++;
    }
    for (int i = 1; i < (int) distance.size(); i++) {
        maxJumps = max(maxJumps, distance[i]);
    }
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
            graph[x].edges.push_back(y);
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

    printf("%d\n", maxJumps);

    return 0;
}

