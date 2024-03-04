#include "matrix3D.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

typedef struct {
    const Matrix3D *a;
    const Matrix3D *b;
    Matrix3D *result;
    unsigned int startDepth;
    unsigned int endDepth;
} WorkerArgs3D;

Matrix3D createRandomMatrix3D(unsigned int depth, unsigned int rows, unsigned int cols) {
    Matrix3D matrix;
    matrix.depth = depth;
    matrix.rows = rows;
    matrix.cols = cols;
    matrix.data = (unsigned int ***)malloc(depth * sizeof(unsigned int **));
    if (matrix.data == NULL) {
        perror("Memory allocation failed for depth");
        exit(EXIT_FAILURE);
    }
    for (unsigned int d = 0; d < depth; d++) {
        matrix.data[d] = (unsigned int **)malloc(rows * sizeof(unsigned int *));
        if (matrix.data[d] == NULL) {
            perror("Memory allocation failed for rows");
            // Free previously allocated memory
            for (unsigned int e = 0; e < d; e++) {
                for (unsigned int i = 0; i < rows; i++) {
                    free(matrix.data[e][i]);
                }
                free(matrix.data[e]);
            }
            free(matrix.data);
            exit(EXIT_FAILURE);
        }
        for (unsigned int i = 0; i < rows; i++) {
            matrix.data[d][i] = (unsigned int *)malloc(cols * sizeof(unsigned int));
            if (matrix.data[d][i] == NULL) {
                perror("Memory allocation failed for cols");
                // Free previously allocated memory
                for (unsigned int e = 0; e <= d; e++) {
                    for (unsigned int j = 0; e == d && j < i; j++) {
                        free(matrix.data[d][j]);
                    }
                    if (e < d) {
                        for (unsigned int j = 0; j < rows; j++) {
                            free(matrix.data[e][j]);
                        }
                    }
                    free(matrix.data[e]);
                }
                free(matrix.data);
                exit(EXIT_FAILURE);
            }
            for (unsigned int j = 0; j < cols; j++) {
                matrix.data[d][i][j] = rand() % 21; // Include 0 in the interval [0,20]
            }
        }
    }
    return matrix;
}

Matrix3D multiplySequential3D(const Matrix3D *a, const Matrix3D *b) {
    Matrix3D result = createRandomMatrix3D(a->depth, a->rows, a->cols);
    for (unsigned int d = 0; d < a->depth; d++) {
        for (unsigned int i = 0; i < a->rows; i++) {
            for (unsigned int j = 0; j < a->cols; j++) {
                result.data[d][i][j] = a->data[d][i][j] * b->data[d][i][j];
            }
        }
    }
    return result;
}

void *multiplyWorker3D(void *args) {
    WorkerArgs3D *workArgs = (WorkerArgs3D *)args;
    for (unsigned int d = workArgs->startDepth; d < workArgs->endDepth; d++) {
        for (unsigned int i = 0; i < workArgs->a->rows; i++) {
            for (unsigned int j = 0; j < workArgs->a->cols; j++) {
                workArgs->result->data[d][i][j] = workArgs->a->data[d][i][j] * workArgs->b->data[d][i][j];
            }
        }
    }
    return NULL;
}

Matrix3D multiplyParallel3D(const Matrix3D *a, const Matrix3D *b, unsigned int numThreads) {
    Matrix3D result = createRandomMatrix3D(a->depth, a->rows, a->cols);
    pthread_t threads[numThreads];
    WorkerArgs3D args[numThreads];
    unsigned int depthPerThread = a->depth / numThreads;
    unsigned int extraDepth = a->depth % numThreads; // Handle depth not evenly divisible

    for (unsigned int i = 0; i < numThreads; i++) {
        args[i].a = a;
        args[i].b = b;
        args[i].result = &result;
        args[i].startDepth = i * depthPerThread + (i < extraDepth ? i : extraDepth);
        args[i].endDepth = args[i].startDepth + depthPerThread + (i < extraDepth ? 1 : 0);
        if (pthread_create(&threads[i], NULL, multiplyWorker3D, (void *)&args[i]) != 0) {
            perror("Failed to create thread");
            exit(EXIT_FAILURE);
        }
    }

    for (unsigned int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    return result;
}
