#ifndef __matrixfunctions__
#define __matrixfunctions__

void add (size_t rows, size_t columns, int m1[rows][columns], int m2[rows][columns], int answer[rows][columns]);
void substract (size_t rows, size_t columns, int m1[rows][columns], int m2[rows][columns], int answer[rows][columns]);
void transpose (size_t rows, size_t columns, int m[rows][columns],  int answer[columns][rows]);
void multiply (size_t rows1, size_t columns1, size_t rows2, size_t columns2 ,int m1[rows1][columns1], int m2[rows2][columns2], int answer[rows1][columns2]);
void concat (size_t rows, size_t columns1,size_t columns2, int m1[rows][columns1], int m2[rows][columns2], int answer[rows][columns1+columns2]);
void printMatrix(size_t rows, size_t columns, int m[rows][columns]);
#endif