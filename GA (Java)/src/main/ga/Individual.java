package main.ga;

import main.Problem;

import java.util.ArrayList;
import java.util.Comparator;

public class Individual implements Comparator<Individual> {

    public ArrayList<Integer> chromosome;
    public double fitness;

    public Individual() {
        fitness = -1;
    }

    public Individual(ArrayList<Integer> chromosome) {
        this.chromosome = new ArrayList<>();
        this.chromosome.addAll(chromosome);
        this.fitness = -1;
    }

    public void random_init() {

    }

    public double calculate_fitness() {
        if (chromosome == null || chromosome.size() <= 1) {
            return -1;
        }

        int N = chromosome.size();
        double obj = Problem.distance[chromosome.get(N-1)][chromosome.get(0)];
        for (int i=1; i<N; i++) {
            obj += Problem.distance[chromosome.get(i-1)][chromosome.get(i)];
        }

        return 1.0 / obj;
    }

    @Override
    public int compare(Individual o1, Individual o2) {
        return -Double.valueOf(o1.fitness).compareTo(o2.fitness);
    }
}
