// MatrixMultiplier.h
#ifndef MATRIXMULTIPLIER_H
#define MATRIXMULTIPLIER_H

#include "Matrix3D.h"
#include "Thread3Ddata.h"
#include <pthread.h>

typedef struct {
    Matrix3D* matrixA;
    Matrix3D* matrixB;
    Matrix3D* matrixC;
    int size;
    pthread_t* threads;
    ThreadData3D* threadData;
} MatrixMultiplier;

MatrixMultiplier* createMatrixMultiplier(int size);
void destroyMatrixMultiplier(MatrixMultiplier* multiplier);
void multiplyMatrices(MatrixMultiplier* multiplier);
void multiplyMatricesSerial(MatrixMultiplier* multiplier);//classic serial method

#endif // MATRIXMULTIPLIER_H
