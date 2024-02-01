#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define NUM_PARTICLES 100
#define NUM_DIMENSIONS 6
#define MAX_ITERATIONS 47
#define TOLERANCE 1e-5

typedef struct {
    double position[NUM_DIMENSIONS];
    double velocity[NUM_DIMENSIONS];
    double best_position[NUM_DIMENSIONS];
    double best_score;
} Particle;

double points[6][3] = {
    {1, 4, 0}, // A
    {3, 2, 0}, // B
    {2, 7, 1}, // C
    {6, 3, 3}, // D
    {7, 6, 5}, // E
    {5, 7, 4}  // F
};

double euclidean_distance_3d(double* p1, double* p2) {
    double sum = 0;
    for (int i = 0; i < 3; i++) {
        sum += (p1[i] - p2[i]) * (p1[i] - p2[i]);
    }
    return sqrt(sum);
}

double objective_function(double* S) {
    double total_distance = 0;

    // Udaljenost od A, B, C do S1
    for (int i = 0; i < 3; i++) {
        total_distance += euclidean_distance_3d(S, points[i]);
    }

    // Udaljenost od D, E, F do S2
    for (int i = 3; i < 6; i++) {
        total_distance += euclidean_distance_3d(S + 3, points[i]);
    }

    // Dodajemo udaljenost između S1 i S2
    total_distance += euclidean_distance_3d(S, S + 3);

    return total_distance;
}

void initialize_particle(Particle* particle) {
    for (int i = 0; i < NUM_DIMENSIONS; i++) {
        particle->position[i] = ((double)rand() / RAND_MAX) * 10 - 5;
        particle->velocity[i] = ((double)rand() / RAND_MAX) * 2 - 1;
        particle->best_position[i] = particle->position[i];
    }
    particle->best_score = objective_function(particle->position);
}

void update_velocity(Particle* particle, double* global_best_position) {
    double w = 0.8; // Inertia
    double c1 = 1.5; // Cognitive component
    double c2 = 1.5;  
    double max_velocity = 0.2; // max brzina

    for (int i = 0; i < NUM_DIMENSIONS; i++) {
        double r1 = (double)rand() / RAND_MAX;
        double r2 = (double)rand() / RAND_MAX;
        particle->velocity[i] = w * particle->velocity[i] +
            c1 * r1 * (particle->best_position[i] - particle->position[i]) +
            c2 * r2 * (global_best_position[i] - particle->position[i]);

        // Limit brzine
        if (particle->velocity[i] > max_velocity) {
            particle->velocity[i] = max_velocity;
        }
        else if (particle->velocity[i] < -max_velocity) {
            particle->velocity[i] = -max_velocity;
        }
    }
}


void update_position(Particle* particle) {
    for (int i = 0; i < NUM_DIMENSIONS; i++) {
        particle->position[i] += particle->velocity[i];
    }
}

int main() {
    srand(time(NULL));
    Particle particles[NUM_PARTICLES];
    double global_best_position[NUM_DIMENSIONS];
    double global_best_score = INFINITY;

    // init
    for (int i = 0; i < NUM_PARTICLES; i++) {
        initialize_particle(&particles[i]);
        if (particles[i].best_score < global_best_score) {
            global_best_score = particles[i].best_score;
            for (int j = 0; j < NUM_DIMENSIONS; j++) {
                global_best_position[j] = particles[i].best_position[j];
            }
        }
    }

    
    for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
        for (int i = 0; i < NUM_PARTICLES; i++) {
            update_velocity(&particles[i], global_best_position);
            update_position(&particles[i]);

            double score = objective_function(particles[i].position);
            if (score < particles[i].best_score) {
                particles[i].best_score = score;
                for (int j = 0; j < NUM_DIMENSIONS; j++) {
                    particles[i].best_position[j] = particles[i].position[j];
                }
            }

            if (score < global_best_score) {
                global_best_score = score;
                for (int j = 0; j < NUM_DIMENSIONS; j++) {
                    global_best_position[j] = particles[i].position[j];
                }
            }
        }

        if (global_best_score <= TOLERANCE) {
            break;
        }
    }

    
    printf("Najkraci put: %f\n", global_best_score);
    printf("Koordinate S1: (%f, %f, %f)\n", global_best_position[0], global_best_position[1], global_best_position[2]);
    printf("Koordinate S2: (%f, %f, %f)\n", global_best_position[3], global_best_position[4], global_best_position[5]);

    return 0;
}
