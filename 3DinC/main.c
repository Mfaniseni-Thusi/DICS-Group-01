#include "MatrixMultiplier.h"
#include "Matrix2D.h"
#include "Matrix3D.h"
#include "Thread3Ddata.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

void runAndMeasure(const char *dimension, int size, const char *method, FILE *outputFile)
{
    struct timeval start, end;
    MatrixMultiplier *multiplier = createMatrixMultiplier(size);

    srand(time(NULL)); // Seed the random number generator

    if (strcmp(dimension, "2D"))
    {
        gettimeofday(&start, NULL);

        pthread_t threads[N];
        ThreadData data[N];

        // Allocate and initialize matrices A, B, and C
        int **A = allocateAndInitMatrix(N);
        int **B = allocateAndInitMatrix(N);
        int **C = allocateAndInitMatrix(N);

        // Print matrices A and B for debugging
        printMatrix(A, N, "A");
        printMatrix(B, N, "B");

        // Create threads for each row
        for (int i = 0; i < N; i++)
        {
            data[i].size = N;
            data[i].matrixA = A;
            data[i].matrixB = B;
            data[i].matrixC = C;
            data[i].row = i;
            if (pthread_create(&threads[i], NULL, multiplyRow, (void *)&data[i]))
            {
                fprintf(stderr, "Error creating thread\n");
                // Free allocated memory in case of error
                freeMatrix(A, N);
                freeMatrix(B, N);
                freeMatrix(C, N);
            }
        }

        // Wait for all threads to complete
        for (int i = 0; i < N; i++)
        {
            pthread_join(threads[i], NULL);
        }

        // Cleanup and memory deallocation
        freeMatrix(A, N);
        freeMatrix(B, N);
        freeMatrix(C, N);

        gettimeofday(&end, NULL);

        method = dimension + *method;
    }
    else
    {
        gettimeofday(&start, NULL);

        if (strcmp(method, "parallel") == 0)
        {
            multiplyMatrices(multiplier);
        }
        else if (strcmp(method, "serial") == 0)
        {
            multiplyMatricesSerial(multiplier);
        }

        gettimeofday(&end, NULL);

        method = dimension + *method;
    }

    double executionTime = (end.tv_sec - start.tv_sec) * 1000.0;
    executionTime += (end.tv_usec - start.tv_usec) / 1000.0;
    printf("%s Execution Time for N=%d: %f ms\n", method, size, executionTime);

    // Output to CSV: size, method, executionTime
    fprintf(outputFile, "%d,%s,%f\n", size, method, executionTime);

    destroyMatrixMultiplier(multiplier);
}

int main()
{

    FILE *outputFile = fopen("execution_times.csv", "w");
    if (outputFile == NULL)
    {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }

    // Write CSV header
    fprintf(outputFile, "Size,Method,ExecutionTime(ms)\n");

    for (int i = 0; i < 50; i++)
    {
        runAndMeasure("3D", 10, "serial", outputFile);
        runAndMeasure("3D", 10, "parallel", outputFile);
        runAndMeasure("3D", 20, "serial", outputFile);
        runAndMeasure("3D", 20, "parallel", outputFile);
        runAndMeasure("3D", 30, "serial", outputFile); 
        runAndMeasure("3D", 30, "parallel", outputFile);

        runAndMeasure("2D", 10, "parallel", outputFile);
        runAndMeasure("2D", 20, "parallel", outputFile);
        runAndMeasure("2D", 30, "parallel", outputFile);
    }

    fclose(outputFile);
    return 0;
}
