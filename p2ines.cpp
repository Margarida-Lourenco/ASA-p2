#include <vector>
#include <stdio.h>
#include <string>
using namespace std;

// Vertex struct
typedef struct {
    int id;
    bool visited;
    vector<int> edges;
} vertex;


int firstDFS(vector<vertex> &graph, int currentVertex, vector<int> &endTimeList){
    if (graph[currentVertex].visited)
        return 0;
    
    graph[currentVertex].visited = true;

    for (int i: graph[currentVertex].edges){
        firstDFS(graph, i, endTimeList);
    }

    endTimeList.push_back(currentVertex);   // list of vertices in order of decreasing end time
    return 0;
}


void transposeGraph(vector<vertex> &graph, vector<vertex> &transposedGraph){
    for (vertex v : graph){
        for (int i: v.edges){
            transposedGraph[i].edges.push_back(v.id);
        }
    }
}

int secondDFS(vector<vertex> &graph, int currentVertex, const vector<int> &endTimeList){
    // reset visited
    for (vertex &v : graph){
        v.visited = false;
    }

    for (int i : endTimeList){
        if (graph[i].visited)
            continue;
        
        graph[i].visited = true;
        for (int j : graph[endTimeList[i]].edges){
            secondDFS(graph, graph[j].id, endTimeList);
        }
    }
    return 0;
}
    
int main(){
    int n, m;
    vector<int> endTimeList;
    
    if (scanf("%d %d", &n, &m) != 2) {
        return 1;
    }

    if ((n < 2) || m < 0)
        return 1;

    vector<vertex>graph (n+1, {0, false, {}});

    while (m > 0) {
        int x, y;

        if (scanf("%d %d", &x, &y) != 2)
            return 1;

        if (x >= 1 && x <= n && y >= 1 && y <= n) {
            graph[x].id = x;
            graph[x].visited = false;
            graph[x].edges.push_back(y);

        } else {
            return 1;   // values out of range
        }
        m--;
    }

    for (vertex v : graph){
        firstDFS(graph, v.id, endTimeList);
    }

    vector<vertex> transposedGraph (n+1, {0, false, {}});
    transposeGraph(graph, transposedGraph);

    secondDFS(transposedGraph, 0, endTimeList);
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

    Resolução (Resumos LEIC-A):
    O algoritmo para chegar aos SCCs de um grafo é bastante simples:

    -> Fazer uma DFS normal, guardando uma lista com os vértices ordenada 
    de modo decrescente pelos respetivos tempos de fim.

    -> Transpor o grafo - alterar o sentido de todos os seus arcos.

    ->Fazer outra DFS (ao grafo transposto), seguindo desta vez a ordem 
    decrescente que guardámos no primeiro passo. A ordem decrescente é relevante 
    ao escolher a raiz do caminho, mas aquando da exploração do caminho em si, 
    não importa - podemos escolher qualquer vértice.
    */
