#include<iostream>
#include<vector>
#include<math.h>
#include<algorithm>
#include<random>

using namespace std;

#define CROSSOVER_RATE 1.0
#define MUTATION_RATE 0.1
#define POPULATION_SIZE 100
#define MAX_GENERATION 1000
#define N 100 //dimension

int* x;
int* y;
double** dis; // dis[i][j] is distance from i to j

void generate_random_dataset();
void free_memory();

//================= Define individual class =================
class Individual {
public:
    vector<int> chromosome; // permutation encoding
    double fitness;         // fitness = 1.0 / obj

    Individual();
    Individual(const vector<int> genes);
    void mutate();
    double calculate_fitness();
};

Individual::Individual() {
    fitness = -1;
}

Individual::Individual(const vector<int> genes) {
    fitness = -1;
    
    for (int i = 0; i < genes.size(); i++) {
        this->chromosome.push_back(genes[i]);
    }
}

void Individual::mutate() {
    // swap mutation
    int p1 = rand() % N;
    int p2 = rand() % N;
    while (p1 == p2) {
        p2 = rand() % N;
    }

    int v1 = chromosome[p1];
    chromosome[p1] = chromosome[p2];
    chromosome[p2] = v1;
}

double Individual::calculate_fitness() {
    double obj = dis[this->chromosome[N-1]][this->chromosome[0]];

    for (int i=0; i<N-1; i++) {
        obj += dis[this->chromosome[i]][this->chromosome[i+1]];
    }

    return 1.0 / obj;
}

bool operator<(const Individual &ind1, const Individual &ind2)
{
    return ind1.fitness > ind2.fitness;
}

//================= end individual definition =================

vector<Individual> crossover(const Individual &p1, const Individual &p2) {
    // single point crossover
    int p = 1 + rand() % (N-2);

    Individual c1, c2;

    int v1[N], v2[N];
    for (int i=0; i<N; i++) {
        v1[i] = 0;
        v2[i] = 0;
    }

    for (int i=0; i<p; i++) {
        c1.chromosome.push_back(p1.chromosome[i]);
        v1[p1.chromosome[i]] = 1;

        c2.chromosome.push_back(p2.chromosome[i]);
        v2[p2.chromosome[i]] = 1;
    }

    for (int i=0; i<N; i++) {
        if (v1[p2.chromosome[i]] == 0) {
            c1.chromosome.push_back(p2.chromosome[i]);
        }

        if (v2[p1.chromosome[i]] == 0) {
            c2.chromosome.push_back(p1.chromosome[i]);
        }
    }

    vector<Individual> offspring;
    offspring.push_back(c1);
    offspring.push_back(c2);

    return offspring;
}

vector<Individual> init_population() {
    vector<Individual> population;
    while (population.size() < POPULATION_SIZE) {
        Individual indiv;

        // generate a random permutation
        vector<int> list;
        for (int i=0; i<N; i++) {
            list.push_back(i);
        }

        while (list.size() > 0) {
            int pos = rand() % list.size();
            indiv.chromosome.push_back(list[pos]);
            list.erase(list.begin() + pos);
        }

        population.push_back(indiv);
    }

    return population;
}

// produce_offspring() basically includes the process of generating [population->] offsprings using crossover
vector<Individual> produce_offspring(const vector<Individual> &mating_pool) {
    vector<Individual> offspring;

    while (offspring.size() < POPULATION_SIZE) {
    //stopping criteria for producing offspring. usually we produce number of offsprings = number of population individuals
        Individual p1 = mating_pool[rand() % mating_pool.size()];
        Individual p2 = mating_pool[rand() % mating_pool.size()];

        if ((double) rand() / RAND_MAX <= CROSSOVER_RATE) { //only perform crossover upon satisfied criteria
            vector<Individual> child = crossover(p1, p2);
            offspring.insert(offspring.end(), child.begin(), child.end());
        } else {
            // just copy p1 to c1, p2 to c2 => (new population individual = old ones) <- not good to do this though
            Individual c1(p1.chromosome);
            Individual c2(p2.chromosome);

            offspring.push_back(c1);
            offspring.push_back(c2);
        }
    }

    for (int i=0; i<offspring.size(); i++) {
        if ((double) rand() / RAND_MAX <= MUTATION_RATE) {
            offspring[i].mutate();
        }
    }

    return offspring;
}

vector<Individual> selection(const vector<Individual> &indivs) {
    vector<Individual> next_pop;
    next_pop.insert(next_pop.end(), indivs.begin(), indivs.end());

    sort(next_pop.begin(), next_pop.end());

    while (next_pop.size() > POPULATION_SIZE) {
        next_pop.pop_back();
    }

    return next_pop;
}

int main(int argc, char **argv) {

    srand(0);
    generate_random_dataset();

    double best_fitness = -1;
    Individual best_indiv;

    int generation = 0;

    // random init population
    vector<Individual> population = init_population();

    // evaluate population
    for (int i=0; i<population.size(); i++) {
        double fit = population[i].calculate_fitness();
        population[i].fitness = fit;

        // update best
        if (fit > best_fitness) {
            best_fitness = fit;
            best_indiv = population[i];
        }
    }
    cout << "Generation " << generation << ", objective = " << (1.0 / best_fitness) << endl;

    while (generation < MAX_GENERATION) {
            //stopping criteria
        generation++;

        // produce offspring
        vector<Individual> offspring = produce_offspring(population);

        // evaluate offspring
        for (int i=0; i<offspring.size(); i++) {
            double fit = offspring[i].calculate_fitness();
            offspring[i].fitness = fit;

            // update best
            if (fit > best_fitness) {
                best_fitness = fit;
                best_indiv = offspring[i];
            }
        }

        // elite selection
        population.insert(population.end(), offspring.begin(), offspring.end());
        population = selection(population);

        cout << "Generation " << generation << ", objective = " << (1.0 / best_fitness) << endl;
    }

    cout << "Best solution found: ";
    for (int i=0; i<N; i++) {
        cout << best_indiv.chromosome[i] << ", ";
    }
    cout << endl << "Objective: " << (1.0 / best_fitness) << endl;

    free_memory();
    return 0;
}


void generate_random_dataset(){
    x = new int[N];
    y = new int[N];

    for (int i=0; i<N; i++) {
        x[i] = rand() % 1000;
        y[i] = rand() % 1000;
    }

    dis = new double*[N];
    for (int i=0; i<N; i++) {
        dis[i] = new double[N];
        for (int j=0; j<N; j++) {
            dis[i][j] = sqrt((x[i] - x[j]) * (x[i] - x[j]) + (y[i] - y[j]) * (y[i] - y[j]));
        }
    }

    // for (int i=0; i<N; i++) {
    //     for (int j=0; j<N; j++) {
    //         cout << dis[i][j] << "\t";
    //     }
    //     cout << endl;
    // }
}

void free_memory() {
    free(x);
    free(y);

    if (dis != NULL) {
        for (int i=0; i<N; i++) {
            free(dis[i]);
        }
    }
    free(dis);
}
