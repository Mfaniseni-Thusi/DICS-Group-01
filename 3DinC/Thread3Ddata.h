// ThreadData3D.h
#ifndef THREADDATA3D_H
#define THREADDATA3D_H
#include "Matrix3D.h" 

typedef struct {
    int layer;
    Matrix3D* matrixA;
    Matrix3D* matrixB;
    Matrix3D* matrixC;
} ThreadData3D;


#endif // THREADDATA3D_H
