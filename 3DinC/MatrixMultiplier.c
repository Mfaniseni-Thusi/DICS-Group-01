#include "MatrixMultiplier.h"
#include <stdlib.h>
#include <stdio.h>

// Helper function prototype for thread routine
static void* multiplyLayer(void* arg);

MatrixMultiplier* createMatrixMultiplier(int size) {
    MatrixMultiplier* multiplier = (MatrixMultiplier*)malloc(sizeof(MatrixMultiplier));
    if (!multiplier) {
        perror("Failed to allocate MatrixMultiplier");
        return NULL;
    }

    multiplier->size = size;
    multiplier->matrixA = createMatrix3D(size);
    multiplier->matrixB = createMatrix3D(size);
    multiplier->matrixC = createMatrix3D(size);
    multiplier->threads = (pthread_t*)malloc(size * sizeof(pthread_t));
    multiplier->threadData = (ThreadData3D*)malloc(size * sizeof(ThreadData3D));

    if (!multiplier->matrixA || !multiplier->matrixB || !multiplier->matrixC || !multiplier->threads || !multiplier->threadData) {
        perror("Failed to allocate resources for MatrixMultiplier");
        destroyMatrixMultiplier(multiplier);
        return NULL;
    }

    // Initialize ThreadData3D for each layer
    for (int i = 0; i < size; i++) {
        multiplier->threadData[i].layer = i;
        multiplier->threadData[i].matrixA = multiplier->matrixA;
        multiplier->threadData[i].matrixB = multiplier->matrixB;
        multiplier->threadData[i].matrixC = multiplier->matrixC;
    }

    return multiplier;
}

void destroyMatrixMultiplier(MatrixMultiplier* multiplier) {
    if (!multiplier) return;

    freeMatrix3D(multiplier->matrixA);
    freeMatrix3D(multiplier->matrixB);
    freeMatrix3D(multiplier->matrixC);
    free(multiplier->threads);
    free(multiplier->threadData);
    free(multiplier);
}

void multiplyMatrices(MatrixMultiplier* multiplier) {
    if (!multiplier) return;

    // Create and launch threads for each layer
    for (int i = 0; i < multiplier->size; i++) {
        if (pthread_create(&multiplier->threads[i], NULL, multiplyLayer, (void*)&multiplier->threadData[i])) {
            perror("Failed to create thread");
            exit(EXIT_FAILURE);
        }
    }

    // Join threads
    for (int i = 0; i < multiplier->size; i++) {
        if (pthread_join(multiplier->threads[i], NULL)) {
            perror("Failed to join thread");
            exit(EXIT_FAILURE);
        }
    }
}

// Thread routine to multiply a single layer of the matrices
static void* multiplyLayer(void* arg) {
    ThreadData3D* data = (ThreadData3D*)arg;
    int layer = data->layer;
    int size = data->matrixA->size;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            data->matrixC->data[layer][i][j] = 0;
            for (int k = 0; k < size; k++) {
                data->matrixC->data[layer][i][j] += data->matrixA->data[layer][i][k] * data->matrixB->data[layer][k][j];
            }
        }
    }

    pthread_exit(NULL);
}
