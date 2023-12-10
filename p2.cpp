#include <vector>
#include <stdio.h>
#include <iostream>
#include <string>


// Vertex struct
typedef struct {
    int id;
    bool visited;
    vector<int> edges;
} Vertex;


int firstDFS(std::vector<vertex> &graph, int currentVertex, std::vector<int> &endTimeList){
    if (graph[currentVertex].visited)
        return 0;
    
    graph[currentVertex].visited = true;

    for (int i = 0; i < (int) graph[currentVertex].edges.size(); i++){
        firstDFS(graph, graph[currentVertex].edges[i], endTimeList);
    }

    endTimeList.push_back(currentVertex);   // list of vertices in order of decreasing end time
    return 0;
}


void transposeGraph(std::vector<vertex> &graph, std::vector<vertex> &transposedGraph){
    for (int i = 1; i < (int) graph.size(); i++){
        for (int j = 0; j < (int) graph[i].edges.size(); j++){
            transposedGraph[graph[i].edges[j]].edges.push_back(i);
        }
    }
}

int secondDFS(std::vector<vertex> &graph, int currentVertex, const std::vector<int> &endTimeList){
    // reset visited
    for (auto &v : graph){
        v.visited = false;
    }

    for (int i = 0; i < (int) endTimeList.size(); i++){
        if (graph[endTimeList[i]].visited)
            continue;
        
        graph[endTimeList[i]].visited = true;
        for (int j = 0; j < (int) graph[endTimeList[i]].edges.size(); j++){
            secondDFS(graph, graph[endTimeList[i]].edges[j], endTimeList);
        }
    }
    return 0;
}
    
int main(){
    int n, m;
    std::string line;
    std::vector<int> endTimeList;
    
    if (!std::getline(std::cin, line) || sscanf(line.c_str(), "%d %d", &n, &m) != 2) {
        return 1;
    }

    if ((n < 2) || m < 0)
        return 1;

    std::vector<vertex>graph (n+1, {0, false, {}});

    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        int x, y;

        if (!(iss >> x >> y)) {  // error when reading, stop condition
            break;
        }

        if (x >= 1 && x <= n && y >= 1 && y <= n) {
            graph[x].id = x;
            graph[x].visited = false;
            graph[x].edges.push_back(y);
        } else {
            return 1;   // values out of range
        }
    }

    for (int i = 1; i <= n; i++){
        firstDFS(graph, i, endTimeList);
    }

    std::vector<vertex> transposedGraph (n+1, {0, false, {}});
    transposeGraph(graph, transposedGraph);

    for(int i = 0; i < (int) endTimeList.size(); i++){
        secondDFS(transposedGraph, 1, endTimeList);
    }

    for (int i = 0; i < m; i++) {
        int x, y;
        if (scanf("%d %d", &x, &y) != 2) {
            printf("Error reading relation\n");
            return 1;
        }
        Vertex newVertex = {x, false};
        newVertex.edges.push_back(y);
        graph.push_back(newVertex);
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
    
    return 0;
}