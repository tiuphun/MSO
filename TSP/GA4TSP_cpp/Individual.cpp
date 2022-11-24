#include "ea.h"
#include <algorithm>
#include <random>

mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

Individual::Individual() {
    fitness = -1;
}

void Individual::random_init(){
    vector<int> list;
    for (int i = 0; i < N; i++){
        list.push_back(i);
    }
    shuffle(list.begin(), list.end(), rng);
    
    for (int i=0; i<N; i++) {
        this->chromosome[i] = list[i];
    }
}

double Individual::calculate_fitness() {
    double obj = dis[chromosome[N-1]][chromosome[0]];
    for (int i=1; i<N; i++) {
        obj += dis[chromosome[i-1]][chromosome[i]];
    }
    
    return 1.0 / obj;
}

void Individual::swapMutate() {
    // swap mutation
    int p1 = rand() % N;
    int p2 = rand() % N;

    while (p1 == p2) {
        p2 = rand() % N;
    }

    int tmp = chromosome[p1];
    chromosome[p1] = chromosome[p2];
    chromosome[p2] = tmp;
}

void Individual::print() {
    cout << "obj = " << (1.0 / fitness) << ", path: ";
    for (int i=0; i<N; i++) {
        cout << chromosome[i] << " ";
    }
    cout << endl;
}