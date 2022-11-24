#include "ea.h"
#include <fstream>

// #define FILE "/Users/cuonglv/Desktop/ga/data/att48_33523.txt"
#define FILE "/Users/cuonglv/Desktop/ga/data/dantzig_699.txt"
#define POP_SIZE 200
#define MAX_GENERATION 500
#define CROSSOVER_RATE 0.9
#define MUTATION_RATE 0.05

double dis[MAX_N][MAX_N];
int N;
Population pop;

void read_input_data(string file);
vector<Individual> reproduction(vector<Individual> mating_pool);

int main() {
    read_input_data(FILE);

    int generation = 0;
    pop.rand_init(POP_SIZE);
    cout << "Generation " << generation << ", best obj = " << (1.0 / pop.best.fitness) << endl;

    while (generation < MAX_GENERATION) {
        generation++;

        // crossover, mutation, and evaluation
        vector<Individual> offspring = reproduction(pop.indivs);

        // survival selection
        pop.indivs.insert(pop.indivs.end(), offspring.begin(), offspring.end());
        pop.survival_selection(POP_SIZE);

        // update best solution
        if (pop.best.fitness < pop.indivs[0].fitness) {
            pop.best = pop.indivs[0];
        }

        cout << "Generation " << generation << ", best obj = " << (1.0 / pop.best.fitness) << endl;
    }

    // print final solution
    cout << "Best solution found: ";
    pop.best.print();
}

vector<Individual> reproduction(vector<Individual> mating_pool) {
    vector<Individual> offspring;

    while (offspring.size() < POP_SIZE) {
        int p1 = rand() % mating_pool.size();
        int p2 = rand() % mating_pool.size();
        while (p1 == p2) {
            p2 = rand() % mating_pool.size();
        }

        if ((double) rand() / RAND_MAX <= CROSSOVER_RATE) {
            vector<Individual> child = pop.singplePointCrossover(mating_pool[p1], mating_pool[p2]);
            offspring.insert(offspring.end(), child.begin(), child.end());
        }
    }

    for (int i=0; i<offspring.size(); i++) {
        if ((double) rand() / RAND_MAX <= MUTATION_RATE) {
            offspring[i].swapMutate();
        }

        offspring[i].fitness = offspring[i].calculate_fitness();
    }

    return offspring;
}

void read_input_data(string file)
{
    ifstream fin(file);

    if (!fin.is_open()) {
        cout << "open file error" << endl;
    }

    fin >> N;
    
    for (int i=0; i<N; i++) {
        for (int j=0; j<N; j++) {
            fin >> dis[i][j];

            // cout << dis[i][j] << "\t";
        }
        // cout << endl;
    }

    fin.close();
}
