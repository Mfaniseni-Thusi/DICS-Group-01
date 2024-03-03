// Matrix3D.c
#include "Matrix3D.h"
#include <stdlib.h>
#include <stdio.h>

Matrix3D* createMatrix3D(int size) {
    Matrix3D *matrix = (Matrix3D*)malloc(sizeof(Matrix3D));
    matrix->size = size;
    matrix->data = (int***)malloc(size * sizeof(int**));
    for (int i = 0; i < size; ++i) {
        matrix->data[i] = (int**)malloc(size * sizeof(int*));
        for (int j = 0; j < size; ++j) {
            matrix->data[i][j] = (int*)malloc(size * sizeof(int));
            for (int k = 0; k < size; ++k) {
                matrix->data[i][j][k] = rand() % 21; // Example: initialize with random numbers
            }
        }
    }
    return matrix;
}

void freeMatrix3D(Matrix3D* matrix) {
    for (int i = 0; i < matrix->size; ++i) {
        for (int j = 0; j < matrix->size; ++j) {
            free(matrix->data[i][j]);
        }
        free(matrix->data[i]);
    }
    free(matrix->data);
    free(matrix);
}

void printMatrix3D(const Matrix3D* matrix) {
    for (int i = 0; i < matrix->size; i++) {
        printf("Layer %d:\n", i);
        for (int j = 0; j < matrix->size; j++) {
            for (int k = 0; k < matrix->size; k++) {
                printf("%d\t", matrix->data[i][j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }
}
