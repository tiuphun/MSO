package main;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class Problem {

    public static int N;
    public static double[][] distance;

    public static void load_input_data(String filepath) throws FileNotFoundException {

        Scanner scanner = new Scanner(new File(filepath));
        N = scanner.nextInt();

        distance = new double[N][N];
        for (int i=0; i<N; i++) {
            for (int j=0; j<N; j++) {
                distance[i][j] = scanner.nextDouble();
            }
        }

        scanner.close();

        for (int i=0; i<N; i++) {
            for (int j=0; j<N; j++) {
                System.out.print(distance[i][j] + "\t");
            }
            System.out.println();
        }
    }
}
