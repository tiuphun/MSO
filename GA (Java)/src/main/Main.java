package main;

import main.ga.GA;
import main.ga.Individual;

import java.io.FileNotFoundException;
import java.util.Random;

public class Main {

    public static void main(String[] args) throws FileNotFoundException {

        Configs.rand = new Random(0);
        Problem.load_input_data("data/att48_33523.txt");

        GA solver = new GA();
        Individual best = solver.run();

    }
}
