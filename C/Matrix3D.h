#ifndef MATRIX3D_H
#define MATRIX3D_H

#include <stdlib.h>
#include <pthread.h>

typedef struct {
    unsigned int depth;
    unsigned int rows;
    unsigned int cols;
    unsigned int ***data;
} Matrix3D;

// Function declarations
Matrix3D createRandomMatrix3D(unsigned int depth, unsigned int rows, unsigned int cols);
Matrix3D multiplySequential3D(const Matrix3D *a, const Matrix3D *b);
Matrix3D multiplyParallel3D(const Matrix3D *a, const Matrix3D *b, unsigned int numThreads);

#endif // MATRIX3D_H
