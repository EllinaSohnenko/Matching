#include "Backtracking.h"
#include "element.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>
#include <set>
using namespace std;

extern int n;
extern vector<graph> gr;
extern vector<vector<int>> com;

vector<graph> graphCopy1;
vector<graph> max1(0);
int maxCount = 0;
bool finish1 = 1;
vector<pair<int, int>> resh1(0);

// Экспонированная ли вершина
bool exp1(vector<graph> gr, int xi) {
    for (int j = 0; j < gr[xi - 1].adjacentVertices.size(); j++)
        if (gr[xi - 1].adjacentVertices[j].second) return 0;
    return 1;
}

void Backtracking() {
    finish1 = 1;
    maxCount = 0;
    max1.clear();
    resh1.clear();
    graphCopy1 = gr;
    for (int i = 0; i < com.size(); i++) {
        if (!(com[i].size() == 1)) {
            if (max1.size() != 0) graphCopy1 = max1;
            for (int j = 0; j < n; j++) {
                if (graphCopy1[j].component == i) graphCopy1[j].work = 1;
                else graphCopy1[j].work = 0;
            }
            int r = com[i][0];
            maxCount = 0;
            finish1 = 1;
            Backtracking1(graphCopy1, r, 0);
        }
    }
    if (max1.size() == 0) max1 = graphCopy1;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < max1[i].adjacentVertices.size(); j++)
            if (max1[i].adjacentVertices[j].second && max1[i].adjacentVertices[j].first > max1[i].peaks)
                resh1.push_back({ max1[i].peaks, max1[i].adjacentVertices[j].first });

    cout << "\n Паросочетание:\n";
    if (resh1.size() == 0) cout << "Пустое множество\n";
    for (int i = 0; i < resh1.size(); i++)
        cout << i + 1 << ". " << resh1[i].first << " " << resh1[i].second << endl;
}

void Backtracking1(vector<graph> graph, int index, int count) {
    for (int i = 0; i < graph[index - 1].adjacentVertices.size(); i++) 
        if (graph[index - 1].adjacentVertices[i].first > index && exp1(graph, graph[index - 1].adjacentVertices[i].first)) {
            // Отмечаем найденное паросочетание
            graph[index - 1].adjacentVertices[i].second = 1;
            int ind = 0;
            for (int j = 0; j < graph[graph[index - 1].adjacentVertices[i].first - 1].adjacentVertices.size() && !ind; j++)
                if (graph[graph[index - 1].adjacentVertices[i].first - 1].adjacentVertices[j].first == index) ind = j;
            graph[graph[index - 1].adjacentVertices[i].first - 1].adjacentVertices[ind].second = 1;

            for (int j = index + 1; j < n; j++)
                if (graph[j - 1].work && exp1(graph, j)) {
                    Backtracking1(graph, j, count + 1);
                    if (!finish1) return;
                }

            if (max1.size() == 0) max1 = graph;
            if (count + 1 > maxCount) {
                maxCount = count;
                max1 = graph;
                int k = 0;
                for (int j = 0; j < n; j++)
                    if (graph[j].work && exp1(graph, j + 1)) k++;
                if (k < 2) {
                    finish1 = 0;
                    return;
                }
            }
            graph[index - 1].adjacentVertices[i].second = 0;
            graph[graph[index - 1].adjacentVertices[i].first - 1].adjacentVertices[ind].second = 0;
        }
}