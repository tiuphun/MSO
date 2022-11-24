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
#define POP_SIZE 20         // Số cá thể trong 1 quần thể
#define MAX_GENERATION 100  // Điều kiện dừng sau số thế hệ nhất định
#define CROSSOVER_RATE 0.9  // Tỉ lệ lai ghép: Xác suất để một cặp cha mẹ được chọn để lai ghép
#define MUTATION_RATE 0.1   // Tỉ lệ đột biến: Xác suất mỗi cá thể con bị đột biến


// Cấu trúc cho mỗi đồ vật
struct Item {
    int weight;     // Trọng lượng đồ vật
    int profit;     // Giá trị đồ vật
};

int capacity;       // Trọng lượng tối đa mà túi chứa được
Item items[MAX_N];  // Danh sách đồ vật
int N;              // Số đồ vật, đọc từ input


// Class biểu diễn một cá thể
/* Một cá thể, tức một lời giải sẽ có dạng một chuỗi nhị phân: 
    vd. với 24 đồ vật, 0 thể hiện trạng thái "bỏ lại", 1 thể hiện trạng thái "chọn vào túi"
    -> 111101000010100101111
*/
class Individual {
public:
    int dim;                    // Chiều dài chuỗi gen của cá thể
    bool chromosomes[MAX_N];    // Chuỗi gen biểu diễn đồ vật được chọn
    double fitness;             // Giá trị hàm thích nghi

    // Khởi tạo cá thể rỗng
    Individual() {
        this->dim = N;
    }

    // Hàm sao chép từ một cá thể sang cá thể khác
    Individual(const Individual &p) {
        this->dim = p.dim;
        for (int i=0; i<p.dim; i++)
            this->chromosomes[i] = p.chromosomes[i];
        this->fitness = p.fitness;
    }

    void random_init() {
        for (int i=0; i<this->dim; i++)
            if (1.0 * rand() / RAND_MAX < 0.5)
                this->chromosomes[i] = true;
            else
                this->chromosomes[i] = false;
    }

    long long total_profit() {
        // TO-DO: Trả về tổng giá trị đồ vật được chọn
        int profit = 0;
        for (int i=0; i<this->dim; i++)
            if (chromosome[i] == 1)
                profit += items[i].profit;
        return profit;
    }

    long long total_weight() {
        // TO-DO: Trả về tổng trọng lượng đồ vật được chọn
        int weight;
        for (int i=0; i<this->dim; i++)
            if (chromosome[i] == 1)
                weight += items[i].weight;
        return weight;
    }

    double evaluate() {
        // TO-DO: Trả về giá trị hàm thích nghi cho bài toán cái túi
        // Gợi ý: total_profit càng lớn càng tốt, còn total_weight <= capacity
    }

    static vector<Individual> crossover(Individual p1, Individual p2) {
        // TO-DO: Lai ghép hai cá thể p1, p2 và trả về vector chứa các cá thể con thu được
    }

    void mutate() {
        // TO-DO: Đột biến cho cá thể hiện tại
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

    // Cập nhật cá thể có fitness cao nhất
    void update_best() {
        for (Individual &p: population)
            if (p.fitness > best_indiv.fitness)
                best_indiv = p;
    }

    // Sinh sản = lai ghép + đột biến
    void reproduction(int count) {      //count là số cá thể con mình muốn sinh ra
        vector<Individual> offspring;   //vector chứa tất cả các thể con được sinh ra

        // Lai ghép
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
        while (offspring.size() > count) // vứt bớt nếu số cá thể con bị thừa
            offspring.pop_back();

        // Đột biến
        for (Individual &indiv: offspring)
            if (1.0 * rand() / RAND_MAX < MUTATION_RATE)
                indiv.mutate();

        population.insert(population.end(), offspring.begin(), offspring.end());
    }

    // Chọn lọc tự nhiên để chọn các cá thể sống sót
    void natural_selection(int count) {
        /* Mô tả: Các cá thể của quần thể được lưu trong biến vector<Individual> population
        Sau bước sinh sản (reproduction), population đang chứa nhiều hơn count cá thể (2x số cá thể ban đầu)
        Cần tiến hành chọn lọc tự nhiên để giữ lại đúng count cá thể trong biến population */
        // TO-DO: Thiết kế hàm chọn lọc tự nhiên để giữ lại count cá thể
        // Gợi ý: Cố gắng vừa chọn cá thể tốt vừa duy trì sự đa dạng của quần thể



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
    cout << "Total profit: " << pop.best_indiv.total_profit() << endl;
    cout << "Violated weight: " << max(0ll, pop.best_indiv.total_weight() - capacity) << endl;
}
