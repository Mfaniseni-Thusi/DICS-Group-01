#include "MatrixMultiplier.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

void runAndMeasure(int size, const char* method, FILE* outputFile) {
    struct timeval start, end;
    MatrixMultiplier* multiplier = createMatrixMultiplier(size);

    gettimeofday(&start, NULL);

    if (strcmp(method, "parallel") == 0) {
        multiplyMatrices(multiplier);
    } else if (strcmp(method, "serial") == 0) {
        multiplyMatricesSerial(multiplier);
    }

    gettimeofday(&end, NULL);

    double executionTime = (end.tv_sec - start.tv_sec) * 1000.0;
    executionTime += (end.tv_usec - start.tv_usec) / 1000.0;
    printf("%s Execution Time for N=%d: %f ms\n", method, size, executionTime);

    // Output to CSV: size, method, executionTime
    fprintf(outputFile, "%d,%s,%f\n", size, method, executionTime);

    destroyMatrixMultiplier(multiplier);
}

int main() {
    FILE* outputFile = fopen("execution_times.csv", "w");
    if (outputFile == NULL) {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }

    // Write CSV header
    fprintf(outputFile, "Size,Method,ExecutionTime(ms)\n");

    for (int i = 0; i < 50; i++) {
        runAndMeasure(10, "serial", outputFile);
        runAndMeasure(10, "parallel", outputFile);
        runAndMeasure(20, "serial", outputFile);
        runAndMeasure(20, "parallel", outputFile);
        runAndMeasure(30, "serial", outputFile);
        runAndMeasure(30, "parallel", outputFile);
    }

    fclose(outputFile);
    return 0;
}
