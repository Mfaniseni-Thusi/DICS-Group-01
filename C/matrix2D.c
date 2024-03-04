#include "matrix2D.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

// Helper structure for parallel computation
typedef struct {
    const Matrix *a;
    const Matrix *b;
    Matrix *result;
    unsigned int startRow;
    unsigned int endRow;
} WorkerArgs;

// Initializes a matrix with random numbers in the interval [0, 20]
Matrix createRandomMatrix(unsigned int rows, unsigned int cols) {
    Matrix matrix;
    matrix.rows = rows;
    matrix.cols = cols;
    matrix.data = (unsigned int **)malloc(rows * sizeof(unsigned int *));
    if (matrix.data == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    for (unsigned int i = 0; i < rows; i++) {
        matrix.data[i] = (unsigned int *)malloc(cols * sizeof(unsigned int));
        if (matrix.data[i] == NULL) {
            perror("Memory allocation failed");
            // Free previously allocated memory to prevent leaks
            for (unsigned int j = 0; j < i; j++) {
                free(matrix.data[j]);
            }
            free(matrix.data);
            exit(EXIT_FAILURE);
        }
    }

    // Fill matrix with random numbers
    for (unsigned int i = 0; i < rows; i++) {
        for (unsigned int j = 0; j < cols; j++) {
            matrix.data[i][j] = rand() % 21;
        }
    }
    return matrix;
}

// Multiplies two matrices sequentially
Matrix multiplySequential(const Matrix *a, const Matrix *b) {
    Matrix result = createRandomMatrix(a->rows, b->cols);
    for (unsigned int i = 0; i < a->rows; i++) {
        for (unsigned int j = 0; j < b->cols; j++) {
            result.data[i][j] = 0; // Initialize element
            for (unsigned int k = 0; k < a->cols; k++) {
                result.data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }
    return result;
}

// Worker function for parallel matrix multiplication
void *multiplyWorker(void *args) {
    WorkerArgs *workArgs = (WorkerArgs *)args;
    for (unsigned int i = workArgs->startRow; i < workArgs->endRow; i++) {
        for (unsigned int j = 0; j < workArgs->b->cols; j++) {
            workArgs->result->data[i][j] = 0;
            for (unsigned int k = 0; k < workArgs->a->cols; k++) {
                workArgs->result->data[i][j] += workArgs->a->data[i][k] * workArgs->b->data[k][j];
            }
        }
    }
    return NULL;
}

// Multiplies two matrices in parallel using pthreads
Matrix multiplyParallel(const Matrix *a, const Matrix *b, unsigned int numThreads) {
    Matrix result = createRandomMatrix(a->rows, b->cols);
    pthread_t threads[numThreads];
    WorkerArgs args[numThreads];
    unsigned int rowsPerThread = a->rows / numThreads;
    unsigned int extraRows = a->rows % numThreads; // For handling rows not evenly divisible by numThreads

    for (unsigned int i = 0; i < numThreads; i++) {
        args[i].a = a;
        args[i].b = b;
        args[i].result = &result;
        args[i].startRow = i * rowsPerThread + (i < extraRows ? i : extraRows);
        args[i].endRow = args[i].startRow + rowsPerThread + (i < extraRows ? 1 : 0);
        int createResult = pthread_create(&threads[i], NULL, multiplyWorker, (void *)&args[i]);
        if (createResult != 0) {
            perror("Thread creation failed");
            // Clean up threads that have been created
            for (unsigned int j = 0; j < i; j++) {
                pthread_cancel(threads[j]);
            }
            exit(EXIT_FAILURE);
        }
    }

    // Join threads
    for (unsigned int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    return result;
}