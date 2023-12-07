#include <vector>
#include <stdio.h>
#include <string>
#include <iostream>
#include <string>


// nao ligar muito para o codigo, so para testar o git mesmo

// Vertex struct
typedef struct {
    int id;
    bool visited;
    std::vector<int> edges;
} vertex;

int main(){
    int n, m;
    scanf("%d %d", &n, &m);

    if ((n < 2) || m < 0)
        return 1;

    while(true){
        std::string line;
        std::getline(std::cin, line);

        if (line.empty())
            break;

        // arranajr maneira de guardar as relacoes
    }
    
    return 0;
}