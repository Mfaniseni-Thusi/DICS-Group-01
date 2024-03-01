#include "MatrixMultiplier.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h> // For time measurement and srand
#include <sys/time.h> // For gettimeofday

void runAndMeasure(int size) {
    struct timeval start, end;
    MatrixMultiplier* multiplier = createMatrixMultiplier(size);

    // Seed the random number generator
    srand(time(NULL));

    // Start timing
    gettimeofday(&start, NULL);

    // Perform matrix multiplication
    multiplyMatrices(multiplier);

    // End timing
    gettimeofday(&end, NULL);

    // Calculate and print the execution time
    double executionTime = (end.tv_sec - start.tv_sec) * 1000.0; // Convert to milliseconds
    executionTime += (end.tv_usec - start.tv_usec) / 1000.0;     // Convert microseconds to milliseconds
    printf("Execution Time for N=%d: %f ms\n", size, executionTime);

    // Clean up
    destroyMatrixMultiplier(multiplier);
}

int main() {
    // Perform and measure matrix multiplication for N=10, 20, 30
    runAndMeasure(10);
    runAndMeasure(20);
    runAndMeasure(30);

    return 0;
}
