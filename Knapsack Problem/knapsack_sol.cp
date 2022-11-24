/*
Bài toán cái túi (Knapsack):
    Cho N đồ vật, biết mỗi đồ vật có trọng lượng (weight) và trị giá (profit) riêng.
    Cần chọn một số đồ vật bỏ vào một cái túi có giới hạn cân nặng (capacity).
    Tìm cách chọn sao cho không vượt quá capacity của túi mà tổng giá trị là lớn nhất.
*/
/*
Mô tả: Code bên dưới là thuật toán di truyền (GA) với mã hóa nhị phân
Yêu cầu: Hoàn thiện các hàm còn trống (các dòng có ghi "TO-DO")
Lưu ý:
    Có thể dùng bất kỳ toán tử GA đã học nào để hoàn thiện hàm.
    Có thể tự đề xuất hàm thích nghi.
    Khuyến khích thử nghiệm thay đổi tham số, thử nhiều hàm thích nghi, toán tử khác nhau.
*/

#include <bits/stdc++.h>
using namespace std;

#define FILE "knapsack_24.txt"
#define MAX_N 100
#define POP_SIZE 100
#define MAX_GENERATION 500
#define CROSSOVER_RATE 0.9
#define MUTATION_RATE 0.1


// Cấu trúc cho mỗi đồ vật
struct Item {
    int weight;     // Trọng lượng đồ vật
    int profit;     // Giá trị đồ vật
};

int capacity;       // Trọng lượng tối đa mà túi chứa được
Item items[MAX_N];  // Danh sách đồ vật
int N;              // Số đồ vật


// Class biểu diễn một cá thể
class Individual {
public:
    int dim;                    // Chiều dài chuỗi gen của cá thể
    bool chromosomes[MAX_N];    // Chuỗi gen biểu diễn đồ vật được chọn
    double fitness;             // Giá trị hàm thích nghi

    Individual() {
        this->dim = N;
    }

    Individual(const Individual &p) {
        this->dim = p.dim;
        for (int i=0; i<p.dim; i++)
            this->chromosomes[i] = p.chromosomes[i];
        this->fitness = p.fitness;
    }

    void random_init() {
        for (int i=0; i<this->dim; i++)
            if (1.0 * rand() / RAND_MAX < 0.6)
                this->chromosomes[i] = true;
            else
                this->chromosomes[i] = false;
    }

    long long total_profit() {
        long long profit = 0;
        for (int i=0; i<dim; i++)
            if (chromosomes[i] == true)
                profit += items[i].profit;
        return profit;
    }

    long long total_weight() {
        long long weight = 0;
        for (int i=0; i<dim; i++)
            if (chromosomes[i] == true)
                weight += items[i].weight;
        return weight;
    }

    double evaluate() {
//        if (total_weight() > capacity)
//            return 0;
//        return total_profit();
        long long weight = 0, profit = 0;
        for (int i=0; i<dim; i++)
        if (chromosomes[i] == true && weight + items[i].weight <= capacity) {
            weight += items[i].weight;
            profit += items[i].profit;
        }
        return this->fitness = profit;
    }

    static vector<Individual> crossover(Individual p1, Individual p2) {
        vector<Individual> children;
        Individual c1(p1);
        Individual c2(p2);

        int slice = rand() % (p1.dim - 1) + 1;
        for (int i=0; i<slice; i++) {
            c1.chromosomes[i] = p2.chromosomes[i];
            c2.chromosomes[i] = p1.chromosomes[i];
        }
        children.push_back(c1);
        children.push_back(c2);
        return children;
    }

    void mutate() {
        int mutated_index = rand() % dim;
        chromosomes[mutated_index] = !chromosomes[mutated_index];
    }
};


// Class biểu diễn một quần thể
class Population {
public:
    vector<Individual> population;
    Individual best_indiv;

    void init() {
        for (int i=0; i<POP_SIZE; i++) {
            Individual p;
            p.random_init();
            p.evaluate();
            population.push_back(p);
        }

        best_indiv = population[0];
        update_best();
    }

    void update_best() {
        for (Individual &p: population)
            if (p.fitness > best_indiv.fitness)
                best_indiv = p;
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
            if (1.0 * rand() / RAND_MAX < CROSSOVER_RATE) {
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
            if (1.0 * rand() / RAND_MAX < MUTATION_RATE)
                indiv.mutate();

        population.insert(population.end(), offspring.begin(), offspring.end());
    }

    // Chọn lọc tự nhiên để chọn các cá thể sống sót
    void natural_selection(int count) {
        vector<Individual> candidates = population;

        // giữ lại cá thể tốt nhất
        population.clear();
        Individual best = candidates[0];
        for (Individual &indiv: candidates) {
            if (indiv.fitness > best.fitness)
                best = indiv;
        }
        population.push_back(best);

        // xây dựng mảng cộng dồn xác suất
        vector<double> acc_prob;
        long long sum = 0;
        for (Individual &indiv: candidates)
            sum += indiv.fitness;
        double acc = 0.0;
        for (Individual &indiv: candidates) {
            acc_prob.push_back(acc);
            acc += 1.0 * indiv.fitness / sum;
        }

        // chọn quần thể mới theo bánh xe Roulette
        while (population.size() < count) {
            double r = 1.0 * rand() / RAND_MAX;
            vector<double>::iterator chosen_iter = lower_bound(acc_prob.begin(), acc_prob.end(), r);
            int chosen_idx = distance(acc_prob.begin(), chosen_iter);
            chosen_idx = min(chosen_idx, (int)acc_prob.size()-1);
            population.push_back(candidates[chosen_idx]);
        }
    }
};

int main() {
    srand ( time(NULL) );

    /*=== Đọc file đầu vào ===*/
    freopen(FILE, "r", stdin);
    cin >> N >> capacity;
    for (int i=0; i<N; i++)
        cin >> items[i].weight >> items[i].profit;

    /*=== Thuật toán GA ===*/
    // Khởi tạo quần thể
    Population pop;
    pop.init();
    cout << "Generation 0, best fitness = " << pop.best_indiv.fitness << endl;

    // vòng lặp tiến hóa
    for (int t=1; t<=MAX_GENERATION; t++) {
        // sinh sản
        pop.reproduction(POP_SIZE);

        // đánh giá cá thể
        for (Individual &indiv: pop.population)
            indiv.fitness = indiv.evaluate();

        // chọn lọc tự nhiên
        pop.natural_selection(POP_SIZE);

        // cập nhật cá thể tốt nhất
        pop.update_best();

        // in kết quả mỗi thế hệ
        cout << "Generation " << t << ", best fitness = " << pop.best_indiv.fitness << endl;
    }

    /*=== In kết quả cuối ===*/
    cout << "===========" << endl;
    cout << "FINAL RESULT:" << endl;
    cout << "Chosen items:";
    for (int i=0; i<N; i++)
        if (pop.best_indiv.chromosomes[i])
            cout << " " << i;
    cout << endl;
    cout << "Total profit: " << (long long)pop.best_indiv.fitness << endl;
//    cout << "Violated weight: " << max(0ll, pop.best_indiv.total_weight() - capacity) << endl;
}
