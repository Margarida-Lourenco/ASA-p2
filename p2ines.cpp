#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <set>

using namespace std;

typedef struct {
    int id;
    bool visited;
    vector<int> edges;
} vertex;

int maxJumps = 0;

/*
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

        // if the vertex has already been visited, it means that all its edges have been explored
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

/*
 * 2nd DFS - finds the SCCs
 * @param graph - graph to be explored
 * @param currentVertex - current vertex
 * @param scc - Empty vector to be filled with the vertices of the SCC
*/
void secondDFS(vector<vertex> &graph, int currentVertex, vector<int> &scc) {
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
        scc.push_back(currentVertex); // add vertex to the SCC

        for (int i : graph[currentVertex].edges) {
            if (!graph[i].visited) {
                stack.push(i);
            }
        }
    }
}

/*
 * Verifies if the SCC has connections to other SCCs and increments the number of jumps if it does
 * @param scc - SCC to be verified
 * @param graph - graph to be explored
*/
void verifyConnections(vector<int> &scc, vector<vertex> &graph) {
    set<int> sccSet(scc.begin(), scc.end()); // set of the SCC vertices

    for (int vertex : scc) {
        for (int neighbor : graph[vertex].edges) {
            if (!sccSet.count(neighbor)) { // if the neighbor is not in the SCC
                maxJumps++;             // increment the number of jumps
                return;
            }
        }
    }
}

/**
 * Calculates the number of jumps needed to go through all the vertices
 * @param graph - graph to be explored
 * @param transposedGraph - transposed graph to be explored
*/
void Calculator(vector<vertex> &graph, vector<vertex> &transposedGraph) {
    stack<int> endTimeStack; 

    // first DFS
    for (int i = 1; i < (int) graph.size(); i++) {
        if (!graph[i].visited) {
            firstDFS(graph, i, endTimeStack);
        }
    }

    // reset visited
    for (int i = 1; i < (int) graph.size(); i++) {
        graph[i].visited = false;
    }

    // second DFS
    while (!endTimeStack.empty()) {
        int vertex = endTimeStack.top(); // get the vertex with the highest end time
        endTimeStack.pop();     // remove it from the stack

        if (!graph[vertex].visited) { // if it hasn't been visited yet
            vector<int> scc;
            secondDFS(transposedGraph, vertex, scc);
            verifyConnections(scc, graph);
        }
    }

}

int main() {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2)
        return 1;

    if (n < 2 || m < 0)
        return 1;

    vector<vertex> graph(n + 1, {0, false, {}});
    vector<vertex> transposedGraph(n + 1, {0, false, {}});

    for (int i = 0; i < m; i++) {
        int x, y;
        if (scanf("%d %d", &x, &y) != 2)
            return 1;

        if (x >= 1 && x <= n && y >= 1 && y <= n) {
            graph[x].id = x;
            graph[x].visited = false;
            graph[x].edges.push_back(y);

            transposedGraph[y].id = y;
            transposedGraph[y].visited = false;
            transposedGraph[y].edges.push_back(x);
        } else {
            return 1; // values out of range
        }
    }

    Calculator(graph, transposedGraph);
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
