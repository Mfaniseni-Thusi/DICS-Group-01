#include "matrix2D.h"
#include "matrix3D.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define ROUNDS 100

// Pass FILE pointer as an argument
void test2DMatrix(unsigned int size, unsigned int numThreads, FILE *fp) {
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
    // Use fprintf to write to the file
    fprintf(fp, "2D Matrix %ux%u, Threads: %u, Avg Time: %f seconds\n", size, size, numThreads, totalTime / ROUNDS);
}

// Pass FILE pointer as an argument
void test3DMatrix(unsigned int size, unsigned int numThreads, FILE *fp) {
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
    // Use fprintf to write to the file
    fprintf(fp, "3D Matrix %ux%ux%u, Threads: %u, Avg Time: %f seconds\n", size, size, size, numThreads, totalTime / ROUNDS);
}

int main() {
    unsigned int sizes[] = {10, 20, 30};
    unsigned int numThreads[] = {1, 2, 3, 4};

    // Open a file for writing
    FILE *fp = fopen("matrix_performance.csv", "w");
    if (fp == NULL) {
        printf("Failed to open file for writing.\n");
        return -1;
    }

    // Write headers to the CSV
    fprintf(fp, "Matrix Type, Size, Threads, Avg Time (seconds)\n");

    for (int i = 0; i < sizeof(sizes) / sizeof(sizes[0]); i++) {
        for (int j = 0; j < sizeof(numThreads) / sizeof(numThreads[0]); j++) {
            test2DMatrix(sizes[i], numThreads[j], fp);
            test3DMatrix(sizes[i], numThreads[j], fp);
        }
    }

    // Close the file
    fclose(fp);

    return 0;
}

//gcc -o matrixProgram main.c matrix2D.c matrix3D.c -lpthread