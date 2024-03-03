#include "Matrix2D.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h> // For seeding the random number generator

void* multiplyRow(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int n = data->size;
    int row = data->row;

    for (int j = 0; j < n; j++) {
        data->matrixC[row][j] = 0;
        for (int k = 0; k < n; k++) {
            data->matrixC[row][j] += data->matrixA[row][k] * data->matrixB[k][j];
        }
    }
    pthread_exit(NULL);
}

// Function to allocate and initialize a matrix
int** allocateAndInitMatrix(int size) {
    int** matrix = (int**)malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        matrix[i] = (int*)malloc(size * sizeof(int));
        for (int j = 0; j < size; j++) {
            matrix[i][j] = rand() % 21; // Random numbers in the range [0, 20]
        }
    }
    return matrix;
}

// Function to print a matrix
void printMatrix(int** matrix, int size, char* name) {
    printf("Matrix %s:\n", name);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Function to free allocated memory for a matrix
void freeMatrix(int** matrix, int size) {
    for (int i = 0; i < size; i++) {
        free(matrix[i]);
    }
    free(matrix);
}