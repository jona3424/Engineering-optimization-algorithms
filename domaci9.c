#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_AGENTS 69
#define NUM_PARAMS 6
#define TOLERANCE 1e-14

typedef struct {
    double params[NUM_PARAMS];
} Agent;

double xCoords[] = { -15, -10, -5, 0, 5, 10, 15, 15, 15, 15, 15, 15, 15, 10, 5, 0, -5, -10, -15, -15, -15, -15, -15, -15 };
double yCoords[] = { -15, -15, -15, -15, -15, -15, -15, -10, -5, 0, 5, 10, 15, 15, 15, 15, 15, 15, 15, 10, 5, 0, -5, -10 };
double targets[] = { -0.09940112332480822, -0.09570265063923192, -0.07782620994584906, -0.044595775065571636, -0.008470411838648773, -0.0013292572938769093, -0.01402876134848341, 0.0011785680597112547, -0.0016096599564817682, -0.03141072397571561, -0.05773121434057853, -0.07098734083487862, -0.07421256224434619, -0.09674779542915338, -0.13216942328836218, -0.18406033359301877, -0.24214426775005213, -0.25978279767024376, -0.2186443973931424, -0.3289283483195699, -0.4205252223787085, -0.32130499477499636, -0.205134242990832, -0.13760381018149595 };

int compareAgents(const Agent* a1, const Agent* a2) {
    for (int i = 0; i < NUM_PARAMS; i++) {
        if (a1->params[i] != a2->params[i]) return 0;
    }
    return 1;
}

double euclideanDistance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

double evaluateAgent(const Agent* agent) {
    double score = 0;
    for (int i = 0; i < sizeof(targets) / sizeof(targets[0]); i++) {
        double d1 = euclideanDistance(xCoords[i], yCoords[i], agent->params[0], agent->params[1]);
        double d2 = euclideanDistance(xCoords[i], yCoords[i], agent->params[2], agent->params[3]);
        if (d1 == 0 || d2 == 0) return 99999999;
        score += pow(agent->params[4] / d1 + agent->params[5] / d2 - targets[i], 2);
    }
    return score;
}

void updatePopulation(Agent* population) {
    Agent newPopulation[MAX_AGENTS];
    for (int i = 0; i < MAX_AGENTS; i++) {
        Agent* current = &population[i], trial;
        Agent* a, * b, * c;
        do { a = &population[rand() % MAX_AGENTS]; } while (compareAgents(current, a));
        do { b = &population[rand() % MAX_AGENTS]; } while (compareAgents(current, b) || compareAgents(a, b));
        do { c = &population[rand() % MAX_AGENTS]; } while (compareAgents(current, c) || compareAgents(a, c) || compareAgents(b, c));

        int j = rand() % NUM_PARAMS;
        for (int k = 0; k < NUM_PARAMS; k++) {
            if (k == j || (rand() % 100) < 90) {
                double newVal = a->params[k] + (b->params[k] - c->params[k]) * 0.8;
                trial.params[k] = (newVal >= -15 && newVal <= 15) ? newVal : current->params[k];
            }
            else {
                trial.params[k] = current->params[k];
            }
        }

        newPopulation[i] = (evaluateAgent(&trial) < evaluateAgent(current)) ? trial : *current;
    }

    for (int i = 0; i < MAX_AGENTS; i++) {
        population[i] = newPopulation[i];
    }
}

int main() {
    srand(time(NULL));
    Agent population[MAX_AGENTS];
    for (int i = 0; i < MAX_AGENTS; i++) {
        for (int j = 0; j < NUM_PARAMS; j++) {
            population[i].params[j] = (rand() % 31) - 15;
        }
    }

    while (1) {
        updatePopulation(population);
        for (int i = 0; i < MAX_AGENTS; i++) {
            double score = evaluateAgent(&population[i]);
            if (score < TOLERANCE) {
                printf("%e: ", score);
                for (int j = 0; j < NUM_PARAMS; j++) {
                    printf("%f ", population[i].params[j]);
                }
                printf("\n");
                return 0;
            }
        }
    }

    return 0;
}