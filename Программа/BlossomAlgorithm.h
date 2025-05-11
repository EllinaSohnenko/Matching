#pragma once
#include <iostream>
#include <vector>
using namespace std;

// Альтернирующее дерево
struct altTree {
public:
    int peaks; // Вершина
    bool included; // Входит ли в альтернирующее дерево
    bool outer; // 0 - внутреняя; 1 - внешняя
    int parent; // Вершина "родитель"

    altTree() {}
    altTree(int peaks, bool include, bool outer, int parent) {
        this->peaks = peaks;
        this->included = include;
        this->outer = outer;
        this->parent = parent;
    }
};

bool exp(int xi);
void DeletePeaks(int peaks);
void Connect(int v1, int v2, int i);
void BlossomAlgorithm();
void BlossomAlgorithm2();
void BlossomAlgorithm3();
void BlossomAlgorithm4(int xi);
void BlossomAlgorithm5(int x0, int xi);
void BlossomAlgorithm6(int xi, int xk);
void BlossomAlgorithm8();