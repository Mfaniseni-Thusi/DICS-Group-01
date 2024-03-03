// Matrix3D.h
#ifndef MATRIX2D_H
#define MATRIX2D_H


#define N 10 // Change this to 20 and 30 for further experiments

// Struct representing a 3D matrix
typedef struct {
    int size;
    int **matrixA;
    int **matrixB;
    int **matrixC;
    int row;
} ThreadData;

// Function prototypes
void* multiplyRow(void* arg);
int** allocateAndInitMatrix(int size);
void printMatrix(int** matrix, int size, char* name);
void freeMatrix(int** matrix, int size);

#endif // MATRIX2D_H