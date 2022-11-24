//#include <bits/stdc++.h>
#include<iostream>
#include<vector>
#include<math.h>
#include<algorithm>
#include<random>
using namespace std;

const int MAX_N = 100;
int dim;
double low[MAX_N], high[MAX_N];

double sphere_function(double x[]) {
    double sum = 0.0;
    for (int i=0; i<dim; i++)
        sum += x[i] * x[i];
    return sum;
}

double evaluate(vector<double> x) {
    double y[dim];
    for (int i=0; i<dim; i++) {
//        cout << x[i];
        y[i] = low[i] + (high[i] - low[i]) * x[i];
    }
//    cout <<endl;
    return sphere_function(y);
}

inline double randDouble() {
    return 1.0 * rand() / RAND_MAX;
}

inline double gauss(double mu, double sigma){
    return mu + sigma * sqrt(-2.0 * log(randDouble())) * sin(2.0 * M_PI * randDouble());
}

int main() {
//    freopen("test.txt", "r", stdin);
//    int dim;
//    double low[MAX_N], high[MAX_N];

    dim = 10;
    for (int i=0; i<dim; i++) {
        low[i]  = -50;
        high[i] = 10;
    }

    double mu[MAX_N], sigma[MAX_N];
    int POP_MU = 10, POP_LAMBDA = 20;
    int MAX_ITERATIONS = 100;

    for (int i=0; i<dim; i++) {
        mu[i] = 0.0;
        sigma[i] = 1.0;
    }

    vector<vector<double> > population;

    for (int g=0; g<MAX_ITERATIONS; g++) {
        // sinh POP_LAMBDA cá thể
        population.clear();
        for (int k=0; k<POP_LAMBDA; k++) {
            vector<double> indiv;
            for (int i=0; i<dim; i++)
                indiv.push_back(gauss(mu[i], sigma[i]));
            population.push_back(indiv);
        }

        // chọn POP_MU cá thể tốt nhất theo hàm evaluate()
        sort(population.begin(), population.end(), [](const vector<double>& indiv1,
                                                      const vector<double>& indiv2) {
            return evaluate(indiv1) < evaluate(indiv2);
        });
        population.resize(POP_MU);

//        for (int k=0; k<POP_MU; k++)
//            cout << evaluate(population[k]) << endl;

        // cập nhật mu và sigma theo POP_MU cá thể tốt nhất
        for (int i=0; i<dim; i++) {

            // cập nhât mu
            mu[i] = 0.0;
            for (int k=0; k<POP_MU; k++)
                mu[i] += population[k][i];
            mu[i] /= POP_MU;

            // cập nhật sigma
            sigma[i] = 0.0;
            for (int k=0; k<POP_MU; k++)
                sigma[i] += (population[k][i] - mu[i]) * (population[k][i] - mu[i]);

            sigma[i] = sqrt(sigma[i] / (POP_MU-1));
            if (sigma[i] < 0.01)
                sigma[i] = 0.01;    // chặn cận dưới để tránh mắc ở cực tiểu địa phương
        }

        // in kết quả
        cout << "Iteration " << g << ", best fitness: " << evaluate(population[0]) << endl;
    }
}
/* NOTES
    Phải chuyển qua chuyển lại giá trị trong khoảng 0->1 và khoảng low->high để giảm thiểu khó khăn trong việc xử lí cận.
    (Do mỗi không gian có thể có cặp cận khác nhau.)
*/