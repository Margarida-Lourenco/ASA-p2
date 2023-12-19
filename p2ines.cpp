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
    while (!endTimeStack.empty()) {
        int vertex = endTimeStack.top(); // get the vertex with the highest end time
        endTimeStack.pop(); // remove it from the stack
        if (graph[vertex].visited)
            continue;

        if (graph[vertex].edges.empty()) {
            graph[vertex].visited = true;
            sccs[vertex] = scc;
            scc++;
            continue;
        }

        stack<int> stack;
        stack.push(vertex);
        
        while (!stack.empty()) {
            vertex = stack.top();

            if (graph[vertex].visited) {
                stack.pop();
                continue;
            }

            graph[vertex].visited = true;
            sccs[vertex] = scc;

            for (int neighbor : graph[vertex].edges) {
                if (!graph[neighbor].visited) {
                    stack.push(neighbor);
                }
            }
        }
        scc++;
    }
}
/**
 * 3rd DFS - finds the maximum number of jumps
 * @param graph - graph to be explored
 * @param sccs - vector of SCCs
*/

void thirdDFS(vector<vertex> &graph) {
    vector<int> distance(graph.size(), 0);

    for (int i = 1; i < (int) graph.size(); i++) {
        if (graph[i].edges.empty())
            continue;
        vector<bool> visited(graph.size(), false);
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
    }

    // Find the maximum distance
    for (int i = 1; i < (int) distance.size(); i++) {
        maxJumps = max(maxJumps, distance[i]);
    }
}

/**
 * Creates the SCC graph
 * @param sccs - vector of SCCs
 * @param graph - graph to be explored
 * @return SCC graph
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
                if (sccs[current] != sccs[neighbor] && sccGraph[sccs[current]].addedEdges.count(sccs[neighbor]) == 0) {
                    sccGraph[sccs[current]].edges.push_back(sccs[neighbor]);
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
            graph[x].edges.push_back(y);
        } else {
            return 1; // values out of range
        }
    }

    stack<int> endTimeStack;
    vector<int> sccs (graph.size(), 0);

    firstDFS(graph, endTimeStack);

    vector<vertex> transposedGraph(graph.size(), {false, 0, {}});
    for (int i = 1; i < (int) graph.size(); i++) {
        for (int j : graph[i].edges) {
            transposedGraph[j].edges.push_back(i);
        }
    }

    secondDFS(transposedGraph, sccs, endTimeStack);

    for (vertex &v : transposedGraph)
        v.edges.clear();

    transposedGraph.clear();

    vector<vertex> sccGraph = createSCCGraph(sccs, graph);

    for (vertex &v : graph)
        v.edges.clear();

    graph.clear();
    
    thirdDFS(sccGraph);
    printf("%d\n", maxJumps);

    return 0;
}


/*
    ===========================================================================
    1. Encontrar os SCCs de um grafo
        * SCC - Strongly Connected Component

        * Este algoritmo dá-nos os SCCs de um grafo, ou seja, os conjuntos de
         vértices que estão todos ligados entre si, mas não estão ligados a
         nenhum vértice fora do conjunto.
        
        * Nós queremos as ligações entre os SCCs, ou seja, os vértices que
         ligam SCCs diferentes. Para isso, basta-nos encontrar os vértices
         que têm arcos para vértices fora do seu SCC.
    ===========================================================================

    O algoritmo para chegar aos SCCs de um grafo:

    -> Fazer uma DFS normal, guardando uma lista com os vértices ordenada 
    de modo decrescente pelos respetivos tempos de fim.

    -> Transpor o grafo - alterar o sentido de todos os seus arcos.

    ->Fazer outra DFS (ao grafo transposto), seguindo desta vez a ordem 
    decrescente que guardámos no primeiro passo. A ordem decrescente é relevante 
    ao escolher a raiz do caminho, mas aquando da exploração do caminho em si, 
    não importa - podemos escolher qualquer vértice.
*/
