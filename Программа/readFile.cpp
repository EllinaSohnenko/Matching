#include "readFile.h"
#include "element.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>
using namespace std;

int n = 0;
vector<graph> gr(0);
vector<vector<int>> com(0);


void PrintGraph() {
    cout << " Граф:\n";
    if (gr.size() == 0) cout << "Пустой граф\n";
    for (int i = 0; i < n; i++) {
        cout << i + 1 << ". ";
        if (gr[i].adjacentVertices.size() == 0) cout << "-";
        for (auto& item : gr[i].adjacentVertices) cout << item.first << " ";
        cout << endl;
    }
}

void Component() {
    com.clear();
    int k = -1, count = 0;
    vector<bool> b(n, 0);
    vector<int> queue(0);
    while (count < n) {
        k++;
        com.push_back(vector<int>(0));
        for (int i = 0; i < n; i++) if (!b[i]) {
            queue.push_back(i + 1);
            b[i] = 1;
            break;
        }
        while (queue.size() != 0) {
            int h = queue[0];
            queue.erase(queue.begin());
            gr[h - 1].component = k;
            com[k].push_back(h);
            count++;
            for (int i = 0; i < gr[h - 1].adjacentVertices.size(); i++) if (!b[gr[h - 1].adjacentVertices[i].first - 1]) {
                queue.push_back(gr[h - 1].adjacentVertices[i].first);
                b[gr[h - 1].adjacentVertices[i].first - 1] = 1;
            }
        }
    }
    for (int i = 0; i < com.size(); i++) sort(com[i].begin(), com[i].end());
}

int ReadInFile() {
    gr.clear();
    ifstream fin("input.txt");
    if (!fin.is_open()) {
        cout << "Ошибка открытия файла.\n";
        return 0;
    }
    int x, y;
    string s;
    getline(fin, s);
    // Проверка корректности количества вершин графа
    regex r(R"((\s*)(\d+)(\s*))");
    if (!regex_match(s, r)) {
        cout << "Данные введены некорректно.";
        return 0;
    }
    n = stoi(s);
    for (int i = 0; i < n; i++) gr.push_back(graph(i + 1, vector<pair<int, bool>>(), 0));
    while (getline(fin, s)) {
        if (s != "") {
            r = (R"((\s*)(\d+)(\s+)(\d+)(\s*))");
            if (!regex_match(s, r)) {
                cout << "Данные введены некорректно.";
                return 0;
            }
            istringstream ss(s);
            ss >> x;
            ss >> y;
            if (x > n || y > n || x <= 0 || y <= 0) {
                cout << "Данные введены некорректно.";
                return 0;
            }
            if (x != y) {
                gr[x - 1].adjacentVertices.push_back({ y, 0 });
                gr[y - 1].adjacentVertices.push_back({ x, 0 });
            }
        }
    }
    for (int i = 0; i < n; i++) {
        sort(gr[i].adjacentVertices.begin(), gr[i].adjacentVertices.end());
        // Удаляем повторяющиеся элементы
        for (int j = 1; j < gr[i].adjacentVertices.size(); j++) if (gr[i].adjacentVertices[j] == gr[i].adjacentVertices[j - 1]) {
            gr[i].adjacentVertices.erase(gr[i].adjacentVertices.begin() + j);
            j--;
        }
    }
    Component();
    PrintGraph();
    return 1;
}

int ReadGraph() {
    gr.clear();
    int x, y;
    string s;
    n = -1;
    while (n == -1) {
        cout << "Введите количество вершин графа: ";
        getline(cin, s);
        // Проверка корректности количества вершин графа
        regex r(R"((\s*)(\d+)(\s*))");
        if (!regex_match(s, r)) cout << "Данные введены некорректно. ";
        else n = stoi(s);
    }
    for (int i = 0; i < n; i++) gr.push_back(graph(i + 1, vector<pair<int, bool>>(), 0));
    cout << "Введите номера вершин, соединённых рёбами через пробел(1 пара - 1 строка) или 0, чтобы закончить ввод:\n";
    while (getline(cin, s)) {
        if (regex_match(s, regex(R"((\s*)[0](\s*))"))) break;
        if (s != "") {
            regex r(R"((\s*)(\d+)(\s+)(\d+)(\s*))");
            if (!regex_match(s, r)) cout << "Данные введены некорректно. Введите номера вершин, соединённых рёбами через пробел(1 пара - 1 строка) или 0, чтобы закончить ввод:\n";
            else {
                istringstream ss(s);
                ss >> x;
                ss >> y;
                if (x > n || y > n || x <= 0 || y <= 0) cout << "Данные введены некорректно. Введите номера вершин, соединённых рёбами через пробел(1 пара - 1 строка) или 0, чтобы закончить ввод:\n";
                else if (x != y) {
                    gr[x - 1].adjacentVertices.push_back({ y, 0 });
                    gr[y - 1].adjacentVertices.push_back({ x, 0 });
                }
            }
        }
    }
    for (int i = 0; i < n; i++) {
        sort(gr[i].adjacentVertices.begin(), gr[i].adjacentVertices.end());
        // Удаляем повторяющиеся элементы
        for (int j = 1; j < gr[i].adjacentVertices.size(); j++) if (gr[i].adjacentVertices[j] == gr[i].adjacentVertices[j - 1]) {
            gr[i].adjacentVertices.erase(gr[i].adjacentVertices.begin() + j);
            j--;
        }
    }
    cout << endl;
    Component();
    PrintGraph();
    return 1;
}
