#pragma once
#include <iostream>
#include <vector>
using namespace std;

// Вершина графа
struct graph {
public:
    int peaks; // Вершина
    vector<pair<int, bool>> adjacentVertices; // Смежные вершины и были ли они рассмотренны
    int component; // Номер компоненты связности
    bool work;

    graph() {}
    graph(int peaks, vector<pair<int, bool>> adj, int component) {
        this->peaks = peaks;
        this->adjacentVertices = adj;
        this->component = component;
        this->work = 1;
    }
};