#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>

using namespace std;

const int N = 20;
const double INF = numeric_limits<double>::infinity();

struct Tacka {
    double x, y;
};

double udaljenost(const Tacka& a, const Tacka& b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

pair<double, vector<int>> bellmanHeldKarp(const vector<Tacka>& tacke) {
    vector<vector<double>> dp(1 << N, vector<double>(N, INF));
    vector<vector<int>> parent(1 << N, vector<int>(N, -1));

    for (int i = 0; i < N; i++) {
        dp[1 << i][i] = 0;
    }

    for (int mask = 1; mask < (1 << N); mask++) {
        for (int i = 0; i < N; i++) {
            if (mask & (1 << i)) {
                for (int j = 0; j < N; j++) {
                    if (mask & (1 << j) && i != j) {
                        double new_distance = dp[mask ^ (1 << i)][j] + udaljenost(tacke[j], tacke[i]);
                        if (new_distance < dp[mask][i]) {
                            dp[mask][i] = new_distance;
                            parent[mask][i] = j;  
                        }
                    }
                }
            }
        }
    }

    double min_path = INF;
    vector<int> path;
    int last_mask = (1 << N) - 1;

    for (int i = 0; i < N; i++) {
        if (dp[last_mask][i] < min_path) {
            min_path = dp[last_mask][i];
            path.clear();
            int current_point = i;
            int current_mask = last_mask;
            while (current_point != -1) {
                path.push_back(current_point);
                int temp_point = parent[current_mask][current_point];
                current_mask = current_mask ^ (1 << current_point);
                current_point = temp_point;
            }
            reverse(path.begin(), path.end());
        }
    }

    return { min_path, path };
}
int main() {
    vector<Tacka> tacke = {
            {2.7, 33.1}, {2.7, 56.8}, {9.1, 40.3},{9.1, 52.8}, {15.1, 49.6},  {15.3, 37.8}, {21.5, 45.8},
            {22.9, 32.7}, {33.4, 60.5},  {28.4, 31.7}, {34.7, 26.4}, {45.7, 25.1}, {34.7, 45.1},  {46.0, 45.1},
            {54.2, 29.1}, {57.7, 42.1}, {67.9, 19.6},  {51.7, 56.0},  {57.5, 56.0}, {62.0, 58.4},




    };

    pair<double, vector<int>> result = bellmanHeldKarp(tacke);
    double min_path = result.first;
    vector<int> path = result.second;
    cout << "Najkraci put: " << min_path << endl;
    cout << "Putanja: ";
    for (int i : path) {
        cout << i + 1 << " ";
    }
    cout << endl;

    return 0;
}

