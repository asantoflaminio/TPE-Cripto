#ifndef __matrixfunctions__
#define __matrixfunctions__

void add (size_t rows, size_t columns, int m1[rows][columns], int m2[rows][columns], int answer[rows][columns]);
void substract (size_t rows, size_t columns, int m1[rows][columns], int m2[rows][columns], int answer[rows][columns]);
void transpose (size_t rows, size_t columns, int m[rows][columns],  int answer[columns][rows]);
void multiply (size_t rows1, size_t columns1, size_t rows2, size_t columns2 ,int m1[rows1][columns1], int m2[rows2][columns2], int answer[rows1][columns2]);
void concat (size_t rows, size_t columns1,size_t columns2, int m1[rows][columns1], int m2[rows][columns2], int answer[rows][columns1+columns2]);
void printMatrix(size_t rows, size_t columns, int m[rows][columns]);
void printMatrixFloat(size_t rows, size_t columns, float m[rows][columns]);
void inverse(size_t size, int m[size][size], int answer[size][size]);
void cofactor(int size, int m[size][size], int ans[size][size], int forbidden_row, int forbidden_col);
int determinant(size_t size, int m[size][size]);
void adjoint(int size, int m[size][size],int answer[size][size]);
void gauss_jordan(int rows, int m[rows][rows+1], int** answer);
int mod_inverse(int a, int m);
void calculateProjection(int a_rows, int a_cols, int a_matrix[a_rows][a_cols], int rows, int cols, int answer[rows][cols]);
void concatWantedBoth(size_t wanted_cols, size_t rows, size_t columns1, size_t columns2, int m1[rows][columns1], int m2[rows][columns2], int answer[rows][wanted_cols+wanted_cols]);
void concatWantedRight(size_t wanted_cols, size_t rows, size_t columns1, size_t columns2, int m1[rows][columns1], int m2[rows][columns2], int answer[rows][columns1+wanted_cols]);
void separateMatrixByColumn(size_t wanted_cols1, size_t wanted_cols2, size_t rows, size_t columns, int m[rows][columns], int answer1[rows][wanted_cols1], int answer2[rows][wanted_cols2]);
#endif