#ifndef MATRIX2D_H
#define MATRIX2D_H

#include <stdlib.h>
#include <pthread.h>

typedef struct {
    unsigned int rows;
    unsigned int cols;
    unsigned int **data;
} Matrix;

// Function declarations
Matrix createRandomMatrix(unsigned int rows, unsigned int cols);
Matrix multiplySequential(const Matrix *a, const Matrix *b);
Matrix multiplyParallel(const Matrix *a, const Matrix *b, unsigned int numThreads);

#endif // MATRIX2D_H
