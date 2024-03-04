#include "matrix2D.h"
#include "matrix3D.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define ROUNDS 100
#define NUM_SIZES 3  
#define NUM_THREADS 4  

double test2DMatrix(unsigned int size, unsigned int numThreads);
double test3DMatrix(unsigned int size, unsigned int numThreads);

int main() {
    unsigned int sizes[] = {10, 20, 30};
    double results2D[NUM_SIZES][NUM_THREADS];
    double results3D[NUM_SIZES][NUM_THREADS];

    // Perform tests and store results
    for (int i = 0; i < NUM_SIZES; i++) {
        for (int j = 0; j < NUM_THREADS; j++) {
            results2D[i][j] = test2DMatrix(sizes[i], j + 1);
            results3D[i][j] = test3DMatrix(sizes[i], j + 1);
        }
    }

    // Open a file for writing
    FILE *fp = fopen("matrix_performance.csv", "w");
    if (fp == NULL) {
        printf("Failed to open file for writing.\n");
        return -1;
    }

    // Write headers to the CSV
    fprintf(fp, "Dimension,Type,Sequential,2-Core,3-Core,4-Core\n");

    // Write results to the file
    for (int i = 0; i < NUM_SIZES; i++) {
        fprintf(fp, "%u,2D", sizes[i]);
        for (int j = 0; j < NUM_THREADS; j++) {
            fprintf(fp, ",%f", results2D[i][j]);
        }
        fprintf(fp, "\n");

        fprintf(fp, "%u,3D", sizes[i]);
        for (int j = 0; j < NUM_THREADS; j++) {
            fprintf(fp, ",%f", results3D[i][j]);
        }
        fprintf(fp, "\n");
    }

    // Close the file
    fclose(fp);

    return 0;
}

// Pass FILE pointer as an argument
double test2DMatrix(unsigned int size, unsigned int numThreads) {
    double totalTime = 0.0;
    for (int i = 0; i < ROUNDS; i++) {
        Matrix a = createRandomMatrix(size, size);
        Matrix b = createRandomMatrix(size, size);

        clock_t start = clock();
        if (numThreads == 1) {
            Matrix result = multiplySequential(&a, &b);
        } else {
            Matrix result = multiplyParallel(&a, &b, numThreads);
        }
        clock_t end = clock();

        totalTime += (double)(end - start) / CLOCKS_PER_SEC;
        // Free memory (to be implemented)
    }

    return totalTime / ROUNDS; 
}

// Pass FILE pointer as an argument
double test3DMatrix(unsigned int size, unsigned int numThreads) {
    double totalTime = 0.0;
    for (int i = 0; i < ROUNDS; i++) {
        Matrix3D a = createRandomMatrix3D(size, size, size);
        Matrix3D b = createRandomMatrix3D(size, size, size);

        clock_t start = clock();
        if (numThreads == 1) {
            Matrix3D result = multiplySequential3D(&a, &b);
        } else {
            Matrix3D result = multiplyParallel3D(&a, &b, numThreads);
        }
        clock_t end = clock();

        totalTime += (double)(end - start) / CLOCKS_PER_SEC;
        // Free memory (to be implemented)
    }

    return totalTime / ROUNDS;
}