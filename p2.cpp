#include <vector>
#include <stdio.h>
#include <string>

// Vertex struct
typedef struct {
    int id;
    bool visited;
    std::vector<int> edges;
} vertex;


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
    std::vector<int> endTimeList;
    
    if (scanf("%d %d", &n, &m) != 2) {
        return 1;
    }

    if ((n < 2) || m < 0)
        return 1;

    std::vector<vertex>graph (n+1, {0, false, {}});

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

    for (int i = 1; i <= n; i++){
        firstDFS(graph, i, endTimeList);
    }

    std::vector<vertex> transposedGraph (n+1, {0, false, {}});
    transposeGraph(graph, transposedGraph);

    for(int i = 0; i < (int) endTimeList.size(); i++){
        secondDFS(transposedGraph, i, endTimeList);
    }
    return 0;
}