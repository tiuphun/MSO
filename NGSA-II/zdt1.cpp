#include <bits/stdc++.h>
using namespace std;

#define INF 1e9
#define OBJ_CNT 2
#define N 10
#define ETA 5
#define POP_SIZE 100
#define MAX_GENERATION 500
#define CROSSOVER_RATE 0.9
#define MUTATION_RATE 0.1


double random() {
    return 1.0 * rand() / RAND_MAX;
}

// Class biểu diễn một cá thể
class Individual {
public:
    int dim;                    // Chiều dài chuỗi gen của cá thể
    int num_objectives;         // Số hàm mục tiêu

    double x[N];  // Chuỗi gen biểu diễn đồ vật được chọn
    double fitness[OBJ_CNT];    // Giá trị hàm thích nghi

    Individual() {
        this->dim = N;
        this->num_objectives = OBJ_CNT;
    }

    Individual(const Individual &p) {
        this->dim = p.dim;
        this->num_objectives = p.num_objectives;
        for (int i=0; i<p.dim; i++)
            this->x[i] = p.x[i];
        for (int i=0; i<num_objectives; i++)
            this->fitness[i] = p.fitness[i];
    }

    void random_init() {
        for (int i=0; i<dim; i++)
            x[i] = random();
    }

    double f1() {
        return x[0];
    }

    double f2() {
        double g = 1.0;
        for (int i=1; i<dim; i++)
            g += 9.0/(dim-1) * x[i];
        return 1 - sqrt(f1() / g);
    }

    void evaluate() {
        for (int i=0; i<dim; i++)
            x[i] = max(0.0, min(x[i], 1.0));
        fitness[0] = f1();
        fitness[1] = f2();
    }

    bool dominate(Individual& other) {
        bool superior = false;
        for (int i=0; i<num_objectives; i++)
            if (this->fitness[i] > other.fitness[i])
                return false;
            else if (this->fitness[i] < other.fitness[i])
                superior = true;
        return superior;
    }

    static vector<Individual> crossover(Individual p1, Individual p2) {
        vector<Individual> children;
        Individual c1(p1);
        Individual c2(p2);

        for (int i=0; i<p1.dim; i++) {
            double u = random();
            double beta;
            if (u <= 0.5)
                beta = pow(2 * u, 1.0 / (ETA + 1.0));
            else
                beta = pow(1.0 / (2.0 - 2 * u), 1.0 / (ETA + 1.0));

            c1.x[i] = 0.5 * ((1.0 + beta) * p1.x[i] + (1.0 - beta) * p2.x[i]);
            c2.x[i] = 0.5 * ((1.0 - beta) * p1.x[i] + (1.0 + beta) * p2.x[i]);
        }

        children.push_back(c1);
        children.push_back(c2);
        return children;
    }

    void mutate() {
        for (int i=0; i<dim; i++) {
            double u = random();
            double sigma;
            if (u <= 0.5)
                sigma = pow(2 * u + (1 - 2 * u) * pow(1 - x[i], ETA + 1.0),
                            1.0 / (ETA + 1.0)) - 1.0;
            else
                sigma = 1.0 - pow(2 * (1 - u) + 2 * (u - 0.5) * pow(x[i], ETA + 1.0),
                                  1.0 / (ETA + 1.0));
            x[i] += sigma;
        }
    }
};


class NSGAIISelection {
public:
    static bool is_pareto_point(Individual p, vector<Individual>& pop) {
        for (Individual& indiv: pop)
            if (indiv.dominate(p))
                return false;
//        cout << p.fitness[0] << " " << p.fitness[1] << endl;
        return true;
    }

    static void select(vector<Individual>& pop, int count) {
        vector<Individual> new_pop;

        while (new_pop.size() < count) {
            vector<Individual> pareto;
            vector<Individual> non_pareto;

            for (Individual& p: pop)
                if (is_pareto_point(p, pop))
                    pareto.push_back(p);
                else
                    non_pareto.push_back(p);
            pop = move(non_pareto);

            if (new_pop.size() + pareto.size() > count) {
                sort_by_crowding_distance(pareto);
                pareto.resize(count - new_pop.size());
            }
            new_pop.insert(new_pop.end(), pareto.begin(), pareto.end());
        }

        pop = move(new_pop);
    }

    class ObjectiveComparator {
    public:
        int obj_idx;
        vector<Individual> pop;
        ObjectiveComparator(vector<Individual>& pop) {
            this->pop = pop;
        }

        bool operator()(const int& i1, const int& i2) {
            return pop.at(i1).fitness[obj_idx] < pop.at(i2).fitness[obj_idx];
        }
    };

    static void sort_by_crowding_distance(vector<Individual>& pop) {
        int PS = pop.size();
        int obj_cnt = pop.at(0).num_objectives;
        vector<int> indices;
        double I[PS];

        for (int i=0; i<PS; i++) {
            indices.push_back(i);
            I[i] = 0.0;
        }

        ObjectiveComparator cmp(pop);
        for (int k=0; k<obj_cnt; k++) {
            cmp.obj_idx = k;
            sort(indices.begin(), indices.end(), cmp);

            I[indices.at(0)] = I[indices.at(PS-1)] = INF;
            double range = pop.at(indices.at(PS-1)).fitness[k]
            - pop.at(indices.at(0)).fitness[k];

            for (int i=1; i<PS-1; i++) {
                if (I[indices.at(i)] == INF)
                    continue;
                I[indices.at(i)] += (pop.at(indices.at(i+1)).fitness[k] -
                                     pop.at(indices.at(i-1)).fitness[k]) / range;
            }
        }

        sort(indices.begin(), indices.end(), [&](int ia, int ib) { return I[ia] > I[ib]; });
        vector<Individual> old_pop = move(pop);
        pop.clear();
        for (int i: indices)
            pop.push_back(old_pop.at(i));
    }
};


// Class biểu diễn một quần thể
class Population {
public:
    vector<Individual> population;

    void init() {
        for (int i=0; i<POP_SIZE; i++) {
            Individual p;
            p.random_init();
            p.evaluate();
            population.push_back(p);
        }
    }

    // Sinh sản = lai ghép + đột biến
    void reproduction(int count) {
        vector<Individual> offspring;

        // lai ghép
        while (offspring.size() < count) {
            int i1 = rand() % POP_SIZE;
            int i2 = rand() % POP_SIZE;
            while (i2 == i1)
                i2 = rand() % POP_SIZE;

            Individual p1 = population[i1];
            Individual p2 = population[i2];
            if (random() < CROSSOVER_RATE) {
                vector<Individual> children = Individual::crossover(p1, p2);
                offspring.insert(offspring.end(), children.begin(), children.end());
            }
            else {
                offspring.push_back(Individual(p1));
                offspring.push_back(Individual(p2));
            }
        }
        while (offspring.size() > count)
            offspring.pop_back();

        // đột biến
        for (Individual &indiv: offspring)
            if (random() < MUTATION_RATE)
                indiv.mutate();

        population.insert(population.end(), offspring.begin(), offspring.end());
    }

    // Chọn lọc theo NSGA-II
    void natural_selection(int count) {
        NSGAIISelection::select(population, count);
    }
};

int main() {
    srand ( time(NULL) );

    /*=== Thuật toán NSGA-II ===*/
    // Khởi tạo quần thể
    Population pop;
    pop.init();
    cout << "Generation 0" << endl;

    // vòng lặp tiến hóa
    for (int t=1; t<=MAX_GENERATION; t++) {
        // sinh sản
        pop.reproduction(POP_SIZE);

        // đánh giá cá thể
        for (Individual &indiv: pop.population)
            indiv.evaluate();

        // chọn lọc tự nhiên
        pop.natural_selection(POP_SIZE);

        // in kết quả mỗi thế hệ
        cout << "Generation " << t << endl;
    }

    /*=== In kết quả cuối ===*/
    cout << "===========" << endl;
    cout << "FINAL RESULT:" << endl;
    for (Individual p: pop.population)
        cout << p.fitness[0] << " " << p.fitness[1] << endl;
}
