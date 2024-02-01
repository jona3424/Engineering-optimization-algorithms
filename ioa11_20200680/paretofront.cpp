#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <fstream>

#define PI 3.14159265358979323846
#define ITERATIONS 1000000

struct Solution {
    double a, b, S, H, R;
};

bool isDominated(const Solution &x, const Solution &y) {
    return (x.H <= y.H && x.R >= y.R) && (x.H < y.H || x.R > y.R);
}

double calculate_R(double a, double b, double S) {
    double sigma = 58 * std::pow(10, 6);
    return 2 * M_PI * a * b / (sigma * S) * std::sqrt(PI / (4*S));
}

double calculate_H(double a, double b, double S) {
    double z0 = b/2 + std::pow(10, -2);
    return 0.5 * std::sqrt(PI / (4*S)) * (((z0 + b/2) / std::sqrt(std::pow(a, 2) + std::pow((z0 + b/2), 2))) - ((z0 - b/2) / std::sqrt(std::pow(a, 2) + std::pow((z0 - b/2), 2))));

}


std::vector<Solution> findParetoFront(const std::vector<Solution> &solutions) {
    std::vector<Solution> paretoFront;
    for (const auto &solution : solutions) {
        bool dominated = std::any_of(solutions.begin(), solutions.end(), [&solution](const Solution &other) {
            return isDominated(solution, other);
        });

        if (!dominated) {
            paretoFront.push_back(solution);
        }
    }
    return paretoFront;
}

void saveSolutions(const std::vector<Solution> &solutions, const std::string &filename) {
    std::ofstream file(filename);
    for (const auto &solution : solutions) {
        file << solution.a << "," << solution.b << "," << solution.S << "," << solution.H << "," << solution.R << std::endl;
    }
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));
    std::vector<Solution> solutions;

    for (int i = 0; i < ITERATIONS; ++i) {
        double a = static_cast<double>(rand()) / RAND_MAX * (5e-2 - 1e-2) + 1e-2;
        double b = static_cast<double>(rand()) / RAND_MAX * (0.4 - 0.1) + 0.1;
        double S = static_cast<double>(rand()) / RAND_MAX * (3e-6 - 1e-6) + 1e-6;
        solutions.push_back({a, b, S, calculate_H(a, b, S), calculate_R(a, b, S)});
    }

    std::vector<Solution> paretoFront = findParetoFront(solutions);

    saveSolutions(solutions, "all_solutions.txt");
    saveSolutions(paretoFront, "pareto_front.txt");

    return 0;
}
