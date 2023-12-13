#include <vector>
#include <stdio.h>
#include <string>
#include <list>
#include <stack>
using namespace std;

// Vertex struct
typedef struct {
    int id;
    bool visited;
    vector<int> edges;
} vertex;

int result = 0;
int firstVertex = 0;

int firstDFS(vector<vertex> &graph, int currentVertex, list<int> &endTimeList){
    if(currentVertex == 0)
        return 0;

    stack<int> stack;
    stack.push(currentVertex);

    while(!stack.empty()){
        currentVertex = stack.top();

        if (graph[currentVertex].visited){
            stack.pop();
        }
        else{ 
            graph[currentVertex].visited = true;

            for (int i : graph[currentVertex].edges){
                if (!graph[i].visited){
                    stack.push(i);
                }
            }
           endTimeList.push_front(currentVertex); 
        }
    }
    return 0;
}

int secondDFS(vector<vertex> &graph, int currentVertex){
    // If we reach the first vertex again, we have a cycle
    if (graph[currentVertex].visited && currentVertex == firstVertex)
        return result++;
    else if (graph[currentVertex].visited)
        return 0;
        
    graph[currentVertex].visited = true;
    for (int j : graph[currentVertex].edges){
        secondDFS(graph, graph[j].id);
    }
    return 0;
}
    
int main(){
    int n, m;
    list<int> endTimeList;
    
    if (scanf("%d %d", &n, &m) != 2) {
        return 1;
    }

    if ((n < 2) || m < 0)
        return 1;

    vector<vertex>graph (n+1, {0, false, {}});
    vector<vertex>transposedGraph (n+1, {0, false, {}}); 

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
            return 1;   // values out of range
        }
    }

    for (int i = 1; i < (int) graph.size(); i++){
        firstDFS(graph, graph[i].id, endTimeList);
    }

    //for(int i : endTimeList){
    //    printf("%d ", i);
    //}
    //printf("\n");

    // Iterate through vertices in order of decreasing finishing times
    for (int i : endTimeList) {
        firstVertex = i;
        secondDFS(transposedGraph, i);
    }

    printf("%d\n", result);

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
