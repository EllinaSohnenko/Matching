#include "BlossomAlgorithm.h"
#include "element.h"
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

extern int n;
extern vector<graph> gr;
extern vector<vector<int>> com;

vector<altTree> altT(0);
vector<graph> graphCopy(0);
int exposedTop = 0; // корень
bool finish = 0, f = 1;
vector<pair<vector<int>, int>> blossoms(0);
vector<vector<graph>> allGraphs(0);
vector<pair<int, int>> resh(0);

// Экспонированная ли вершина
bool exp(int xi) {
    for (int j = 0; j < graphCopy[xi - 1].adjacentVertices.size(); j++)
        if (graphCopy[xi - 1].adjacentVertices[j].second) return 0;
    return 1;
}

void DeletePeaks(int peaks) {
    graphCopy[peaks - 1].work = 0;
    altT[peaks - 1].included = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < graphCopy[i].adjacentVertices.size() && graphCopy[i].adjacentVertices[j].first <= peaks; j++)
            if (graphCopy[i].adjacentVertices[j].first == peaks) 
                graphCopy[i].adjacentVertices.erase(graphCopy[i].adjacentVertices.begin() + j);
}

void BlossomAlgorithm() {
    f = 1;
    exposedTop = 0;
    blossoms.clear();
    allGraphs.clear();
    resh.clear();
    altT.clear();
    graphCopy = gr;
    for (int i = 0; i < n; i++) altT.push_back(altTree(i + 1, 0, 1, 0));
    for (int i = 0; i < com.size(); i++) {
        if (i == com.size() - 1) f = 0;
        if (com[i].size() == 1) i++;
        else {
            for (int j = 0; j < n; j++) {
                if (graphCopy[j].component == i) graphCopy[j].work = 1;
                else graphCopy[j].work = 0;
            }
            finish = 0;
            allGraphs.clear();
            blossoms.clear();
            allGraphs.push_back(graphCopy);
            blossoms.push_back({ {0}, 0 });
            BlossomAlgorithm2();
        }
    }
    if (com.back().size() == 1) return BlossomAlgorithm8();
    if (resh.size() == 0) {
        cout << "\n Паросочетание:\n";
        cout << "Пустое множество\n";
        return;
    }
}

void BlossomAlgorithm2() {
    int back = exposedTop;
    exposedTop = 0;
    bool l = 1;
    // Если существуют хотя бы 2 экспонированные вершны, то берём одну из них в качестве корня альтернирующего дерева
    for (int i = 0; i < n && l; i++) 
        if (exp(i + 1) && graphCopy[i].work) {
            if (i + 1 == back) 
                DeletePeaks(i + 1);
            else if (!exposedTop) exposedTop = i + 1;
            else l = 0;
        }
    if (l) return BlossomAlgorithm8();
    altT[exposedTop - 1].included = 1;
    altT[exposedTop - 1].outer = 1;
    altT[exposedTop - 1].parent = 0;
    return BlossomAlgorithm3();
};

void BlossomAlgorithm3() {
    // Проверяем все внешние вершины альтенирующего дерева
    for (int h = 0; h < n; h++) if (altT[h].included && altT[h].outer)
        for (int i = 0; i < graphCopy[h].adjacentVertices.size(); i++) {
            int xi = graphCopy[h].adjacentVertices[i].first; // Смежная вершина
            // если xi - экспонированная вершина - шаг 4
            if (exp(xi)) {
                altT[xi - 1].included = 1;
                altT[xi - 1].parent = h + 1;
                return BlossomAlgorithm4(xi);
            }
            // если xi - не пренадлежит дереву и не является экспонированной - шаг 5
            else if (!altT[xi - 1].included) {
                BlossomAlgorithm5(h + 1, xi);
                if (finish) return;
                h = -1;
                break;
            }
        }

    // Удалить текущее дерево
    for (int i = 0; i < n; i++) altT[i].included = 0;
    return BlossomAlgorithm2();
}

void BlossomAlgorithm4(int xi) { // Найдена аугуметальная цепь из корня к xi

    // Построить новое паросочетание
    while (xi) {
        // Удаляем старое ребро из паросочетания
        for (int i = 0; i < graphCopy[xi - 1].adjacentVertices.size(); i++) graphCopy[xi - 1].adjacentVertices[i].second = 0;
        for (int i = 0; i < graphCopy[altT[xi - 1].parent - 1].adjacentVertices.size(); i++) graphCopy[altT[xi - 1].parent - 1].adjacentVertices[i].second = 0;
        // Добавляем новое
        for (int i = 0; i < graphCopy[xi - 1].adjacentVertices.size(); i++) 
            if (graphCopy[xi - 1].adjacentVertices[i].first == altT[xi - 1].parent)
                graphCopy[xi - 1].adjacentVertices[i].second = 1;
        for (int i = 0; i < graphCopy[altT[xi - 1].parent - 1].adjacentVertices.size(); i++) {
            if (graphCopy[altT[xi - 1].parent - 1].adjacentVertices[i].first == xi)
                graphCopy[altT[xi - 1].parent - 1].adjacentVertices[i].second = 1;
        }
        xi = altT[xi - 1].parent;
        if (xi) xi = altT[xi - 1].parent;
    }

    // Удалить текущее дерево
    for (int i = 0; i < n; i++) altT[i].included = 0;
    return BlossomAlgorithm2();
}

void BlossomAlgorithm5(int x0, int xi) { 
    // добавить к дереву (x0; xi)
    altT[xi - 1].included = 1;
    altT[xi - 1].outer = 0;
    altT[xi - 1].parent = x0;

    // найти ребро (xi; xk) принадлежащее паросочетанию
    int xk = 0;
    for (int i = 0; i < graphCopy[xi - 1].adjacentVertices.size(); i++) 
        if (graphCopy[xi - 1].adjacentVertices[i].second)
            xk = graphCopy[xi - 1].adjacentVertices[i].first;

    // добавить к дереву (xi; xk)
    altT[xk - 1].included = 1;
    altT[xk - 1].outer = 1;
    altT[xk - 1].parent = xi;

    // если существует ребро между xk  и другой внешней вершиной
    for (int i = 0; i < graphCopy[xk - 1].adjacentVertices.size(); i++)
        if (altT[graphCopy[xk - 1].adjacentVertices[i].first - 1].outer == 1 && altT[graphCopy[xk - 1].adjacentVertices[i].first - 1].included) 
            return BlossomAlgorithm6(xk, graphCopy[xk - 1].adjacentVertices[i].first);
}

void BlossomAlgorithm6(int xi, int xk) {
    // опознать и срезать получившийся цветок
    allGraphs.push_back(graphCopy);
    blossoms.push_back({ vector<int>(0), 0 });
    vector<int> xi_exposedTop(0); 
    xi_exposedTop.push_back(xi);
    int par = altT[xi - 1].parent;
    while (par != 0) {
        xi_exposedTop.push_back(par);
        par = altT[par - 1].parent;
    }
    vector<int> xk_exposedTop(0);
    xk_exposedTop.push_back(xk);
    par = altT[xk - 1].parent;
    while (par != 0) {
        xk_exposedTop.push_back(par);
        par = altT[par - 1].parent;
    }
    int u = xk_exposedTop.back();
    while (xi_exposedTop.size() != 0 && xk_exposedTop.size() != 0 && xi_exposedTop.back() == xk_exposedTop.back()) {
        u = xk_exposedTop.back();
        xi_exposedTop.pop_back();
        xk_exposedTop.pop_back();
    }
    altT[u - 1].outer = 1;
    for (int i = 0; i < xi_exposedTop.size(); i++) {
        blossoms.back().first.push_back(xi_exposedTop[i]);
        graphCopy[xi_exposedTop[i] - 1].work = 0;
        altT[xi_exposedTop[i] - 1].included = 0;
    }
    blossoms.back().first.push_back(u);
    blossoms.back().second = u;
    for (int i = xk_exposedTop.size() - 1; i >= 0; i--) {
        blossoms.back().first.push_back(xk_exposedTop[i]);
        graphCopy[xk_exposedTop[i] - 1].work = 0;
        altT[xk_exposedTop[i] - 1].included = 0;
    }
    // смежные вершины псевдовершины
    for (int i = 0; i < blossoms.back().first.size(); i++) if (blossoms.back().first[i] != u)
        for (int j = 0; j < graphCopy[blossoms.back().first[i] - 1].adjacentVertices.size(); j++)
            graphCopy[u - 1].adjacentVertices.push_back(graphCopy[blossoms.back().first[i] - 1].adjacentVertices[j]);

    sort(graphCopy[u - 1].adjacentVertices.begin(), graphCopy[u - 1].adjacentVertices.end());
    for (int i = 1; i < graphCopy[u - 1].adjacentVertices.size(); i++)
        // Удаляем повторяющиеся вершины
        while (i < graphCopy[u - 1].adjacentVertices.size() && graphCopy[u - 1].adjacentVertices[i].first == graphCopy[u - 1].adjacentVertices[i - 1].first)
            graphCopy[u - 1].adjacentVertices.erase(graphCopy[u - 1].adjacentVertices.begin() + i);

    //  Удаление вершин цветка из её смежны вершин u
    for (int i = 0; i < blossoms.back().first.size(); i++) 
        for (int j = 0; j < graphCopy[u - 1].adjacentVertices.size() && graphCopy[u - 1].adjacentVertices[j].first <= blossoms.back().first[i]; j++)
            if (blossoms.back().first[i] == graphCopy[u - 1].adjacentVertices[j].first)
                graphCopy[u - 1].adjacentVertices.erase(graphCopy[u - 1].adjacentVertices.begin() + j);

    //удаление из остальных вершин рёбер с вешинами цветка
    for (int i = 0; i < blossoms.back().first.size(); i++) 
        DeletePeaks(blossoms.back().first[i]);
    // Добавление в вершины, смежные с u, саму вершину u
    for (int i = 0; i < graphCopy[u - 1].adjacentVertices.size(); i++) {
        pair<int, int> v = graphCopy[u - 1].adjacentVertices[i];
        graphCopy[v.first - 1].adjacentVertices.push_back({ u, v.second });
        sort(graphCopy[v.first - 1].adjacentVertices.begin(), graphCopy[v.first - 1].adjacentVertices.end());
    }
    graphCopy[u - 1].work = 1;
    altT[u - 1].included = 1;
    altT[u - 1].outer = 1;
    BlossomAlgorithm3();
}

void Connect(int v1, int v2, int i) {
    // Меняем во всех сохранённых графах до i
    for (int j = 0; j < i + 1; j++) {
        for (int h = 0; h < allGraphs[j][v1 - 1].adjacentVertices.size(); h++)
            allGraphs[j][v1 - 1].adjacentVertices[h].second = 0;
        for (int h = 0; h < allGraphs[j][v2 - 1].adjacentVertices.size(); h++)
            allGraphs[j][v2 - 1].adjacentVertices[h].second = 0;

        for (int g = 0; g < allGraphs[j][v1 - 1].adjacentVertices.size(); g++)
            if (allGraphs[j][v1 - 1].adjacentVertices[g].first == v2) {
                allGraphs[j][v1 - 1].adjacentVertices[g].second = 1;
                break;
            }

        for (int g = 0; g < allGraphs[j][v2 - 1].adjacentVertices.size(); g++)
            if (allGraphs[j][v2 - 1].adjacentVertices[g].first == v1) {
                allGraphs[j][v2 - 1].adjacentVertices[g].second = 1;
                break;
            }
    }
}

void BlossomAlgorithm8() {
    finish = 1;
    allGraphs.push_back(graphCopy);
    // Распустить цветы, начиная с крайнего и выделить в них паросочетание таким образом, чтобы в этом цветке экспонированной вершиной
    // осталась та, которая входит входит в паросочетание в нераспустившемся цветке(и без того входит в паросочетание)
    
    // Переносим паросочетание на все сохранённые графы
    for (int h = 0; h < blossoms.size(); h++)
        for (int i = 0; i < n; i++) {
            if (graphCopy[i].work)
                for (int j = 0; j < allGraphs[h][i].adjacentVertices.size(); j++)
                    allGraphs[h][i].adjacentVertices[j].second = 0;

            for (int j = 0; j < graphCopy[i].adjacentVertices.size(); j++)
                if (graphCopy[i].work && graphCopy[i].adjacentVertices[j].second)
                    for (int g = 0; g < allGraphs[h][i].adjacentVertices.size(); g++)
                        if (allGraphs[h][i].adjacentVertices[g].first == graphCopy[i].adjacentVertices[j].first)
                            allGraphs[h][i].adjacentVertices[g].second = 1;
        }

    for (int i = blossoms.size() - 1; i >= 1; i--) {
        int v = 0; // Вершина принадлежащя ребру, входящему в паросочетание, которая не входит в цветок
        for (int j = 0; j < allGraphs[i + 1][blossoms[i].second - 1].adjacentVertices.size() && !v; j++)
            if (allGraphs[i + 1][blossoms[i].second - 1].adjacentVertices[j].second) {
                v = allGraphs[i + 1][blossoms[i].second - 1].adjacentVertices[j].first;
            }
        int v1 = 0;// Вершина, входящая в цветок, смежная с v (экспонированная в цветке)
        int posv1 = 0;
        for (int j = 0; j < allGraphs[i][v - 1].adjacentVertices.size() && !v1; j++)
            for (int h = 0; h < blossoms[i].first.size() && !v1; h++)
                if (allGraphs[i][v - 1].adjacentVertices[j].first == blossoms[i].first[h]) {
                    v1 = blossoms[i].first[h];
                    posv1 = h;
                }
        Connect(v, v1, i);

        if (posv1 == blossoms[i].first.size()) {
            for (int h = 0; h < blossoms[i].first.size() - 2; h += 2)
                Connect(blossoms[i].first[h], blossoms[i].first[h + 1], i);
        }
        else {
            for (int f = posv1 - 1; f >= 1; f -= 2)
                Connect(blossoms[i].first[f], blossoms[i].first[f - 1], i);
            for (int f = posv1 + 1; f <= blossoms[i].first.size() - 2; f += 2)
                Connect(blossoms[i].first[f], blossoms[i].first[f + 1], i);
            if (posv1 % 2)
                Connect(blossoms[i].first[0], blossoms[i].first.back(), i);
        }
    }
    graphCopy = allGraphs[0];

    if (!f) {
        allGraphs[0] = graphCopy;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < allGraphs[0][i].adjacentVertices.size(); j++)
                if (allGraphs[0][i].adjacentVertices[j].second && allGraphs[0][i].adjacentVertices[j].first > allGraphs[0][i].peaks)
                    resh.push_back({ allGraphs[0][i].peaks, allGraphs[0][i].adjacentVertices[j].first });

        cout << "\n Паросочетание:\n";
        for (int i = 0; i < resh.size(); i++)
            cout << i + 1 << ". " << resh[i].first << " " << resh[i].second << endl;
    }
}