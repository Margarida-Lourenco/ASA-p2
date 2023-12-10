#include <vector>
#include <stdio.h>
#include <string>
#include <iostream>
#include <string>
using namespace std;


// Vertex struct
typedef struct {
    int id;
    bool visited;
    vector<int> edges;
} Vertex;

int main(){
    int n, m, time = 0;
    vector<Vertex> graph;
    
    if (scanf("%d %d", &n, &m) != 2) {
        printf("Error reading number of individuos and relations\n");
        return 1;
    }

    if ((n < 2) || m < 0)
        return 1;

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