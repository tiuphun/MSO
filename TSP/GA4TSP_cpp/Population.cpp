#include "ea.h"

void Population::rand_init(int pop_size){
    while (indivs.size() < pop_size) {
        Individual indiv;
        indiv.random_init();
        indiv.fitness = indiv.calculate_fitness();
        // cout << indiv.fitness << endl;
        
        if (best.fitness < indiv.fitness) {
            best = indiv;
        }

        indivs.push_back(indiv);
    }
}

vector<Individual> Population::singplePointCrossover(Individual &p1, Individual &p2){
    // single point crossover
    int dim = N;
    int p = rand() % dim;

    Individual o1;
    Individual o2;

    int v1[dim];
    int v2[dim];
    for (int i=0; i<dim; i++) {
        v1[i] = 0;
        v2[i] = 0;
    }

    for (int i=0; i<p; i++) {
        o1.chromosome[i] = p2.chromosome[i];
        v1[o1.chromosome[i]] = 1;

        o2.chromosome[i] = p1.chromosome[i];
        v2[o2.chromosome[i]] = 1;
    }

    int idx = 0;
    for (int i=p; i<dim; i++) {
        while (v1[p1.chromosome[idx]] != 0) {
            idx++;
        }
        o1.chromosome[i] = p1.chromosome[idx++];
        v1[o1.chromosome[i]] = 1;
    }

    idx = 0;
    for (int i=p; i<dim; i++) {
        while (v2[p2.chromosome[idx]] != 0) {
            idx++;
        }
        o2.chromosome[i] = p2.chromosome[idx++];
        v2[o2.chromosome[i]] = 1;
    }

    vector<Individual> child;
    child.push_back(o1);
    child.push_back(o2);
    return child;
}

int individual_comparator(Individual i1, Individual i2) {
    return (i1.fitness > i2.fitness);
}

void Population::survival_selection(int pop_size){
    sort(indivs.begin(), indivs.end(), individual_comparator);
    
    while (indivs.size() > pop_size) {
        indivs.pop_back();
    }
}