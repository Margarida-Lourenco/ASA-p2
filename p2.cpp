#include <vector>
#include <stdio.h>
#include <string>
#include <iostream>
#include <string>
#include <sstream>

// Vertex struct
typedef struct {
    int id;
    bool visited;
    std::vector<int> edges;
} vertex;

int main(){
    int n, m;
    std::string line;
    std::vector<vertex> graph;
    
    if (!std::getline(std::cin, line) || sscanf(line.c_str(), "%d %d", &n, &m) != 2) {
        std::cout << "Erro na leitura." << std::endl;
        return 1;
    }

    if ((n < 2) || m < 0)
        return 1;

    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        int v1, v2;

        if (!(iss >> v1 >> v2)) {  // error when reading, stop condition
            break;
        }

        if (v1 >= 1 && v1 <= n && v2 >= 1 && v2 <= n) {
            graph[v1 - 1].edges.push_back(v2);
            graph[v2 - 1].edges.push_back(v1);  // n sei se e suposto
        } else {
            return 1;   // values out of range
        }

    }
    
    return 0;
}