#include <iostream>
#include <vector>

#define MAX_N 200

using namespace std;

extern double dis[MAX_N][MAX_N];
extern int N;

class Individual {
public:    
    int chromosome[MAX_N];
    double fitness;

    Individual();
    void random_init();
    double calculate_fitness();
    void swapMutate();
    void print();
};

class Population {
public:
    vector<Individual> indivs;
    Individual best;

    void rand_init(int pop_size);
    vector<Individual> singplePointCrossover(Individual &p1, Individual &p2);
    void survival_selection(int pop_sizes);
};
