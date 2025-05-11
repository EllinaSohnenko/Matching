#include "BlossomAlgorithm.h"
#include "Backtracking.h"
#include "readFile.h"
#include <iostream>
#include <string>
#include <regex>
#include <chrono>
using namespace std;

extern vector<graph> gr;
extern int n;

double sr = 0;

class Timer {
private:
    chrono::time_point<chrono::steady_clock> start, end;
public:
    Timer() {
        start = chrono::high_resolution_clock::now(); 
    }
    ~Timer() {
        end = chrono::high_resolution_clock::now();

        chrono::duration<float> duration = end - start;
        //cout << "Время: " << duration.count() << " s" << endl;
        sr += duration.count();
    }
};


int main() {
#pragma region Подсчёт среднего времени
    //setlocale(LC_ALL, "Russian");
    //srand(NULL);
    //for (n = 1; n < 16; n++) {
    //    sr = 0;
    //    gr.clear();
    //    int k = (int)((0.8 * n * (n - 1)) / 2);
    //    vector<pair<int, int>> vv(0);
    //    while (vv.size() < k) {
    //        int x = rand() % n + 1;
    //        int y = rand() % n + 1;
    //        if (x != y) {
    //            if (x < y) {
    //                int c = y;
    //                y = x;
    //                x = c;
    //            }
    //            bool l = 1;
    //            for (int i = 0; i < vv.size() && l; i++) if (vv[i] == pair<int, int> {x, y}) l = 0;
    //            if (l) vv.push_back({ x, y });
    //        }
    //    }
    //    for (int i = 0; i < n; i++) gr.push_back(graph(i + 1, vector<pair<int, int>>(), 0));
    //    for (int i = 0; i < vv.size(); i++) {
    //        gr[vv[i].first - 1].adjacentVertices.push_back({vv[i].second, 0});
    //        gr[vv[i].second - 1].adjacentVertices.push_back({vv[i].first, 0});
    //    }
    //    for (int i = 0; i < n; i++) {
    //        sort(gr[i].adjacentVertices.begin(), gr[i].adjacentVertices.end());
    //        // Удаляем повторяющиеся элементы
    //        for (int j = 1; j < gr[i].adjacentVertices.size(); j++) if (gr[i].adjacentVertices[j] == gr[i].adjacentVertices[j - 1]) {
    //            gr[i].adjacentVertices.erase(gr[i].adjacentVertices.begin() + j);
    //            j--;
    //        }
    //    }
    //    Component();

    //    cout << "n = " << n << ":\n";
    //    for (int i = 0; i < 10; i++) {
    //        Timer t;
    //        Backtracking();
    //        t.~Timer();
    //    }
    //    cout << "Среднее: " << sr / double(10) <<  endl << endl;
    //}
    //return 0;
#pragma endregion
    setlocale(LC_ALL, "Russian");
    int otv = -1;
    while (1) {
        cout << "\n Список команд:\n1. Ввести граф вручную;\n2. Считать граф из файла;\n0. Выход.\n";
        string s;
        while (otv < 0 || otv > 2) {
            cout << "Введите номер команды: ";
            getline(cin, s);
            regex r(R"((\s*)[0-2](\s*))");
            if (!regex_match(s, r)) cout << "Неверный номер команды. ";
            else otv = stoi(s);
        }
        switch (otv) {
        case 0: return 0;
        case 1: 
            otv = ReadGraph();
            break;
        case 2:
            otv = ReadInFile();
            break;
        }
        if (otv) {
            while (otv != 3) {
                cout << "\n Способы нахождения паросочетания:\n1. Алгоритм Эдмондса(соцветий);\n2. Перебор с возвратом;\n3. Задать новый граф;\n0. Выход.\n";
                otv = -1;
                while (otv < 0 || otv > 3) {
                    cout << "Введите номер команды: ";
                    getline(cin, s);
                    regex r(R"((\s*)[0-3](\s*))");
                    if (!regex_match(s, r)) cout << "Неверный номер команды. ";
                    else otv = stoi(s);
                }
                switch (otv) {
                case 0: return 0;
                case 1:
                    BlossomAlgorithm();
                    break;
                case 2:
                    Backtracking();
                    break;
                case 3: break;
                }
            }
        }
        otv = -1;
    }
}