#ifndef __matrixfunctions__
#define __matrixfunctions__

void add (size_t rows, size_t columns, int m1[rows][columns], int m2[rows][columns], int answer[rows][columns]);
void substract (size_t rows, size_t columns, int m1[rows][columns], int m2[rows][columns], int answer[rows][columns]);
void transpose (size_t rows, size_t columns, int m1[rows][columns], int m2[rows][columns], int answer[rows][columns]);
void multiply (size_t rows, size_t columns, int m1[rows][columns], int m2[rows][columns], int answer[rows][columns]);
void concat (size_t rows, size_t columns, int m1[rows][columns], int m2[rows][columns], int answer[rows][columns]);
#endif