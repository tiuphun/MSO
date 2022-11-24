#include <iostream>

#define N 50 
#define UB 50
#define LB -50

#define POP_SIZE 100
#define GENERATION 1000
#define F 0.2
#define CR 0.5

using namespace std;

double pop[POP_SIZE][N];
double pop_fit[POP_SIZE]; // objective values

double offspring[POP_SIZE][N];
double offspring_fit[POP_SIZE]; // objective values

double best[N];
double best_fit = 1e9; // objective value

double calculate_obj(double indiv[]){
    double obj = 0;

    // sphere
    for (int i=0; i<N; i++) {
        double v = LB + indiv[i] * (UB - LB);
        obj += v * v;
    }

    return obj;
}

void random_init() {
    for (int i=0; i<POP_SIZE; i++) {
        for (int j=0; j<N; j++) {
            pop[i][j] = rand() * 1.0 / RAND_MAX;
        }

        pop_fit[i] = calculate_obj(pop[i]);

        if (best_fit > pop_fit[i]) {
            best_fit = pop_fit[i];
            for (int j=0; j<N; j++) {
                best[j] = pop[i][j];
            }
        }
    }
}

int main() {

    // double x[N];
    // for (int i=0; i<N; i++) {
    //     x[i] = 0.;
    // }
    // cout << calculate_obj(x) << endl;

    random_init();
    for (int i=0; i<POP_SIZE; i++) {
        cout << i << " " << pop_fit[i] << endl;
    }

    int gen = 0;
    while (gen++ < GENERATION) {
        for (int i=0; i<POP_SIZE; i++) {
            int k1 = rand() % POP_SIZE;
            int k2 = rand() % POP_SIZE;
            int k3 = rand() % POP_SIZE;
            while (k2 == k1) {
                k1 = rand() % POP_SIZE;
            }

            // create mutant vector
            // DE/rand/1
            double v[N];
            for (int j=0;j<N; j++) {
                v[j] = pop[k3][j] + F * (pop[k1][j] - pop[k2][j]);
            }

            // crossover
            int j_rand = rand() % N;
            for (int j=0; j<N; j++) {
                if (j == j_rand || rand() * 1.0 / RAND_MAX < CR) {
                    offspring[i][j] = v[j];
                } else {
                    offspring[i][j] = pop[i][j];
                }
            }

            offspring_fit[i] = calculate_obj(offspring[i]);
        }

        // survival selection
        for (int i=0; i<POP_SIZE; i++) {
            if (pop_fit[i] >= offspring_fit[i]) {
                for (int j=0; j<N; j++) {
                    pop[i][j] = offspring[i][j];
                }
                pop_fit[i] = offspring_fit[i];
            }

            if (best_fit > pop_fit[i]) {
                best_fit = pop_fit[i];
                for (int j=0; j<N; j++) {
                    best[j] = pop[i][j];
                }
            }
        }

        cout << "Generation " << gen << ", best objective = " << best_fit << endl;
    }

    return 0;
}