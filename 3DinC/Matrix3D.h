// Matrix3D.h
#ifndef MATRIX3D_H
#define MATRIX3D_H

// Struct representing a 3D matrix
typedef struct {
    int size;
    int ***data;
} Matrix3D;

// Function prototypes
Matrix3D* createMatrix3D(int size);
void freeMatrix3D(Matrix3D* matrix);
void printMatrix3D(const Matrix3D* matrix);


#endif // MATRIX3D_H
