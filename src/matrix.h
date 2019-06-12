#ifndef MATRIX_H
#define MATRIX_H


#include <stdint.h>
#include <stdlib.h>

typedef struct matrixCDT *matrixADT;

matrixADT createMatrix(const size_t rows, const size_t cols, const size_t base);

void destroyMatrix(const matrixADT restrict matrix);

size_t getRows(const matrixADT restrict matrix);

size_t getCols(const matrixADT restrict matrix);

//retStatus assignToMatrix(const matrixADT restrict matrix,
//						 const int *restrict elems);

void fillRandomMatrix(const matrixADT restrict matrix);

void fillRandomKRankedMatrix(const matrixADT restrict matrix, const size_t k);

int getElem(const matrixADT restrict m, const size_t row, const size_t col);

void assignElem(const matrixADT restrict m, const size_t row, const size_t col,
				const int elem);

matrixADT copyMatrix(const matrixADT restrict m);

size_t rankOf(const matrixADT restrict matrix);

int determinantOfMatrix(const matrixADT restrict matrix);

void scalarMultiplyMatrix(const matrixADT restrict matrix, const int scalar);

matrixADT transposeMatrix(const matrixADT restrict matrix);

matrixADT multiplyMatrix(const matrixADT restrict m1,
						 const matrixADT restrict m2);

matrixADT inverseMatrix(const matrixADT restrict matrix);

matrixADT adjoint(const matrixADT restrict m);

matrixADT add(const matrixADT restrict m1, const matrixADT restrict m2);

matrixADT subtract(const matrixADT restrict m1, const matrixADT restrict m2);

matrixADT projectionMatrix(const matrixADT restrict a);

matrixADT horizontalCombineMatrix(const matrixADT restrict m1,
								  const matrixADT restrict m2);

void printMatrix(const matrixADT restrict matrix);

uint8_t *toCharArray(matrixADT m, size_t *size);

#endif
