#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <limits>
#include <cmath>
#include <random>

const int POPULATION_SIZE = 1000;
const int MAX_ITERATIONS = 100;
const int SEQUENCE_LENGTH = 31;
const double MUTATION_RATE = 0.1;

const std::vector<int> X0 = { 1, 0, 1, 0,   0, 0, 0, 1,   0, 0, 0, 1,   1, 0, 1, 1,   0, 0, 0, 0,   1, 1, 0, 0,   1, 1, 1, 0,   0, 1, 1 };



struct Individual {
    std::vector<int> sequence;
    int fitness;
};



Individual selectParent(const std::vector<Individual>& population) {
    int tournamentSize = 5; 
    int bestIndex = -1;
    int bestFitness = std::numeric_limits<int>::max();

    for (int i = 0; i < tournamentSize; ++i) {
        int index = std::rand() % POPULATION_SIZE;
        if (population[index].fitness < bestFitness) {
            bestFitness = population[index].fitness;
            bestIndex = index;
        }
    }

    return population[bestIndex];
}


int calculateCrossCorrelation(const std::vector<int>& seq1, const std::vector<int>& seq2, int shift) {
    int correlation = 0;
    int n = seq1.size();
    for (int i = 0; i < n; ++i) {
        int j = (i + shift) % n;
        correlation += (seq1[i] == seq2[j]) ? 1 : -1;
    }
    return correlation;
}

int calculateFitness(const std::vector<int>& sequence) {
    int fitness = 0;

    // Kroskorelacija sa X0
    for (int k = 0; k < SEQUENCE_LENGTH; ++k) {
        int crossCorr = calculateCrossCorrelation(sequence, X0, k);
        if (crossCorr < -4 || crossCorr > 6) {
            fitness += abs(crossCorr < -4 ? crossCorr + 4 : crossCorr - 6);
        }
    }

    // Autokorelacija
    for (int k = 1; k < SEQUENCE_LENGTH; ++k) {
        int autoCorr = calculateCrossCorrelation(sequence, sequence, k);
        if (autoCorr < -18 || autoCorr > 12) {
            fitness += abs(autoCorr < -18 ? autoCorr + 18 : autoCorr - 12);
        }
    }

    return fitness;
}


bool isSolutionValid(const std::vector<int>& sequence) {
    if (sequence.size() != SEQUENCE_LENGTH) {
        return false;
    }

    int countZero = std::count(sequence.begin(), sequence.end(), 0);
    int countOne = SEQUENCE_LENGTH - countZero;
    if (abs(countZero - countOne) != 1) {
        return false;
    }

    for (int k = 0; k < SEQUENCE_LENGTH; ++k) {
        int crossCorr = calculateCrossCorrelation(sequence, X0, k);
        if (crossCorr < -4 || crossCorr > 6) {
            return false;
        }
    }

    for (int k = 1; k < SEQUENCE_LENGTH; ++k) {
        int autoCorr = calculateCrossCorrelation(sequence, sequence, k);
        if (autoCorr < -18 || autoCorr > 12) {
            return false;
        }
    }

    return true;
}

void crossover(const Individual& parent1, const Individual& parent2, Individual& child1, Individual& child2) {
    int crossoverPoint = std::rand() % SEQUENCE_LENGTH;
    child1.sequence.clear();
    child2.sequence.clear();
    for (int i = 0; i < SEQUENCE_LENGTH; ++i) {
        if (i < crossoverPoint) {
            child1.sequence.push_back(parent1.sequence[i]);
            child2.sequence.push_back(parent2.sequence[i]);
        }
        else {
            child1.sequence.push_back(parent2.sequence[i]);
            child2.sequence.push_back(parent1.sequence[i]);
        }
    }
}


void mutate(Individual& individual) {
    int mutationPoint = std::rand() % SEQUENCE_LENGTH;
    individual.sequence[mutationPoint] = 1 - individual.sequence[mutationPoint];
}

std::vector<int> simulatedAnnealing(const std::vector<int>& initialState) {
    const double INITIAL_TEMP = 100.0;
    const double COOLING_RATE = 0.99;  
    const int SA_MAX_ITERATIONS = 10000;
    const int NUM_BITS_TO_MUTATE = 3;  

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);

    std::vector<int> bestState = initialState;
    int bestFitness = calculateFitness(bestState);
    std::vector<int> currentState = initialState;

    double temp = INITIAL_TEMP;
    for (int i = 0; i < SA_MAX_ITERATIONS; ++i) {
        std::vector<int> newState = currentState;

        for (int j = 0; j < NUM_BITS_TO_MUTATE; ++j) {
            int mutationPoint = std::rand() % SEQUENCE_LENGTH;
            newState[mutationPoint] = 1 - newState[mutationPoint];
        }

        int newFitness = calculateFitness(newState);

        if (newFitness < bestFitness || dis(gen) < exp((bestFitness - newFitness) / temp)) {
            currentState = newState;
            if (newFitness < bestFitness) {
                bestState = newState;
                bestFitness = newFitness;
            }
        }

        temp *= COOLING_RATE;
    }
    return bestState;
}

void printSolution(const std::vector<int>& solution) {
    std::cout << "Rješenje: ";
    for (int bit : solution) {
        std::cout << bit << " ";
    }
    std::cout << std::endl << "Fitness: " << calculateFitness(solution) << std::endl;
}


double calculateAverageFitness(const std::vector<Individual>& population) {
    double totalFitness = 0.0;
    for (const auto& individual : population) {
        totalFitness += individual.fitness;
    }
    return totalFitness / population.size();
}

void geneticAlgorithm() {
    std::vector<Individual> population(POPULATION_SIZE);

    Individual bestSolution;
    int bestFitness = std::numeric_limits<int>::max();

    for (int i = 0; i < POPULATION_SIZE; ++i) {
        for (int j = 0; j < SEQUENCE_LENGTH; ++j) {
            population[i].sequence.push_back(rand() % 2); // Dodajemo 0 ili 1
        }
        population[i].fitness = calculateFitness(population[i].sequence);
    }

    double prevAvgFitness = calculateAverageFitness(population);
    double deltaFitness = 0.0;
    const double FITNESS_THRESHOLD = 0.01; 

    for (int iteration = 0; iteration < MAX_ITERATIONS; ++iteration) {
        
       
        std::vector<Individual> newPopulation;


        
        // Selekcija i ukrštanje
        while (newPopulation.size() < POPULATION_SIZE) {
            Individual parent1 = selectParent(population);
            Individual parent2 = selectParent(population);

            Individual child1, child2;
            crossover(parent1, parent2, child1, child2);

            if (rand() / (double)RAND_MAX < MUTATION_RATE) {
                mutate(child1);
            }
            if (rand() / (double)RAND_MAX < MUTATION_RATE) {
                mutate(child2);
            }

            child1.fitness = calculateFitness(child1.sequence);
            child2.fitness = calculateFitness(child2.sequence);

            newPopulation.push_back(child1);
            newPopulation.push_back(child2);
        }

        population = newPopulation;

        // Ažuriranje bestSolution i bestFitness
        for (const auto& individual : population) {
            if (individual.fitness < bestFitness) {
                bestFitness = individual.fitness;
                bestSolution = individual;
            }
        }
	// Uslov za izlazak
        if (isSolutionValid(bestSolution.sequence)) {
            std::cout << "genetski nasao najbolje rjesenje" << std::endl;
            std::cout << "Finalno rjesenje: ";
            for (int bit : bestSolution.sequence) {
                std::cout << bit << " ";
            }
            return;
        }
        double currentAvgFitness = calculateAverageFitness(population);
        deltaFitness = fabs(prevAvgFitness - currentAvgFitness);
        prevAvgFitness = currentAvgFitness;

        if (deltaFitness < FITNESS_THRESHOLD) {
            std::vector<int> annealedSolution = simulatedAnnealing(bestSolution.sequence);
            if (isSolutionValid(annealedSolution)) {
                std::cout << "simulirano kaljenje naslo najbolje rjesenje" << std::endl;
                printSolution(annealedSolution);
                return;
            }
        }
        
    }

    geneticAlgorithm(); 
}



int main() {
    srand(time(NULL));
    geneticAlgorithm();
    return 0;
}