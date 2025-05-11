#pragma once
#include <iostream>
#include <vector>
using namespace std;

// ������� �����
struct graph {
public:
    int peaks; // �������
    vector<pair<int, bool>> adjacentVertices; // ������� ������� � ���� �� ��� ������������
    int component; // ����� ���������� ���������
    bool work;

    graph() {}
    graph(int peaks, vector<pair<int, bool>> adj, int component) {
        this->peaks = peaks;
        this->adjacentVertices = adj;
        this->component = component;
        this->work = 1;
    }
};